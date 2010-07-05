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
#include "mgles2renderer.h"
#endif

#include "mwindow.h"
#include "mwindow_p.h"

#include "mscene.h"
#include "mscene_p.h"
#include "mapplication.h"
#include "mapplication_p.h"
#include "mapplicationwindow.h"
#include "mcomponentcache.h"
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

#include <QPropertyAnimation>
#include <QSettings>
#include <QDir>

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xatom.h>
# include <X11/Xlib.h>
// Avoid conflict with QEvent::KeyPress usage in MWindow::Event
# undef KeyPress
#endif

namespace {
    const QString ImagesPath(QDir::homePath() + "/MyDocs/.images");
}

/// Actual class

MWindowPrivate::MWindowPrivate() :
    glWidget(0),
    sceneManager(0),
    oldOrientation(M::Landscape), // the initial value is not used at all
    orientationAngleLocked(false),
    orientationLocked(false),
    isLogicallyClosed(true),
    closeOnLazyShutdown(false),
    onDisplay(false),
    onDisplaySet(false),
    q_ptr(NULL)
{
#ifndef Q_WS_X11
    onDisplay = true; // For non X11, windows always visible
#endif

    MWindow *window = MApplication::activeWindow();

    if (window)
        angle = window->orientationAngle();
    else
        angle = MOrientationTracker::instance()->orientationAngle();
}

MWindowPrivate::~MWindowPrivate()
{
}

void MWindowPrivate::init()
{
    Q_Q(MWindow);

#ifdef Q_WS_X11
    // We do window decorations ourselves. Set env variable accordingly for
    // development purposes

#ifdef HAVE_N900
    if ( !MApplication::fullScreen() ) {
#endif

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

#ifdef HAVE_N900
    }
#endif

#endif // Q_WS_X11

    // resize always to the size in landscape mode,
    // since it's not the window but the scene content that is rotated
    q->resize(q->visibleSceneSize(M::Landscape));
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
    }
#endif

    q->setTranslucentBackground(false);

    if (MApplication::fullScreen())
        q->showFullScreen();
}

void MWindowPrivate::initSoftwareViewport()
{
    Q_Q(MWindow);

    mWarning("MWindow") << "Switching to software rendering";

#ifdef M_USE_OPENGL
    MGLES2Renderer::activate(NULL);
    MGLES2Renderer::destroy(glWidget);
    glWidget = NULL;
#endif

    q->setViewport(new QWidget());
    q->setViewportUpdateMode(MWindow::MinimalViewportUpdate);

    configureViewport();
}

void MWindowPrivate::initGLViewport()
{
#ifdef QT_OPENGL_LIB
    Q_Q(MWindow);

    mWarning("MWindow") << "Window restored, switching to GL rendering";

    bool translucent = q->testAttribute(Qt::WA_TranslucentBackground);

    // The sequence of calls here is important. When translucency is not
    // enabled, ensure setViewport() is called before DuiGLES2Renderer
    // initializes its vertices, otherwise call setViewport() after
    // DuiGLES2Renderer initializes itself. Failure to do this will cause
    // a crash.
    // This QGLWidget is owned by the viewport so previous one
    // actually gets deleted if we overwrite it with a new one
    if (translucent) {
        QGLFormat fmt;
        // disable multisampling, is enabled by default in Qt
        fmt.setSampleBuffers(false);
        fmt.setSamples(0);
        fmt.setAlpha(true); // Workaround for NB#153625

        glWidget = MComponentCache::glWidget(fmt);
        QPalette palette;
        palette.setColor(QPalette::Base, Qt::transparent);
        glWidget->setAutoFillBackground(true);
        glWidget->setPalette(palette);
    } else {
        glWidget = MComponentCache::glWidget();
        q->setViewport(glWidget);
    }
#ifdef M_USE_OPENGL
    MGLES2Renderer::instance(glWidget);
    MGLES2Renderer::activate(glWidget);
#endif
    if (translucent)
        q->setViewport(glWidget);
#endif

    q->setViewportUpdateMode(MWindow::FullViewportUpdate);

    configureViewport();
}

void MWindowPrivate::configureViewport()
{
    Q_Q(MWindow);

    q->viewport()->grabGesture(Qt::TapAndHoldGesture);
    q->viewport()->grabGesture(Qt::PinchGesture);
    q->viewport()->grabGesture(Qt::PanGesture);
    q->viewport()->grabGesture(Qt::SwipeGesture);
    q->viewport()->grabGesture(Qt::TapGesture);

    q->setAttribute(Qt::WA_AcceptTouchEvents);
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

#ifdef Q_WS_X11
void MWindowPrivate::appendVisibilityChangeMask()
{
    Q_Q(MWindow);

    XWindowAttributes existingAttributes;
    XSetWindowAttributes newAttributes;
    Status status;

    status = XGetWindowAttributes(QX11Info::display(), q->winId(), &existingAttributes);
    if (status == 0) {
        qFatal("MWindow: XGetWindowAttributes() failed!");
    }

    newAttributes.event_mask = existingAttributes.your_event_mask | VisibilityChangeMask;

    XChangeWindowAttributes(QX11Info::display(), q->winId(), CWEventMask, &newAttributes);
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

M::Orientation MWindowPrivate::orientation(M::OrientationAngle angle) const
{
    return (angle == M::Angle0 || angle == M::Angle180) ? M::Landscape : M::Portrait;
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
        if (graphicsScene) {
            MOrientationChangeEvent event(newOrientation);
            foreach(QGraphicsItem * item, graphicsScene->items())
                graphicsScene->sendEvent(item, &event);
        }

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
}

void MWindowPrivate::doExitDisplayEvent()
{
    Q_Q(MWindow);

    onDisplay = false;
    onDisplaySet = true;

    q->exitDisplayEvent();
    emit q->displayExited();
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

#ifdef Q_WS_X11
void MWindowPrivate::setX11PrestartProperty(bool set)
{
    Q_Q(MWindow);
    Display *dpy  = QX11Info::display();
    if (dpy) {
        Atom prestartAtom = XInternAtom(dpy, "_MEEGOTOUCH_PRESTARTED", False);
        unsigned char data=1;
        if (set) {
            XChangeProperty(dpy, q->winId(), prestartAtom, 
                            XA_CARDINAL, 8, PropModeAppend, &data, 1);
        } else {
            XDeleteProperty(dpy, q->winId(), prestartAtom);
        }
    }
}
#endif

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
#endif
    }

    if (MApplication::softwareRendering() || MApplication::isPrestarted())
        d->initSoftwareViewport();
    else
        d->initGLViewport();
}

#ifdef Q_WS_X11
void MWindowPrivate::setX11Property(const char *propertyName, qreal value)
{
    Q_Q(MWindow);

    Atom atom = XInternAtom(QX11Info::display(), propertyName, False);

    if (value < 0.0 || value >= 1.0) {
        XDeleteProperty(QX11Info::display(), q->winId(), atom);
    } else {
        // We use same conventions as _NET_WM_WINDOW_OPACITY so we could re-use
        // same code in the compositor
        unsigned int opacity = (unsigned int) (0xffffffff * value);

        XChangeProperty(QX11Info::display(), q->winId(), atom, XA_CARDINAL, 32 ,
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

    int status = XGetWindowProperty(QX11Info::display(), q->winId(), propertyAtom,
                                    0, 1, False, AnyPropertyType,
                                    &actualType, &actualFormat, &nitems,
                                    &bytes, &data.asUChar);

    if (status == Success && actualType == XA_CARDINAL && actualFormat == 32 && nitems == 1)
        level = (qreal)data.asULong[0] / 0xffffffff;
    if (status == Success)
        XFree(data.asUChar);
    return level;
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
        setSceneRect(QRectF(QPointF(), visibleSceneSize(M::Landscape)));
        centerOn(sceneRect().center());
    }
}

MSceneManager *MWindow::sceneManager()
{
    Q_D(MWindow);

    // A scene manager is needed. Let's create one on the fly
    // if we don't have one already.
    if (!d->sceneManager) {
        setSceneManager(new MSceneManager);
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
        return d->orientation(d->angle);
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

    if (orientationAngle() != angle) {
        d->oldOrientation = orientation();
        d->angle = angle;

        if (d->sceneManager) {
            MSceneManager::TransitionMode mode = isVisible() ?
                MSceneManager::AnimatedTransition :
                MSceneManager::ImmediateTransition;

            d->sceneManager->setOrientationAngle(angle, mode);
        } else {
            // first notify widgets, then emit the signal (in case someone
            // would like to connect to the signal and get correct size hints for widgets)
            d->notifyWidgetsAboutOrientationChange();
            emit orientationAngleChanged(angle);
        }
    }
}

void MWindow::setLandscapeOrientation()
{
    if (orientation() != M::Landscape)
        setOrientationAngle(M::Angle0);
}

void MWindow::setPortraitOrientation()
{
    if (orientation() != M::Portrait)
        setOrientationAngle(M::Angle270);
}

QSize MWindow::visibleSceneSize(M::Orientation orientation) const
{
    QSize s;

    if (orientation == M::Landscape) {
        s = MDeviceProfile::instance()->resolution();
    } else {
        s = QSize(MDeviceProfile::instance()->resolution().height(),
                  MDeviceProfile::instance()->resolution().width());
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

void MWindow::onDisplayChangeEvent(MOnDisplayChangeEvent *event)
{
    Q_D(MWindow);

    switch (event->state()) {

    case MOnDisplayChangeEvent::FullyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {
            d->doEnterDisplayEvent();
        }
        break;

    case MOnDisplayChangeEvent::FullyOffDisplay:
        if (d->onDisplay || !d->onDisplaySet) {
            d->doExitDisplayEvent();
        }
        break;

    default:
        event->ignore();
        break;
    }

    if (scene()) {
        d->propagateMOnDisplayChangeEventToScene(event);
    }
}

void MWindow::paintEvent(QPaintEvent *event)
{
#ifdef M_USE_OPENGL
    Q_D(MWindow);

    if (!MApplication::softwareRendering()) {
        MGLES2Renderer::activate(d->glWidget);
    }
#endif // M_USE_OPENGL

    QGraphicsView::paintEvent(event);
}

bool MWindow::event(QEvent *event)
{
    Q_D(MWindow);

    if (event->type() == QEvent::Show || event->type() == QEvent::WindowActivate) {
        MComponentData::setActiveWindow(this);
    }

    if (event->type() == QEvent::Close) {

        // Call close event manually here, because we want to check if the
        // event got ignored before executing lazy shutdown routines.
        closeEvent(static_cast<QCloseEvent *>(event));

        if (!event->isAccepted()) {
            return true;
        }

        d->isLogicallyClosed = true;

        if (MApplication::prestartMode() == M::LazyShutdownMultiWindow ||
            MApplication::prestartMode() == M::LazyShutdown) {

#ifdef Q_WS_X11
            MApplicationPrivate::removeWindowFromSwitcher(winId(), true);
#endif

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

            if (!closeOnLazyShutdown()) {
                hide();
                lower();
                event->ignore();
                return true;
            }
        }

#ifdef M_USE_OPENGL
        if (!MApplication::softwareRendering()) {
            MGLES2Renderer::destroy(d->glWidget);
        }
#endif

        // closeEvent() already called.
        return true;
    }

    if (QEvent::KeyPress == event->type()) {
        bool updateNeeded = false;

        //SIMULATION OF ROTATION FOR DEVELOPMENT PURPOSES
        QKeyEvent *k = static_cast<QKeyEvent *>(event);
        if (Qt::Key_R == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            foreach (MWindow *window, MApplication::windows()) {
                int newAngle = (window->orientationAngle()
                                + ((k->modifiers() & Qt::AltModifier) ? 270 : 90)) % 360;
                if (!window->isOrientationAngleLocked()) {
                    if ((!window->isOrientationLocked())
                        || window->orientation() == static_cast<M::Orientation>(newAngle)) {
                        window->setOrientationAngle(static_cast<M::OrientationAngle>(newAngle));
                    }
                }
            }
        } else if (Qt::Key_P == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowPosition(!MApplication::showPosition());
            updateNeeded = true;
        } else if (Qt::Key_S == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowSize(!MApplication::showSize());
            updateNeeded = true;
        } else if (Qt::Key_B == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowBoundingRect(!MApplication::showBoundingRect());
            updateNeeded = true;
        } else if (Qt::Key_M == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowMargins(!MApplication::showMargins());
            updateNeeded = true;
        } else if (Qt::Key_N == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            MApplication::setShowObjectNames(!MApplication::showObjectNames());
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
        } else if (Qt::Key_T == k->key() && d->debugShortcutModifiersPresent(k->modifiers())) {
            QPixmap screenshot;
            screenshot = QPixmap::grabWindow(QApplication::desktop()->winId());

            QString path;
            if (QDir(ImagesPath).exists())
                path = ImagesPath;
            else
                path = QDir::homePath();

            if (!screenshot.save(QString("%1/%2-%3.png").arg(path)
                .arg(QDate::currentDate().toString("yyyyMMdd"))
                .arg(QTime::currentTime().toString("hhmmss"))))
                mWarning("MWindow") << "Could not save screenshot to" << path;

            d->playScreenshotEffect();
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
        } else if (MTheme::hasPendingRequests()) {
            // The showing of the window gets delayed until the theme
            // has finished to load all pixmap requests. This prevents
            // a flickering of the application on startup and improves
            // the performance.
            connect(MTheme::instance(), SIGNAL(pixmapRequestsFinished()),
                    this, SLOT(_q_onPixmapRequestsFinished()));
            return;            
        } else {
            if (!MApplication::softwareRendering() && d->glWidget == 0) {
                d->initGLViewport();
            }
            d->isLogicallyClosed = false;
        }

#ifdef Q_WS_X11
        MApplicationPrivate::removeWindowFromSwitcher(winId(), false);
#endif

    } else {
        MOnDisplayChangeEvent ev(false, sceneRect());
        onDisplayChangeEvent(&ev);
    }

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

#include "moc_mwindow.cpp"
