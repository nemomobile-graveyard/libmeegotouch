/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifdef QT_OPENGL_LIB
#include <QtOpenGL>
#endif

#include "mwindow.h"
#include "mwindow_p.h"

#include "mscene.h"
#include "mscene_p.h"
#include "mapplication.h"
#include "mapplication_p.h"
#ifndef Q_OS_WIN
#include "mcomponentcache.h"
#endif
#include "morientationtracker.h"
#include "mdeviceprofile.h"
#include "mdeviceprofile.h"
#include "mwidget.h"
#include "mcomponentdata.h"
#include "morientationchangeevent.h"
#include "mondisplaychangeevent.h"
#include "mdebug.h"
#include "mgconfitem.h"
#include "mlocale.h"
#include "mgraphicssystemhelper.h"
#include "mwindowstyle.h"
#include <mwindowborderdecorator.h>

#include <QPropertyAnimation>
#include <QSettings>
#include <QDir>
#include <QTimer>
#include <QDynamicPropertyChangeEvent>

#include "morientationtracker_p.h"
#include "mscenemanager_p.h"

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
// Avoid conflict with QEvent::KeyPress usage in MWindow::Event
# undef KeyPress
#endif

namespace {
    const QString ImagesPath(QDir::homePath() + "/MyDocs/.images");
    const int DisplayExitedDelay = 1000; //ms.
#ifdef Q_WS_X11
    const char* FollowsCurrentApplicationWindowOrientationPropertyName =
            "followsCurrentApplicationWindowOrientation";
#endif
    const char* AnimatedOrientationChangePropertyName =
            "animatedOrientationChange";
}

/// Actual class

MWindowPrivate::MWindowPrivate() :
    glWidget(0),
    glContext(0),
    sceneManager(0),
    oldOrientation(M::Landscape), // the initial value is not used at all
    orientationAngleLocked(false),
    orientationLocked(false),
    isLogicallyClosed(true),
    isInSwitcher(false),
    closeOnLazyShutdown(false),
    delayedMOnDisplayChangeEvent(0),
    onDisplay(false),
    onDisplaySet(false),
    displayExitedTimer(),
    visibleInSwitcher(false),
    fullyObscured(false),
    updateIsPending(false),
    discardedPaintEvent(false),
    beforeFirstPaintEvent(true),
    invisiblePaintCounter(0),
    allowedPaintEventsWhenInvisible(5),
    borderDecorator(0),
    q_ptr(NULL)
{
#ifdef Q_WS_X11
    notificationPreviewDisabled = false;
#endif

    MWindow *window = MApplication::activeWindow();

    if (window)
        angle = window->orientationAngle();
    else
        angle = MOrientationTracker::instance()->orientationAngle();

    timeSinceLastPaintInSwitcher.invalidate();
}

MWindowPrivate::~MWindowPrivate()
{
    delete delayedMOnDisplayChangeEvent;
    delete borderDecorator;
}

void MWindowPrivate::init()
{
    Q_Q(MWindow);

    displayExitedTimer.connect(&displayExitedTimer, SIGNAL(timeout()),
                               q, SLOT(_q_exitDisplayStabilized()));
    displayExitedTimer.setInterval(DisplayExitedDelay);
    displayExitedTimer.setSingleShot(true);

#ifdef Q_WS_X11
    // We do window decorations ourselves. Set env variable accordingly for
    // development purposes

#ifdef M_OS_MAEMO5
    if ( !MApplication::fullScreen() ) {
#endif //M_OS_MAEMO5

    QString env = qgetenv("M_DECORATED");
    if (env.contains("0")) {
        q->setWindowFlags(Qt::FramelessWindowHint);
    }
    /* Workaround until we get M_DECORATED defined in the target env */
#ifdef __arm__
    else if (env.isEmpty()) {
        q->setWindowFlags(Qt::FramelessWindowHint);
    }
#endif // __arm__

#ifdef M_OS_MAEMO5
    }
#endif //M_OS_MAEMO5

#endif // Q_WS_X11

    // resize always to the display's native orientation
    // since it's not the window but the scene content that is rotated
    q->resize(q->visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(M::Angle0)));
    q->setFrameStyle(0);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (MApplication::softwareRendering() == false) {
#ifdef QT_OPENGL_LIB
        mDebug("MWindowPrivate") << "Renderer: OpenGL";
#else
        mDebug("MWindowPrivate") << "Renderer: Software";
#endif
    } else {
        mDebug("MWindowPrivate") << "Renderer: Software (forced)";
    }

#ifdef Q_WS_X11
    appendVisibilityChangeMask();
    // If the window is created for the application in prestarted mode
    // we have to set X11 property _MEEGOTOUCH_PRESTARTED
    if(MApplication::isPrestarted()) {
        setX11PrestartProperty(true);
        resolveOrientationRules();
    }
    setX11OrientationAngleProperty(angle);
#endif

    q->setTranslucentBackground(false);
    if (MApplication::fullScreen())
        q->showFullScreen();

#ifdef Q_WS_X11
    if (MApplication::softwareRendering() || MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()) {
        applyStartupWindowBackground();
    }
#endif //Q_WS_X11
}

void MWindowPrivate::initSoftwareViewport()
{
    Q_Q(MWindow);

    mDebug("MWindow") << "Switching to software rendering";

    MGraphicsSystemHelper::switchToSoftwareRendering(q);

    q->setViewportUpdateMode(MWindow::MinimalViewportUpdate);

    configureViewport();
}

void MWindowPrivate::initGLViewport()
{
    Q_Q(MWindow);

#ifdef QT_OPENGL_LIB
    mDebug("MWindow") << "Window restored, switching to GL rendering";

    bool translucent = q->testAttribute(Qt::WA_TranslucentBackground);

    MGraphicsSystemHelper::switchToHardwareRendering(q, &glContext);

    if (translucent) {
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::transparent);
        palette.setColor(QPalette::Window, Qt::transparent);
        q->setAutoFillBackground(true);
        q->setPalette(palette);
        q->viewport()->setAutoFillBackground(true);
        q->viewport()->setPalette(palette);
    }
#endif // QT_OPENGL_LIB

    q->setViewportUpdateMode(MWindow::FullViewportUpdate);

    configureViewport();
}

void MWindowPrivate::configureViewport()
{
    Q_Q(MWindow);

    q->grabGesture(Qt::TapAndHoldGesture);
    q->grabGesture(Qt::PinchGesture);
    q->grabGesture(Qt::PanGesture);
    q->grabGesture(Qt::SwipeGesture);
    q->grabGesture(Qt::TapGesture);

    q->setAttribute(Qt::WA_AcceptTouchEvents);

    if (!beforeFirstPaintEvent) {
        disableAutomaticBackgroundRepainting();
    }
}

void MWindowPrivate::disableAutomaticBackgroundRepainting()
{
    Q_Q(MWindow);

    // If we don't set these flags, the technique of discarding paintEvent() calls
    // for limiting the framerate (e.g. because window is a thumbnail in the
    // application switcher) will cause the window to flicker between its regular
    // appearance and a blank window.
    // That's because even though we avoided the actual rendering of the contents,
    // by the time paintEvent() is called Qt might have already painted the system
    // background over of the window's previous state. This seems to be happening only
    // with software rendering.
    q->setAttribute(Qt::WA_OpaquePaintEvent);
    q->setAttribute(Qt::WA_NoSystemBackground);
    q->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    q->viewport()->setAttribute(Qt::WA_NoSystemBackground);
}


//! \internal
class ScreenshotEffect : public QGraphicsWidget
{
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0)
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->fillRect(boundingRect(), Qt::white);
    }
};
//! \internal_end

void MWindowPrivate::takeScreenshot()
{
    Q_Q(MWindow);

    QPixmap screenshot;
    screenshot = QPixmap::grabWindow(q->effectiveWinId());

    QString path;
    if (QDir(ImagesPath).exists())
        path = ImagesPath;
    else
        path = QDir::homePath();

    if (!screenshot.save(QString("%1/%2-%3.png").arg(path)
        .arg(QDate::currentDate().toString("yyyyMMdd"))
        .arg(QTime::currentTime().toString("hhmmss"))))
        mWarning("MWindow") << "Could not save screenshot to" << path;

    playScreenshotEffect();
}

void MWindowPrivate::playScreenshotEffect()
{
    Q_Q(MWindow);

    ScreenshotEffect *flash = new ScreenshotEffect();
    flash->setGeometry(0, 0, q->width(), q->height());

    QPropertyAnimation *animation = new QPropertyAnimation(flash, "opacity", q);
    animation->setDuration(400);
    animation->setEndValue(0.0f);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    q->scene()->addItem(flash);

    QObject::connect(animation, SIGNAL(finished()), flash, SLOT(deleteLater()));
}

void MWindowPrivate::rotateWindowsFromKeyEvent(QKeyEvent *event) {
    foreach (MWindow *window, MApplication::windows()) {

        int newAngle = (window->orientationAngle()
                        + ((event->modifiers() & Qt::AltModifier) ? 270 : 90)) % 360;
        M::OrientationAngle newOrientationAngle = static_cast<M::OrientationAngle>(newAngle);
        M::Orientation newOrientation = MDeviceProfile::instance()->orientationFromAngle(newOrientationAngle);

        if (!window->isOrientationAngleLocked()) {
            if (!window->isOrientationLocked() || (window->orientation() == newOrientation)) {
                window->setOrientationAngle(newOrientationAngle);
            }
        }
    }
}

WId MWindowPrivate::robustEffectiveWinId() const
{
    Q_Q(const MWindow);
    if (q->isVisible()) {
        return q->effectiveWinId();
    } else {
        if (QWidget *parent = q->parentWidget()) {
            while (parent->parentWidget()) {
                parent = parent->parentWidget();
            }
            return parent->winId();
        } else {
            return  q->winId();
        }
    }
}

#ifdef Q_WS_X11
void MWindowPrivate::appendVisibilityChangeMask()
{
    XWindowAttributes existingAttributes;
    XSetWindowAttributes newAttributes;
    Status status;

   status = XGetWindowAttributes(QX11Info::display(), robustEffectiveWinId(), &existingAttributes);
    if (status == 0) {
        qFatal("MWindow: XGetWindowAttributes() failed!");
    }

    newAttributes.event_mask = existingAttributes.your_event_mask | VisibilityChangeMask;

    XChangeWindowAttributes(QX11Info::display(), robustEffectiveWinId(), CWEventMask, &newAttributes);
}

void MWindowPrivate::applyStartupWindowBackground()
{
    Q_Q(MWindow);

    const MWindowStyle *style =
            static_cast<const MWindowStyle *>(MTheme::style("MWindowStyle",
                                                            QString(), QString(), QString(),
                                                            q->orientation()));
    if (!style) {
        mWarning("MWindow") << "No MWindowStyle found";
        return;
    }

    WId windowId = robustEffectiveWinId();
    const QPixmap *startupPixmap = style->x11StartupPixmap();
    const QColor startupFillColor = style->startupFillColor();
    if (startupPixmap) {
        XSetWindowBackgroundPixmap(QX11Info::display(), windowId, startupPixmap->handle());
    } else if (startupFillColor.isValid()) {
        XSetWindowBackground(QX11Info::display(), windowId,
                             QColormap::instance().pixel(startupFillColor));
    } else {
        disableAutomaticBackgroundRepainting();
    }

    MTheme::releaseStyle(style);
}

void MWindowPrivate::resolveOrientationRules() {
    Q_Q(MWindow);

    //follow current app window if window is visible in switcher
    if ((isInSwitcher && q->isOnDisplay()) ||
        (q->isVisible() && !q->isMinimized() && !q->isOnDisplay())) {
        MOrientationTracker::instance()->d_ptr->startFollowingCurrentAppWindow(q, true);
    } else {
        MOrientationTracker::instance()->d_ptr->stopFollowingCurrentAppWindow(q, true);
    }

    MOrientationTracker::instance()->d_ptr->resolveIfOrientationUpdatesRequired();

}
#endif

void MWindowPrivate::_q_onPixmapRequestsFinished()
{
    Q_Q(MWindow);

    q->disconnect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()),
                  q, SLOT(_q_onPixmapRequestsFinished()));
    q->setVisible(true);
}

void MWindowPrivate::handleApplicationLayoutDirectionChangeEvent(QGraphicsItem *item)
{
    if (item->isWidget()) {
        QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
        Qt::LayoutDirection direction = qApp->layoutDirection();
        // if the direction has not changed or has been specified
        // directly, do not update.
        if (((direction == Qt::RightToLeft) == widget->testAttribute(Qt::WA_RightToLeft))
                || widget->testAttribute(Qt::WA_SetLayoutDirection))
            return;
        widget->setAttribute(Qt::WA_RightToLeft, (direction == Qt::RightToLeft));
        // Send the notification event to this widget item.
        QEvent e(QEvent::LayoutDirectionChange);
        QApplication::sendEvent(widget, &e);
    }
    // Propagate this change to all children.
    const int size = item->childItems().size();
    for (int i = 0; i < size; ++i) {
        QGraphicsItem *childItem = item->childItems().at(i);
        handleApplicationLayoutDirectionChangeEvent(childItem);
    }
}

void MWindowPrivate::handleLanguageChangeEvent(QGraphicsItem *item)
{
    if (item->isWidget()) {
        QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);
        if (qobject_cast<MWidget*> (widget)) {
            // If it is a MWidget, sent it the language change event
            // to trigger the retranslateUi() method of the MWidget:
            QEvent e(QEvent::LanguageChange);
            qApp->sendEvent(widget, &e);
        }
    }
    // Propagate this change to all children.
    const int size = item->childItems().size();
    for (int i = 0; i < size; ++i) {
        QGraphicsItem *childItem = item->childItems().at(i);
        handleLanguageChangeEvent(childItem);
    }
}

bool MWindow::isInSwitcher() const
{
    Q_D(const MWindow);

    return d->isInSwitcher;
}

// return true if modifiers match what is required for debug keyboard shortcuts
bool MWindowPrivate::debugShortcutModifiersPresent(Qt::KeyboardModifiers modifiers) const
{
    return (modifiers & (Qt::ControlModifier | Qt::AltModifier)
            && (modifiers & Qt::ShiftModifier));
}

void MWindowPrivate::notifyWidgetsAboutOrientationChange()
{
    Q_Q(MWindow);

    M::Orientation newOrientation = q->orientation();

    if (sceneManager == 0 && oldOrientation != newOrientation) {
        QGraphicsScene *graphicsScene = q->QGraphicsView::scene();
        MScenePrivate::notifySceneAboutOrientationChange(graphicsScene, newOrientation);

        emit q->orientationChanged(newOrientation);
        emit q->orientationChangeFinished(newOrientation);
    }
}

void MWindowPrivate::doEnterDisplayEvent()
{
    Q_Q(MWindow);

    onDisplay = true;
    onDisplaySet = true;

    q->enterDisplayEvent();
    emit q->displayEntered();

#ifdef Q_WS_X11
    resolveOrientationRules();
#endif //Q_WS_X11

    if (discardedPaintEvent) {
        // we discarded a paint event while beeing invisible
        // make sure the screen is up to date
        discardedPaintEvent = false;
        invisiblePaintCounter = 0;
        QTimer::singleShot(0, q->viewport(), SLOT(update()));
    }
}

void MWindowPrivate::doExitDisplayEvent()
{
    Q_Q(MWindow);

    onDisplay = false;
    onDisplaySet = true;

    q->exitDisplayEvent();
    emit q->displayExited();
#ifdef Q_WS_X11
    resolveOrientationRules();
#endif

    QPixmapCache::clear();
}

void MWindowPrivate::_q_exitDisplayStabilized()
{
    Q_Q(MWindow);
    if (delayedMOnDisplayChangeEvent) {
        q->onDisplayChangeEvent(delayedMOnDisplayChangeEvent);
        delete delayedMOnDisplayChangeEvent;
        delayedMOnDisplayChangeEvent = 0;
    }
}

void MWindowPrivate::propagateMOnDisplayChangeEventToScene(MOnDisplayChangeEvent *event)
{
    Q_Q(MWindow);

    MOnDisplayChangeEvent::State eventState;

    if (event->state() == MOnDisplayChangeEvent::PartiallyOnDisplay ||
            event->state() == MOnDisplayChangeEvent::FullyOnDisplay ||
            event->state() == MOnDisplayChangeEvent::MustBeResolved) {
        eventState = MOnDisplayChangeEvent::MustBeResolved;
    } else {
        eventState = MOnDisplayChangeEvent::FullyOffDisplay;
    }


    MOnDisplayChangeEvent ev(eventState, q->sceneRect());

    // FIXME:
    // Actually sending the event would require overriding customEvent() which
    // would mess ABI compatibility. Calling the event handler directly for now
    q->scene()->d_func()->onDisplayChangeEvent(&ev);

}

void MWindowPrivate::_q_enablePaintUpdates()
{
    Q_Q(MWindow);

    q->setUpdatesEnabled(true);
}

void MWindowPrivate::ensureOrientationAngleIsUpToDateBeforeShowing()
{
    Q_Q(MWindow);

    if (!q->isVisible() && !q->isOrientationAngleLocked()
        && MOrientationTracker::instance()->orientationAngle() != q->orientationAngle()) {

#ifdef HAVE_CONTEXTSUBSCRIBER
        // We are about to be shown but our orientation angle is outdated.
        MOrientationTracker::instance()->d_ptr->updateOrientationAngle();
#endif

        q->setUpdatesEnabled(true);
        q->update();
    }
}

MSceneManager::TransitionMode MWindowPrivate::orientationChangeTransitionMode()
{
    Q_Q(MWindow);
    MSceneManager::TransitionMode transitionMode;

    bool animatedOrientationChange = true;


    if (q->property(AnimatedOrientationChangePropertyName).isValid()) {
        animatedOrientationChange = q->property(AnimatedOrientationChangePropertyName).toBool();
    }

    if (animatedOrientationChange && q->isVisible()) {
        transitionMode = MSceneManager::AnimatedTransition;
    } else {
        transitionMode = MSceneManager::ImmediateTransition;
    }

    return transitionMode;
}

void MWindowPrivate::doSwitcherExited()
{
    Q_Q(MWindow);

    if (isInSwitcher) {
        isInSwitcher = false;
#ifdef Q_WS_X11
        resolveOrientationRules();
#endif
        emit q->switcherExited();
    }
}

void MWindowPrivate::doSwitcherEntered()
{
    Q_Q(MWindow);

    if (!isInSwitcher) {
        isInSwitcher = true;
#ifdef Q_WS_X11
        resolveOrientationRules();
#endif
        emit q->switcherEntered();
    }
}

void MWindowPrivate::handleWindowStateChangeEvent(QWindowStateChangeEvent *event)
{
    Q_Q(MWindow);

    // Check if window has entered / left the switcher
    if (!event->oldState().testFlag(Qt::WindowMinimized) && q->windowState().testFlag(Qt::WindowMinimized)) {
        doSwitcherEntered();
        QPixmapCache::clear();
    }
    else if (event->oldState().testFlag(Qt::WindowMinimized) &&
             !q->windowState().testFlag(Qt::WindowMinimized)) {
        doSwitcherExited();
        timeSinceLastPaintInSwitcher.invalidate();
    }

#ifdef QT_OPENGL_LIB
    if (MApplication::softwareRendering())
        return;

    if (!event->oldState().testFlag(Qt::WindowMinimized) && q->windowState().testFlag(Qt::WindowMinimized)) {
        initSoftwareViewport();
        MTheme::cleanupGarbage();
    } else if (event->oldState().testFlag(Qt::WindowMinimized)
               && !q->windowState().testFlag(Qt::WindowMinimized)) {
        if (MGraphicsSystemHelper::isRunningNativeGraphicsSystem()) {
            q->setUpdatesEnabled(false);
        }
        initGLViewport();
        if (MGraphicsSystemHelper::isRunningNativeGraphicsSystem()) {
            QTimer::singleShot(700, q, SLOT(_q_enablePaintUpdates()));
        }
    }
#endif
}

void MWindowPrivate::handleCloseEvent(QCloseEvent *event)
{
    Q_Q(MWindow);

    // Call close event manually here, because we want to check if the
    // event got ignored before executing lazy shutdown routines.
    q->closeEvent(static_cast<QCloseEvent *>(event));

    if (!event->isAccepted()) {
        return;
    }

    isLogicallyClosed = true;

    MOnDisplayChangeEvent ev(false, QRectF(QPointF(0, 0), q->visibleSceneSize()));
    q->onDisplayChangeEvent(&ev);

    doSwitcherExited();

    if (MApplication::prestartMode() == M::LazyShutdownMultiWindow ||
        MApplication::prestartMode() == M::LazyShutdown) {

        // Check if all windows are closed. If so,
        // return to the prestarted state.
        bool allWindowsLogicallyClosed = true;
        Q_FOREACH(MWindow * win, MApplication::windows()) {
            if (!win->d_ptr->isLogicallyClosed) {
                allWindowsLogicallyClosed = false;
            }
        }

        if (allWindowsLogicallyClosed) {
            MApplication::setPrestarted(true);
        }

        if (!q->closeOnLazyShutdown()) {
            q->hide();
            event->ignore();
            return;
        }
    }

#ifdef Q_WS_X11
    if (q->testAttribute(Qt::WA_QuitOnClose) &&
        q->windowState().testFlag(Qt::WindowNoState) &&
        !event->spontaneous())
    {
        sendNetCloseWindow();
    }
#endif
}

MWindow::MWindow(MWindowPrivate &dd, QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(&dd)
{
    Q_D(MWindow);

    d->q_ptr = this;
    d->init();
    MComponentData::registerWindow(this);
}

MWindow::MWindow(MWindowPrivate &dd, MScene *scene, QWidget *parent)
    : QGraphicsView(parent), d_ptr(&dd)
{
    Q_D(MWindow);
    d->q_ptr = this;
    d->init();
    MComponentData::registerWindow(this);
    setSceneManager(new MSceneManager(scene, this));
}

MWindow::MWindow(MWindowPrivate &dd, MSceneManager *sceneManager, QWidget *parent)
    : QGraphicsView(parent), d_ptr(&dd)
{
    Q_D(MWindow);
    d->q_ptr = this;
    d->init();
    MComponentData::registerWindow(this);
    setSceneManager(sceneManager);
}

MWindow::MWindow(MSceneManager *sceneManager, QWidget *parent)
    : QGraphicsView(parent), d_ptr(new MWindowPrivate)
{
    Q_D(MWindow);
    d->q_ptr = this;
    d->init();
    MComponentData::registerWindow(this);
    setSceneManager(sceneManager);
}

MWindow::MWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(new MWindowPrivate)
{
    Q_D(MWindow);
    d->q_ptr = this;
    d->init();
    MComponentData::registerWindow(this);
}

MWindow::~MWindow()
{
#ifdef Q_WS_X11
    MOrientationTracker::instance()->d_func()->stopFollowingCurrentAppWindow(this, true);
    MOrientationTracker::instance()->d_func()->stopFollowingCurrentAppWindow(this, false);
#endif
    MComponentData::unregisterWindow(this);
    delete d_ptr;
}

void MWindow::setTranslucentBackground(bool enable)
{
    Q_D(MWindow);

    if (enable) {
        setAttribute(Qt::WA_TranslucentBackground);
#ifdef Q_WS_X11
        // This is workaround for NB#170883
        // Setting Qt::WA_TranslucentBackground property for window
        // changes mask for yet unknown reason, only on hardware,
        // not in scratchbox and supposingly its a candidate for
        // filing bug against Qt when confirmed
        d->appendVisibilityChangeMask();
        if (MApplication::softwareRendering() || MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()) {
            d->disableAutomaticBackgroundRepainting();
        }
#endif
    }

    // when the gl widget is not initialized yet we will also not initialize it
    if (MApplication::softwareRendering() 
        || MApplication::isPrestarted() 
#ifndef Q_OS_WIN
        || MComponentCache::populating()
#endif
        || (MGraphicsSystemHelper::isRunningNativeGraphicsSystem() && !dynamic_cast<QGLWidget*>(viewport()))) {
        d->initSoftwareViewport();
    } else {
        d->initGLViewport();
    }

    if (MApplication::softwareRendering())
        viewport()->setAutoFillBackground(!enable);
}

#ifdef Q_WS_X11
void MWindowPrivate::setX11Property(const char *propertyName, qreal value)
{
    Q_Q(MWindow);

    Atom atom = XInternAtom(QX11Info::display(), propertyName, False);

    if (value < 0.0 || value >= 1.0) {
        XDeleteProperty(QX11Info::display(), q->effectiveWinId(), atom);
    } else {
        // We use same conventions as _NET_WM_WINDOW_OPACITY so we could re-use
        // same code in the compositor
        unsigned int opacity = (unsigned int) (0xffffffff * value);

        XChangeProperty(QX11Info::display(), q->effectiveWinId(), atom, XA_CARDINAL, 32 ,
                        PropModeReplace, (unsigned char *) &opacity, 1);
    }
}

qreal MWindowPrivate::getX11Property(const char *propertyName) const
{
    Q_Q(const MWindow);

    qreal level = 1.0;
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytes = 0;

    union {
        unsigned char* asUChar;
        unsigned long* asULong;
    } data = {0};

    Atom propertyAtom = XInternAtom(QX11Info::display(), propertyName, False);

    int status = XGetWindowProperty(QX11Info::display(), q->effectiveWinId(), propertyAtom,
                                    0, 1, False, AnyPropertyType,
                                    &actualType, &actualFormat, &nitems,
                                    &bytes, &data.asUChar);

    if (status == Success && actualType == XA_CARDINAL && actualFormat == 32 && nitems == 1)
        level = (qreal)data.asULong[0] / 0xffffffff;
    if (status == Success)
        XFree(data.asUChar);
    return level;
}

void MWindowPrivate::setX11PrestartProperty(bool set)
{
    Q_Q(MWindow);
    Display *dpy  = QX11Info::display();
    if (dpy) {
        Atom prestartAtom = XInternAtom(dpy, "_MEEGOTOUCH_PRESTARTED", False);
        unsigned char data=1;
        if (set) {
            XChangeProperty(dpy, q->effectiveWinId(), prestartAtom,
                            XA_CARDINAL, 8, PropModeAppend, &data, 1);
        } else {
            XDeleteProperty(dpy, q->effectiveWinId(), prestartAtom);
        }
    }
}

void MWindowPrivate::setX11OrientationAngleProperty(M::OrientationAngle angle)
{
    Display *display = QX11Info::display();

    if (!display)
        return;

    //sometimes this class is used without valid x11 window
    if (robustEffectiveWinId() == 0) {
        return;
    }

    Atom orientationAngleAtom = XInternAtom(display, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);

    XChangeProperty(display, robustEffectiveWinId(), orientationAngleAtom, XA_CARDINAL, 32,
                    PropModeReplace, (unsigned char*)&angle, 1);
}

void MWindowPrivate::setX11NotificationPreviewsDisabledProperty(bool set)
{
    Q_Q(MWindow);
    Display *dpy  = QX11Info::display();
    if (dpy) {
        Atom atom  = XInternAtom(dpy, "_MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED", False);
        unsigned char data=1;
        if (set) {
            XChangeProperty(dpy, q->effectiveWinId(), atom,
                            XA_INTEGER, 8, PropModeReplace, &data, 1);
        } else {
            XDeleteProperty(dpy, q->effectiveWinId(), atom);
        }
    }
}

bool MWindowPrivate::getX11NotificationPreviewsDisabledProperty() const
{
    Q_Q(const MWindow);

    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytes = 0;

    union {
        unsigned char* asUChar;
        unsigned long* asULong;
    } data = {0};

    bool previewsDisabled = false;

    Display *dpy  = QX11Info::display();

    if (dpy) {
        Atom atom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_NOTIFICATION_PREVIEWS_DISABLED", False);

        int status = XGetWindowProperty(dpy, q->effectiveWinId(), atom,
                                        0L, 1L, False, XA_INTEGER,
                                        &actualType, &actualFormat, &nitems, &bytes, &data.asUChar);
        if (status == Success && nitems) {
            previewsDisabled = data.asULong != 0;
            XFree(data.asUChar);
        }
    }

    return previewsDisabled;
}

void MWindowPrivate::setMeegoX11Properties()
{
    setX11OrientationAngleProperty(angle);
    if (sceneManager) {
        sceneManager->d_func()->updateStatusBarGeometryProperty();
    }
    setX11PrestartProperty(MApplication::isPrestarted());
    setX11NotificationPreviewsDisabledProperty(notificationPreviewDisabled);
}

void MWindowPrivate::sendNetCloseWindow()
{
    Q_Q(const MWindow);

    XEvent ev;
    Display *dpy = QX11Info::display();
    memset(&ev, 0, sizeof(ev));

    ev.xclient.type         = ClientMessage;
    ev.xclient.display      = dpy;
    ev.xclient.window       = q->effectiveWinId();
    ev.xclient.message_type = XInternAtom(dpy, "_NET_CLOSE_WINDOW", False);
    ev.xclient.format       = 32;
    ev.xclient.data.l[0]    = CurrentTime;
    ev.xclient.data.l[1]    = QX11Info::appRootWindow();

    XSendEvent(dpy, QX11Info::appRootWindow(), False, SubstructureRedirectMask | SubstructureNotifyMask,
               &ev);

    XSync(dpy, False);
}
#endif

void MWindow::setGlobalAlpha(qreal level)
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    d->setX11Property("_MEEGOTOUCH_GLOBAL_ALPHA", level);
#else
    Q_UNUSED(level);
#endif
}

qreal MWindow::globalAlpha()
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    return d->getX11Property("_MEEGOTOUCH_GLOBAL_ALPHA");
#else
    return 1.0;
#endif
}

void MWindow::setVideoGlobalAlpha(qreal level)
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    d->setX11Property("_MEEGOTOUCH_VIDEO_ALPHA", level);
#else
    Q_UNUSED(level);
#endif
}

qreal MWindow::videoGlobalAlpha()
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    return d->getX11Property("_MEEGOTOUCH_VIDEO_ALPHA");
#else
    return 1.0;
#endif
}

MScene *MWindow::scene()
{
    return qobject_cast<MScene *>(QGraphicsView::scene());
}

bool MWindow::isOrientationAngleLocked() const
{
    Q_D(const MWindow);

    return d->orientationAngleLocked;
}

void MWindow::setOrientationAngleLocked(bool locked)
{
    Q_D(MWindow);

    if (d->orientationAngleLocked != locked) {
        d->orientationAngleLocked = locked;

        // update from the orientation tracker if we're unlocking orientation changes
        if (!locked)
            setOrientationAngle(MOrientationTracker::instance()->orientationAngle());
    }
}

bool MWindow::isOrientationLocked() const
{
    Q_D(const MWindow);

    return d->orientationLocked;
}

void MWindow::setOrientationLocked(bool locked)
{
    Q_D(MWindow);

    if (d->orientationLocked != locked) {
        d->orientationLocked = locked;

        // update from the orientation tracker if we're unlocking orientation changes
        if (!locked)
            setOrientationAngle(MOrientationTracker::instance()->orientationAngle());
    }
}

void MWindow::lockOrientationAngle()
{
    setOrientationAngleLocked(true);
}

void MWindow::unlockOrientationAngle()
{
    setOrientationAngleLocked(false);
}

void MWindow::lockOrientation()
{
    setOrientationLocked(true);
}

void MWindow::unlockOrientation()
{
    setOrientationLocked(false);
}

void MWindow::setSceneManager(MSceneManager *sceneManager)
{
    Q_D(MWindow);

    if (d->sceneManager == sceneManager) {
        return;
    }

    if (sceneManager && sceneManager->orientationAngle() != orientationAngle()) {
        sceneManager->setOrientationAngle(orientationAngle(), MSceneManager::ImmediateTransition);
    }

    if (d->sceneManager) {
        delete d->sceneManager;
    }

    d->sceneManager = sceneManager;

    if (sceneManager) {
        connect(sceneManager, SIGNAL(orientationAngleChanged(M::OrientationAngle)),
                SIGNAL(orientationAngleChanged(M::OrientationAngle)));
        connect(sceneManager, SIGNAL(orientationChanged(M::Orientation)),
                SIGNAL(orientationChanged(M::Orientation)));
        connect(sceneManager, SIGNAL(orientationChangeFinished(M::Orientation)),
                SIGNAL(orientationChangeFinished(M::Orientation)));
        sceneManager->setParent(this);
        setScene(sceneManager->scene());
        // make scene rect match the display's native orientation
        setSceneRect(QRectF(QPointF(), visibleSceneSize(MDeviceProfile::instance()->orientationFromAngle(M::Angle0))));
        centerOn(sceneRect().center());
    }
}

MSceneManager *MWindow::sceneManager()
{
    Q_D(MWindow);

    // A scene manager is needed. Let's create one on the fly
    // if we don't have one already.
    if (!d->sceneManager) {
        setSceneManager(new MSceneManager(scene()));
    }

    return d->sceneManager;
}

MSceneManager *MWindow::sceneManager() const
{
    Q_D(const MWindow);

    return d->sceneManager;
}

M::Orientation MWindow::orientation() const
{
    Q_D(const MWindow);

    if (d->sceneManager) {
        return d->sceneManager->orientation();
    } else {
        return MDeviceProfile::instance()->orientationFromAngle(d->angle);
    }
}

M::OrientationAngle MWindow::orientationAngle() const
{
    Q_D(const MWindow);

    if (d->sceneManager) {
        return d->sceneManager->orientationAngle();
    } else {
        return d->angle;
    }
}

void MWindow::setOrientationAngle(M::OrientationAngle angle)
{
    Q_D(MWindow);

    //orientation was forced by command line option
    if (MComponentData::isOrientationForced())
        return;

    M::OrientationAngle targetAngle;
    if (d->sceneManager && d->sceneManager->d_ptr->pendingRotation)
        targetAngle = d->sceneManager->d_ptr->pendingRotation->angle;
    else
        targetAngle = orientationAngle();

    if (targetAngle != angle) {
        //reset invisible paint counter
        //because orientation change will require repaint
        d->invisiblePaintCounter = 0;

        d->oldOrientation = orientation();
        d->angle = angle;

        if (d->sceneManager) {
            d->sceneManager->setOrientationAngle(angle,
                                                 d->orientationChangeTransitionMode());
        } else {
            // first notify widgets, then emit the signal (in case someone
            // would like to connect to the signal and get correct size hints for widgets)
            d->notifyWidgetsAboutOrientationChange();
            emit orientationAngleChanged(angle);
        }
#ifdef Q_WS_X11
        d->setX11OrientationAngleProperty(angle);
#endif
    }
}

void MWindow::setLandscapeOrientation()
{
    if (orientation() == M::Landscape) {
        return;
    }

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
        // native display orientation is landscape
        setOrientationAngle(M::Angle0);
    } else {
        // native display orientation is portrait
        setOrientationAngle(M::Angle270);
    }
}

void MWindow::setPortraitOrientation()
{
    if (orientation() == M::Portrait) {
        return;
    }

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Portrait) {
        // native display orientation is portrait
        setOrientationAngle(M::Angle0);
    } else {
        // native display orientation is landscape
        setOrientationAngle(M::Angle270);
    }
}

QSize MWindow::visibleSceneSize(M::Orientation orientation) const
{
    QSize s;

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
        // native display orientation is landscape
        if (orientation == M::Landscape)
            s = QSize(MDeviceProfile::instance()->resolution().width(),
                      MDeviceProfile::instance()->resolution().height());
        else
            s = QSize(MDeviceProfile::instance()->resolution().height(),
                      MDeviceProfile::instance()->resolution().width());
    } else {
        // native display orientation is portrait
        if (orientation == M::Landscape)
            s = QSize(MDeviceProfile::instance()->resolution().height(),
                      MDeviceProfile::instance()->resolution().width());
        else
            s = QSize(MDeviceProfile::instance()->resolution().width(),
                      MDeviceProfile::instance()->resolution().height());
    }

    return s;
}

QSize MWindow::visibleSceneSize() const
{
    return visibleSceneSize(orientation());
}

bool MWindow::isOnDisplay() const
{
    Q_D(const MWindow);

    return d->onDisplay;
}

void MWindow::enterDisplayEvent()
{}

void MWindow::exitDisplayEvent()
{}

#ifdef Q_WS_X11
void MWindowPrivate::sendDelayedExitDisplayEvent()
{
    Q_Q(MWindow);
    delete delayedMOnDisplayChangeEvent;
    delayedMOnDisplayChangeEvent = new MOnDisplayChangeEvent(MOnDisplayChangeEvent::FullyOffDisplay, q->sceneRect());
    displayExitedTimer.start();
}
#endif

void MWindow::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    Q_D(MWindow);
    d->displayExitedTimer.stop();

    switch (event->state()) {

    case MOnDisplayChangeEvent::FullyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {
            d->doEnterDisplayEvent();
            if (scene()) {
                d->propagateMOnDisplayChangeEventToScene(event);
            }
        }
        break;

    case MOnDisplayChangeEvent::FullyOffDisplay:
        if (d->onDisplay || !d->onDisplaySet) {
            d->doExitDisplayEvent();
            if (scene()) {
                d->propagateMOnDisplayChangeEventToScene(event);
            }
        }
        break;

    default:
        event->ignore();
        break;
    }
}

void MWindow::paintEvent(QPaintEvent *event)
{
    Q_D(MWindow);
    if (d->beforeFirstPaintEvent) {
        d->beforeFirstPaintEvent = false;
        d->disableAutomaticBackgroundRepainting();
    }

    // FIXME: disabled for the meego graphicssystem right now until we have a solution for NB#205680
    if (!isOnDisplay() && !MGraphicsSystemHelper::isRunningMeeGoGraphicsSystem()) {
        // we allow some paint events when we are not visible as we might have a race between
        // the visibility information and the paint events
        if (d->invisiblePaintCounter < d->allowedPaintEventsWhenInvisible) {
            mDebug("MWindow::paintEvent") << "Application is not visible. Paint event allowed nevertheless.";
            ++d->invisiblePaintCounter;
        } else {
            mWarning("MWindow::paintEvent") << "Application is not visible. Paint event discarded. Make sure the application does not paint in the first place.";
            event->accept();
            d->discardedPaintEvent = true;
            return;
        }
    }

    if (isInSwitcher()) {
        if (!d->timeSinceLastPaintInSwitcher.isValid()) {
            d->timeSinceLastPaintInSwitcher.start();
            d->updateIsPending = false;
        } else {
            const int maxFpsInSwitcher = 5;
            const int minDelay = 1000. / maxFpsInSwitcher;
            qint64 msSinceLastPaint = d->timeSinceLastPaintInSwitcher.elapsed();
            if (msSinceLastPaint < minDelay) {
                event->accept();
                if (!d->updateIsPending) {
                    // trigger a new paint event as otherwise the screen may not be up to date
                    QTimer::singleShot(minDelay, viewport(), SLOT(update()));
                    d->updateIsPending = true;
                }
                return;
            } else {
                d->timeSinceLastPaintInSwitcher.restart();
                d->updateIsPending = false;
            }
        }
    }

    QGraphicsView::paintEvent(event);
}

bool MWindow::event(QEvent *event)
{
    Q_D(MWindow);

    if ((event->type() == QEvent::Show && !isMinimized()) || event->type() == QEvent::WindowActivate) {
        MComponentData::setActiveWindow(this);
    } else if (event->type() == QEvent::Show || event->type() == QEvent::Hide) {
#ifdef Q_WS_X11
        d->resolveOrientationRules();
#endif
    } else if (event->type() == QEvent::WindowStateChange) {
        d->handleWindowStateChangeEvent(static_cast<QWindowStateChangeEvent *>(event));
    } else if (event->type() == QEvent::Close) {
        d->handleCloseEvent(static_cast<QCloseEvent *>(event));
        // closeEvent() already called.
        return true;
    } else if (QEvent::KeyPress == event->type()) {
        bool updateNeeded = false;

        //SIMULATION OF ROTATION FOR DEVELOPMENT PURPOSES
        QKeyEvent *k = static_cast<QKeyEvent *>(event);
        if (Qt::Key_R == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            d->rotateWindowsFromKeyEvent(k);
        } else if (Qt::Key_T == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowPosition(!MApplication::showPosition());
            updateNeeded = true;
        } else if (Qt::Key_S == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowSize(!MApplication::showSize());
            updateNeeded = true;
        } else if (Qt::Key_B == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            if (!MApplication::showBoundingRect()) {
                foreach (QGraphicsItem *item, scene()->items()) {
                    QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget*>(item);
                    if (widget && !widget->parentWidget())
                        MDebug::printDebugChildInformation(widget);
                }
            }
            MApplication::setShowBoundingRect(!MApplication::showBoundingRect());
            updateNeeded = true;
        } else if (Qt::Key_M == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowMargins(!MApplication::showMargins());
            updateNeeded = true;
        } else if (Qt::Key_N == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowObjectNames(!MApplication::showObjectNames());
            MApplication::setShowStyleNames(!MApplication::showStyleNames());
            updateNeeded = true;
        } else if (Qt::Key_F == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowFps(!MApplication::showFps());
            updateNeeded = true;
        } else if (Qt::Key_D == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            Qt::LayoutDirection dir = MApplication::layoutDirection();

            if (dir == Qt::LeftToRight)
                dir = Qt::RightToLeft;
            else
                dir = Qt::LeftToRight;

            MApplication::setLayoutDirection(dir);

            updateNeeded = true;
        } else if (Qt::Key_L == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            // switch language
            QString language;

            MLocale oldLocale; // get current system default
            language = oldLocale.name();

            if (language == "en_US_POSIX" || language.isEmpty())
                language = "fi";
            else if (language == "fi")
                language = "en";
            else if (language == "en")
                language = "de";
            else if (language == "de")
                language = "ar";
            else if (language == "ar")
                language = "hu";
            else if (language == "hu")
                language = "ur";
            else if (language == "ur")
                language = "zh_CN";
            else
                // engineering English:
                language = "";

            MLocale newLocale(language);
            MLocale::setDefault(newLocale);

            updateNeeded = true;
        } else if (Qt::Key_P == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            d->takeScreenshot();
        } else if (Qt::Key_Q == k->key() && (k->modifiers() & Qt::ControlModifier)) {
            foreach(MWindow* window, MApplication::windows())
                window->close();
        }

        if (updateNeeded) {
            this->viewport()->update();
        }
    } else if (event->type() == QEvent::ApplicationLayoutDirectionChange) {
        // tell the scene and its items about the layout change
        if (scene()) {
            QList<QGraphicsItem *> items = scene()->items();

            // call setLayoutDirection_helper() for all top-level items
            for (int i = 0; i < items.size(); i++) {
                QGraphicsItem *item = items.at(i);
                if (scene()->items().contains(item) && !item->parentItem()) {
                    d->handleApplicationLayoutDirectionChangeEvent(item);
                }
            }
        }
        return true;
    } else if (event->type() == QEvent::LanguageChange) {
        // Tell the scene and its top-level items about the language change
        if (scene()) {
            QList<QGraphicsItem *> items = scene()->items();
            // Call handler for language change event only for top
            // level widgets. The handler then recurses over the
            // children.
            QList<QGraphicsItem *> itemsWithoutParents;
            foreach(QGraphicsItem *item, items)
                if(!item->parentItem()) itemsWithoutParents << item;
            foreach(QGraphicsItem * item, itemsWithoutParents) {
                d->handleLanguageChangeEvent(item);
            }
        }
        return true;
    } else if (event->type() == MOnDisplayChangeEvent::eventNumber()) {
        onDisplayChangeEvent(static_cast<MOnDisplayChangeEvent *>(event));
        return true;
    }
#ifdef Q_WS_X11
    else if (event->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent* dynamicEvent = static_cast<QDynamicPropertyChangeEvent*>(event);        
        if (dynamicEvent->propertyName() == FollowsCurrentApplicationWindowOrientationPropertyName) {
            //property was set, does not matter what value
            if (property(FollowsCurrentApplicationWindowOrientationPropertyName).isValid()) {
                mDebug("MWindow") << "window follows current app window orientation";
                MOrientationTracker::instance()->d_func()->startFollowingCurrentAppWindow(this);
            }
            //propery was unset
            else {
                MOrientationTracker::instance()->d_func()->stopFollowingCurrentAppWindow(this);
                d->resolveOrientationRules();
            }
        }
    }
    else if (event->type() == QEvent::WinIdChange) {
        d->setMeegoX11Properties();
    }
#endif
    return QGraphicsView::event(event);
}

void MWindow::setVisible(bool visible)
{
    Q_D(MWindow);

    if (visible) {
        // This effectively overrides call to show() when in
        // prestarted state.
        if (MApplication::isPrestarted()) {
            return;
        }

        // Ensure that window is in a proper orientation angle before
        // it gets displayed on screen.
        d->ensureOrientationAngleIsUpToDateBeforeShowing();

        // Set onDisplay if it's not already set, because
        // it is used to discard paint events and we don't have
        // time to wait for visibility notifies from compositor.
        if (!d->onDisplaySet) {
            d->onDisplay = true;
        }

        if (MTheme::hasPendingRequests()) {
            // The showing of the window gets delayed until the theme
            // has finished to load all pixmap requests. This prevents
            // a flickering of the application on startup and improves
            // the performance.
            connect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()),
                    this, SLOT(_q_onPixmapRequestsFinished()));
            return;
        } else {
            if (!windowState().testFlag(Qt::WindowMinimized) && !MApplication::softwareRendering()) {
                if (MGraphicsSystemHelper::isRunningNativeGraphicsSystem()) {
                    if (!dynamic_cast<QGLWidget*>(viewport())) {
                        d->initGLViewport();
                    }
                } else {
                    d->initGLViewport();
                }
            }
            d->isLogicallyClosed = false;
        }

    }

    // Resets invisible paint event counter, as a side effect:
    MOnDisplayChangeEvent ev(visible, sceneRect());
    onDisplayChangeEvent(&ev);

    QGraphicsView::setVisible(visible);
}

void MWindow::setCloseOnLazyShutdown(bool enable)
{
    Q_D(MWindow);

    d->closeOnLazyShutdown = enable;
}

bool MWindow::closeOnLazyShutdown() const
{
    Q_D(const MWindow);

    return d->closeOnLazyShutdown;
}

void MWindow::prestartedInit()
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    if (MApplication::softwareRendering() || MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()) {
        d->applyStartupWindowBackground();
    }
#endif //Q_WS_X11
}

int MWindow::orientationChangeTransitionMode()
{
    Q_D(MWindow);
    return d->orientationChangeTransitionMode();
}

void MWindow::setNotificationPreviewsVisible(bool visible)
{
#ifdef Q_WS_X11
    Q_D(MWindow);
    d->notificationPreviewDisabled = !visible;
    d->setX11NotificationPreviewsDisabledProperty(!visible);
#else
    Q_UNUSED(visible);
#endif
}

bool MWindow::notificationPreviewsVisible() const
{
#ifdef Q_WS_X11
    Q_D(const MWindow);
    return !d->notificationPreviewDisabled;
#else
    return false;
#endif
}

void MWindow::enableNotificationPreviews()
{
    setNotificationPreviewsVisible(true);
}

void MWindow::disableNotificationPreviews()
{
    setNotificationPreviewsVisible(false);
}

bool MWindow::isRoundedCornersEnabled() const
{
    Q_D(const MWindow);

    return d->borderDecorator && d->borderDecorator->isDecorating();
}

void MWindow::setRoundedCornersEnabled(bool enabled)
{
    Q_D(MWindow);

    if (enabled == isRoundedCornersEnabled())
        return;

    if (enabled) {
        if (!d->borderDecorator)
            d->borderDecorator = new MWindowBorderDecorator;

        d->borderDecorator->decorate(sceneManager());
    } else {
        if (d->borderDecorator)
            d->borderDecorator->removeDecorations();
    }
}

#ifndef UNIT_TEST
#include "moc_mwindow.cpp"
#endif
