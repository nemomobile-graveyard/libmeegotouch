/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include "duiglrenderer.h"
#include "duiglwidget.h"
#include "duigles2renderer.h"
#endif

#include <QTimeLine>
#include <QSettings>

#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiorientationtracker.h"
#include "duideviceprofile.h"
#include "duiwindow.h"
#include "duiwindow_p.h"
#include "duideviceprofile.h"
#include "duiwidget.h"
#include "duicomponentdata.h"
#include "duiorientationchangeevent.h"
#include <duiondisplaychangeevent.h>
#include <DuiDebug>
#include <DuiGConfItem>
#include <DuiScene>
#include <duiscene_p.h>

#include "duilocale.h"

#ifdef Q_WS_X11
# include <QX11Info>
# include <X11/Xlib.h>
// Avoid conflict with QEvent::KeyPress usage in DuiWindow::Event
# undef KeyPress
#endif

/// Actual class

DuiWindowPrivate::DuiWindowPrivate() :
    glWidget(0),
    sceneManager(0),
    oldOrientation(Dui::Landscape), // the initial value is not used at all
    orientationAngleLocked(false),
    orientationLocked(false),
    onDisplay(false),
    onDisplaySet(false)
{
    DuiWindow *window = DuiApplication::activeWindow();

    if (window)
        angle = window->orientationAngle();
    else
        angle = DuiOrientationTracker::instance()->orientationAngle();
}

DuiWindowPrivate::~DuiWindowPrivate()
{
}

void DuiWindowPrivate::init()
{
    Q_Q(DuiWindow);

#ifdef Q_WS_X11
    // We do window decorations ourselves. Set env variable accordingly for
    // development purposes
    QString env = qgetenv("DUI_DECORATED");
    if (env.contains("0")) {
        q->setWindowFlags(Qt::FramelessWindowHint);
    }
    /* Workaround until we get DUI_DECORATED defined in the target env */
#ifdef __arm__
    else if (env.isEmpty()) {
        q->setWindowFlags(Qt::FramelessWindowHint);
    }
#endif

#endif

    q->resize(q->visibleSceneSize(q->orientation()));
    q->setFrameStyle(0);
    q->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    q->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (DuiApplication::softwareRendering() == false) {
#ifdef QT_OPENGL_LIB
        duiDebug("DuiWindowPrivate") << "Renderer: OpenGL";
#else
        duiDebug("DuiWindowPrivate") << "Renderer: Software";
#endif
    } else {
        duiDebug("DuiWindowPrivate") << "Renderer: Software (forced)";
    }

#ifdef Q_WS_X11
    appendVisibilityChangeMask();
#endif

    q->setTranslucentBackground(false);

    if (DuiApplication::fullScreen())
        q->showFullScreen();
}

#ifdef Q_WS_X11
void DuiWindowPrivate::appendVisibilityChangeMask()
{
    Q_Q(DuiWindow);

    XWindowAttributes existingAttributes;
    XSetWindowAttributes newAttributes;
    Status status;

    status = XGetWindowAttributes(QX11Info::display(), q->winId(), &existingAttributes);
    if (status == 0) {
        qFatal("DuiWindow: XGetWindowAttributes() failed!");
    }

    newAttributes.event_mask = existingAttributes.your_event_mask | VisibilityChangeMask;

    XChangeWindowAttributes(QX11Info::display(), q->winId(), CWEventMask, &newAttributes);
}
#endif

void DuiWindowPrivate::setLayoutDirection_helper(QGraphicsItem *item)
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
    }
    // Propagate this change to all children.
    const int size = item->childItems().size();
    for (int i = 0; i < size; ++i) {
        QGraphicsItem *childItem = item->childItems().at(i);
        setLayoutDirection_helper(childItem);
    }
    if (item->isWidget()) {
        // Send the notification event to this widget item.
        QEvent e(QEvent::LayoutDirectionChange);
        QApplication::sendEvent(static_cast<QGraphicsWidget *>(item), &e);
    }
}

Dui::Orientation DuiWindowPrivate::orientation(Dui::OrientationAngle angle) const
{
    return (angle == Dui::Angle0 || angle == Dui::Angle180) ? Dui::Landscape : Dui::Portrait;
}

void DuiWindowPrivate::_q_sendOrientationChangedSignal()
{
    Q_Q(DuiWindow);

    if (sceneManager == 0)
        return;

    emit q->orientationAngleChanged(sceneManager->orientationAngle());
    emit q->orientationChanged(sceneManager->orientation());
}

void DuiWindowPrivate::notifyWidgetsAboutOrientationChange()
{
    Q_Q(DuiWindow);

    Dui::Orientation newOrientation = q->orientation();

    if (sceneManager == 0 && oldOrientation != newOrientation) {
        emit q->orientationChanged(newOrientation);
        emit q->orientationChangeFinished(newOrientation);

        if (q->scene()) {
            DuiOrientationChangeEvent event(newOrientation);
            foreach(QGraphicsItem * item, q->scene()->items())
                q->scene()->sendEvent(item, &event);
        }
    }
}

void DuiWindowPrivate::doEnterDisplayEvent()
{
    Q_Q(DuiWindow);

    onDisplay = true;
    onDisplaySet = true;

    q->enterDisplayEvent();
    emit q->enteredDisplay();
}

void DuiWindowPrivate::doExitDisplayEvent()
{
    Q_Q(DuiWindow);

    onDisplay = false;
    onDisplaySet = true;

    q->exitDisplayEvent();
    emit q->exitedDisplay();
}

void DuiWindowPrivate::propagateDuiOnDisplayChangeEventToScene(DuiOnDisplayChangeEvent *event)
{
    Q_Q(DuiWindow);

    DuiOnDisplayChangeEvent::State eventState;

    if (event->state() == DuiOnDisplayChangeEvent::PartiallyOnDisplay ||
            event->state() == DuiOnDisplayChangeEvent::FullyOnDisplay ||
            event->state() == DuiOnDisplayChangeEvent::MustBeResolved) {
        eventState = DuiOnDisplayChangeEvent::MustBeResolved;
    } else {
        eventState = DuiOnDisplayChangeEvent::FullyOffDisplay;
    }


    DuiOnDisplayChangeEvent ev(eventState, q->sceneRect());

    // FIXME:
    // Actually sending the event would require overriding customEvent() which
    // would mess ABI compatibility. Calling the event handler directly for now
    q->scene()->d_func()->onDisplayChangeEvent(&ev);

}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(&dd)
{
    Q_D(DuiWindow);
    d->q_ptr = this;
    d->init();
    DuiComponentData::registerWindow(this);
}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, DuiScene *scene, QWidget *parent)
    : QGraphicsView(parent), d_ptr(&dd)
{
    Q_D(DuiWindow);
    d->q_ptr = this;
    d->init();
    DuiComponentData::registerWindow(this);
    setSceneManager(new DuiSceneManager(scene, this));
}

DuiWindow::DuiWindow(DuiWindowPrivate &dd, DuiSceneManager *sceneManager, QWidget *parent)
    : QGraphicsView(parent), d_ptr(&dd)
{
    Q_D(DuiWindow);
    d->q_ptr = this;
    d->init();
    DuiComponentData::registerWindow(this);
    setSceneManager(sceneManager);
}

DuiWindow::DuiWindow(DuiSceneManager *sceneManager, QWidget *parent)
    : QGraphicsView(parent), d_ptr(new DuiWindowPrivate)
{
    Q_D(DuiWindow);
    d->q_ptr = this;
    d->init();
    DuiComponentData::registerWindow(this);
    setSceneManager(sceneManager);
}

DuiWindow::DuiWindow(QWidget *parent)
    : QGraphicsView(parent),
      d_ptr(new DuiWindowPrivate)
{
    Q_D(DuiWindow);
    d->q_ptr = this;
    d->init();
    DuiComponentData::registerWindow(this);
}

DuiWindow::~DuiWindow()
{
    DuiComponentData::unregisterWindow(this);
    delete d_ptr;
}

void DuiWindow::setTranslucentBackground(bool enable)
{
    Q_D(DuiWindow);

    if (!DuiApplication::softwareRendering()) {
#ifdef QT_OPENGL_LIB
        setViewportUpdateMode(DuiWindow::FullViewportUpdate);

        QGLFormat fmt;
        // disable multisampling, is enabled by default in Qt
        fmt.setSampleBuffers(false);
        fmt.setSamples(0);

        // The sequence of calls here is important. When translucency is not
        // enabled, ensure setViewport() is called before DuiGLES2Renderer
        // initializes its vertices, otherwise call setViewport() after
        // DuiGLES2Renderer initializes itself. Failure to do this will cause
        // a crash.
        if (enable)  {
            //d->glWidget->setAttribute(Qt::WA_TranslucentBackground);

            fmt.setAlpha(true); // Workaround for NB#153625
            d->glWidget = new DuiGLWidget(fmt);
            QPalette palette;
            palette.setColor(QPalette::Base, Qt::transparent);
            d->glWidget->setAutoFillBackground(true);
            d->glWidget->setPalette(palette);
        } else {
            d->glWidget = new DuiGLWidget(fmt);
            setViewport(d->glWidget);
        }
#ifdef DUI_USE_OPENGL
        DuiGLRenderer::instance()->setGLWidget(d->glWidget);
        DuiGLRenderer::instance()->initGL(d->glWidget);
        DuiGLES2Renderer::instance(d->glWidget);
#endif
        if (enable)
            setViewport(d->glWidget);
#endif
    } else {
        viewport()->setAutoFillBackground(!enable);
    }
    if (enable)
        setAttribute(Qt::WA_TranslucentBackground);
}

DuiScene *DuiWindow::scene()
{
    return qobject_cast<DuiScene *>(QGraphicsView::scene());
}

bool DuiWindow::keepCurrentOrientation() const
{
    duiWarning("DuiWindow::keepCurrentOrientation()") << "THIS METHOD IS DEPRECATED - use isOrientationLocked()";
    return isOrientationLocked();
}

void DuiWindow::setKeepCurrentOrientation(bool enabled)
{
    duiWarning("DuiWindow::keepCurrentOrientation()") << "THIS METHOD IS DEPRECATED - use setOrientationLocked()";
    setOrientationLocked(enabled);
}

bool DuiWindow::isOrientationAngleLocked() const
{
    Q_D(const DuiWindow);

    return d->orientationAngleLocked;
}

void DuiWindow::setOrientationAngleLocked(bool locked)
{
    Q_D(DuiWindow);

    if (d->orientationAngleLocked != locked) {
        d->orientationAngleLocked = locked;

        // update from the orientation tracker if we're unlocking orientation changes
        if (!locked)
            setOrientationAngle(DuiOrientationTracker::instance()->orientationAngle());
    }
}

bool DuiWindow::isOrientationLocked() const
{
    Q_D(const DuiWindow);

    return d->orientationLocked;
}

void DuiWindow::setOrientationLocked(bool locked)
{
    Q_D(DuiWindow);

    if (d->orientationLocked != locked) {
        d->orientationLocked = locked;

        // update from the orientation tracker if we're unlocking orientation changes
        if (!locked)
            setOrientationAngle(DuiOrientationTracker::instance()->orientationAngle());
    }
}

void DuiWindow::lockOrientationAngle()
{
    setOrientationAngleLocked(true);
}

void DuiWindow::unlockOrientationAngle()
{
    setOrientationAngleLocked(false);
}

void DuiWindow::lockOrientation()
{
    setOrientationLocked(true);
}

void DuiWindow::unlockOrientation()
{
    setOrientationLocked(false);
}

void DuiWindow::setSceneManager(DuiSceneManager *sceneManager)
{
    Q_D(DuiWindow);

    if (d->sceneManager == sceneManager) {
        return;
    }

    if (d->sceneManager) {
        delete d->sceneManager;
    }

    d->sceneManager = sceneManager;

    if (sceneManager) {
        connect(sceneManager, SIGNAL(orientationChanged(Dui::Orientation)),
                SLOT(_q_sendOrientationChangedSignal()));
        connect(sceneManager, SIGNAL(orientationChangeFinished(Dui::Orientation)),
                SIGNAL(orientationChangeFinished(Dui::Orientation)));
        sceneManager->setParent(this);
        setScene(sceneManager->scene());
    }
}

DuiSceneManager *DuiWindow::sceneManager()
{
    Q_D(DuiWindow);

    // A scene manager is needed. Let's create one on the fly
    // if we don't have one already.
    if (!d->sceneManager) {
        setSceneManager(new DuiSceneManager);
    }

    return d->sceneManager;
}

DuiSceneManager *DuiWindow::sceneManager() const
{
    Q_D(const DuiWindow);

    return d->sceneManager;
}

Dui::Orientation DuiWindow::orientation() const
{
    Q_D(const DuiWindow);

    if (d->sceneManager) {
        return d->sceneManager->orientation();
    } else {
        return d->orientation(d->angle);
    }
}

Dui::OrientationAngle DuiWindow::orientationAngle() const
{
    Q_D(const DuiWindow);

    if (d->sceneManager) {
        return d->sceneManager->orientationAngle();
    } else {
        return d->angle;
    }
}

void DuiWindow::setOrientationAngle(Dui::OrientationAngle angle, Dui::OrientationChangeMode mode)
{
    Q_D(DuiWindow);

    if (d->angle != angle) {
        d->oldOrientation = orientation();
        d->angle = angle;

        if (d->sceneManager) {
            d->sceneManager->setOrientationAngle(angle, mode);
        } else {
            // first notify widgets, then emit the signal (in case someone
            // would like to connect to the signal and get correct size hints for widgets)
            d->notifyWidgetsAboutOrientationChange();
            emit orientationAngleChanged(angle);
        }
    }
}

QSize DuiWindow::visibleSceneSize(Dui::Orientation orientation) const
{
    QSize s;

    if (orientation == Dui::Landscape) {
        s = DuiDeviceProfile::instance()->resolution();
    } else {
        s = QSize(DuiDeviceProfile::instance()->resolution().height(),
                  DuiDeviceProfile::instance()->resolution().width());
    }

    return s;
}

QSize DuiWindow::visibleSceneSize() const
{
    return visibleSceneSize(orientation());
}

bool DuiWindow::isOnDisplay() const
{
    Q_D(const DuiWindow);

    return d->onDisplay;
}

void DuiWindow::enterDisplayEvent()
{}

void DuiWindow::exitDisplayEvent()
{}

void DuiWindow::onDisplayChangeEvent(DuiOnDisplayChangeEvent *event)
{
    Q_D(DuiWindow);

    switch (event->state()) {

    case DuiOnDisplayChangeEvent::FullyOnDisplay:
        if (!d->onDisplay || !d->onDisplaySet) {
            d->doEnterDisplayEvent();
        }
        break;

    case DuiOnDisplayChangeEvent::FullyOffDisplay:
        if (d->onDisplay || !d->onDisplaySet) {
            d->doExitDisplayEvent();
        }
        break;

    default:
        event->ignore();
        break;
    }

    if (scene()) {
        d->propagateDuiOnDisplayChangeEventToScene(event);
    }
}

void DuiWindow::paintEvent(QPaintEvent *event)
{
    // Disable view updates if the theme is not fully loaded yet
    // TODO: Alco check for "!isOnDisplay()" to block repaints if the
    // window is not visible anyway. Enable this once this works in
    // scratchbox.
    if (!updatesEnabled() || DuiTheme::hasPendingRequests()) {
        return;
    }
#ifdef DUI_USE_OPENGL
    Q_D(DuiWindow);

    if (!DuiApplication::softwareRendering()) {
        DuiGLES2Renderer::activate(d->glWidget);
        DuiGLRenderer::instance()->setGLWidget(d->glWidget);
    }
#endif // DUI_USE_OPENGL

    QGraphicsView::paintEvent(event);
}


bool DuiWindow::event(QEvent *event)
{
    Q_D(DuiWindow);

    if (event->type() == QEvent::Show || event->type() == QEvent::WindowActivate)
        DuiComponentData::setActiveWindow(this);

    if (event->type() == QEvent::Close) {
#if defined DUI_USE_OPENGL
        if (!DuiApplication::softwareRendering())
            DuiGLES2Renderer::destroy(d->glWidget);
#endif
        if (DuiComponentData::windows().size() > 1)
            DuiComponentData::setActiveWindow(DuiComponentData::windows().at(1));
    }

    if (event->type() == QEvent::Close || event->type() == QEvent::WindowDeactivate) {
        DuiOnDisplayChangeEvent ev(false, sceneRect());
        onDisplayChangeEvent(&ev);
    }

    if (QEvent::KeyPress == event->type()) {
        bool updateNeeded = false;

        //SIMULATION OF ROTATION FOR DEVELOPMENT PURPOSES
        QKeyEvent *k = (QKeyEvent *) event;
        if (Qt::Key_R == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            foreach(DuiWindow * window, DuiApplication::windows()) {
                int newAngle = (window->orientationAngle() + ((k->modifiers() & Qt::ShiftModifier) ? 270 : 90)) % 360;
                if (!window->isOrientationAngleLocked()) {
                    if (!window->isOrientationLocked() || window->orientation() == (Dui::Orientation)newAngle)
                        window->setOrientationAngle((Dui::OrientationAngle)newAngle);
                }
            }
        } else if (Qt::Key_P == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowPosition(!DuiApplication::showPosition());
            updateNeeded = true;
        } else if (Qt::Key_S == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowSize(!DuiApplication::showSize());
            updateNeeded = true;
        } else if (Qt::Key_B == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowBoundingRect(!DuiApplication::showBoundingRect());
            updateNeeded = true;
        } else if (Qt::Key_M == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowMargins(!DuiApplication::showMargins());
            updateNeeded = true;
        } else if (Qt::Key_N == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowObjectNames(!DuiApplication::showObjectNames());
            updateNeeded = true;
        } else if (Qt::Key_F == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            DuiApplication::setShowFps(!DuiApplication::showFps());
            updateNeeded = true;
        } else if (Qt::Key_D == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            Qt::LayoutDirection dir = DuiApplication::layoutDirection();

            if (dir == Qt::LeftToRight)
                dir = Qt::RightToLeft;
            else
                dir = Qt::LeftToRight;

            DuiApplication::setLayoutDirection(dir);

            updateNeeded = true;
        } else if (Qt::Key_L == k->key() && (k->modifiers() & (Qt::ControlModifier | Qt::AltModifier))) {
            // switch language

            DuiGConfItem languageItem("/Dui/i18n/Language");
            QString language = languageItem.value().toString();
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
                language = "";
            languageItem.set(language);

            updateNeeded = true;
        }

        if (updateNeeded) {
            this->viewport()->update();
        }
    } else if (event->type() == QEvent::ApplicationLayoutDirectionChange) {
        // tell the scene and its items about the layout change
        if (scene()) {
            QList<QGraphicsItem *> items = scene()->items();

            // call setLayoutDirection_helper() for all top-level items
            foreach(QGraphicsItem * item, items) {
                if (!item->parentItem())
                    d->setLayoutDirection_helper(static_cast<QGraphicsWidget *>(item));
            }
        }
        return true;
    } else if (event->type() == QEvent::LanguageChange) {
        // tell the scene and its items about the language change
        if (scene()) {
            QList<QGraphicsItem *> items = scene()->items();

            foreach(QGraphicsItem * item, items) {
                if (item->isWidget()) {
                    QGraphicsWidget *widget = static_cast<QGraphicsWidget *>(item);

                    QEvent ev(QEvent::LanguageChange);
                    qApp->sendEvent(widget, &ev);
                }
            }
        }
        return true;
    } else if (event->type() == DuiOnDisplayChangeEvent::eventNumber()) {
        onDisplayChangeEvent(static_cast<DuiOnDisplayChangeEvent *>(event));
        return true;
    }

    return QGraphicsView::event(event);
}

#include "moc_duiwindow.cpp"
