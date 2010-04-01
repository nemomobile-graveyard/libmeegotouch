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

#include "duiextensionrunner.h"
#include <duiappletclient.h>
#include <duiappletmessage.h>
#include <duiappletmousemessage.h>
#include <duiappletorientationmessage.h>
#include <duiappletvisibilitymessage.h>
#include <duiappletsetgeometrymessage.h>
#include <duiappletpixmaptakenintousemessage.h>
#include <duiappletalivemessagerequest.h>
#include <duiappletalivemessageresponse.h>
#include <duiappleticonchangedmessage.h>
#include <duiapplettitlechangedmessage.h>
#include <duiapplettextchangedmessage.h>
#include <duiappletupdategeometrymessage.h>
#include <duiappletpixmapmodifiedmessage.h>
#include <duiappletcancelmessage.h>
#include <duiappletobjectmenumessage.h>
#include <duiappletobjectmenurequestmessage.h>
#include <duiappletobjectmenuactionselectedmessage.h>
#include <duiappletinterface.h>
#include <duiappletmetadata.h>
#include <duiappletsettings.h>
#include <duifiledatastore.h>
#include <duicancelevent.h>
#include <duiondisplaychangeevent.h>
#include <DuiWidget>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiAction>
#include <QGraphicsView>
#include <QLocalSocket>
#include <QPluginLoader>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaProperty>
#include <QTimer>
#include <QDebug>
#include <QAction>
#include <QGraphicsLinearLayout>
#ifdef Q_WS_X11
#include <QX11Info>
#endif
#include "duiappletsharedmutex.h"
#ifdef QT_OPENGL_LIB
#include <QGLWidget>
#endif

#include <typeinfo>

//! The number of milliseconds to wait for a message from the host process until it is considered to be dead
#define ALIVE_TIMER_TIMEOUT 20000

//! \internal An internal widget for listening to LayoutRequests
class LayoutRequestListenerWidget : public QGraphicsWidget
{
public:
    LayoutRequestListenerWidget(DuiExtensionRunner *parentRunner);

protected:
    DuiExtensionRunner *parentRunner;
    bool event(QEvent *event);
};
//! \internal_end

LayoutRequestListenerWidget::LayoutRequestListenerWidget(DuiExtensionRunner *parentRunner) :
    parentRunner(parentRunner)
{
}

bool LayoutRequestListenerWidget::event(QEvent *event)
{
    bool returnValue = QGraphicsWidget::event(event);
    if (event->type() == QEvent::LayoutRequest) {
        parentRunner->sendUpdateGeometryMessage();
    }
    return returnValue;
}

DuiExtensionRunner::DuiExtensionRunner() :
    communicator(new DuiAppletClient),
    widget(NULL),
    scene(NULL),
    view(NULL),
#ifdef QT_OPENGL_LIB
    context(NULL),
#endif
    aliveTimer(new QTimer),
    pixmapMutex(NULL),
    changedRect(QRectF()),
    visible(true)
{
    aliveTimer->setSingleShot(true);
    connect(communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(hostProcessNotAlive()));
    connect(communicator, SIGNAL(connectionLost()), this, SLOT(hostProcessNotAlive()));
}

DuiExtensionRunner::~DuiExtensionRunner()
{
    teardown();
    delete aliveTimer;
    delete communicator;
#ifdef QT_OPENGL_LIB
    delete context;
#endif
}

bool DuiExtensionRunner::init(const QString &serverName)
{
    // Take shared memory into use
    pixmapMutex = new DuiAppletSharedMutex;
    if (!pixmapMutex->init(serverName)) {
        qWarning() << "Unable to attach to shared memory.";
        return false;
    }

    // Create a view for the scene
    view = new DuiWindow();

    // Remove "everything" from the view so that it only contains the viewport
    // and no scrollbars or borders
    // TODO FIXME: The borders are still appearing making viewport size
    //             a bit too small
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setWindowFlags(Qt::FramelessWindowHint);

    // Create a scene for the widget
    scene = new DuiScene();

    // Create the widget layout
    QGraphicsLinearLayout *widgetLayout = new QGraphicsLinearLayout;
    widgetLayout->setContentsMargins(0, 0, 0, 0);

    parentWidget = new LayoutRequestListenerWidget(this);
    parentWidget->setLayout(widgetLayout);

    scene->addItem(parentWidget);

    // Set the scene for the view
    view->setScene(scene);

    // Get signaled when the scene changes
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged(QList<QRectF>)));

    // Connect to server socket
    if (!communicator->connectToServer(serverName)) {
        teardown();
        return false;
    }

    return true;
}

void DuiExtensionRunner::teardown()
{
    delete scene;
    scene = NULL;
    // Set the widgets to NULL. The scene has destroyed them already.
    parentWidget = NULL;
    widget = NULL;
    delete view;
    view = NULL;

    communicator->closeConnection();

    delete pixmapMutex;
    pixmapMutex = NULL;
}

void DuiExtensionRunner::messageReceived(const DuiAppletMessage &message)
{
    if (view == NULL) {
        return;
    }

    aliveTimer->stop();
    aliveTimer->start(ALIVE_TIMER_TIMEOUT);

    switch (message.type()) {
    case DuiAppletMessage::MousePressMessage:
    case DuiAppletMessage::MouseReleaseMessage:
    case DuiAppletMessage::MouseMoveMessage: {
        const DuiAppletMouseMessage *m = dynamic_cast<const DuiAppletMouseMessage *>(&message);
        if (m != NULL) {
            handleMouseEvent(m);
        }
        break;
    }

    case DuiAppletMessage::CancelMessage: {
        const DuiAppletCancelMessage *m = dynamic_cast<const DuiAppletCancelMessage *>(&message);
        if (m != NULL) {
            handleCancelEvent(m);
        }
        break;
    }

    case DuiAppletMessage::VisibilityMessage: {
        const DuiAppletVisibilityMessage *m = dynamic_cast<const DuiAppletVisibilityMessage *>(&message);
        if (m != NULL) {
            if (m->isVisible() != visible) {
                // Visible area rectangle is irrelevant when sending
                // on or off display events.
                QRectF dummyRectangle;
                emit visibilityChanged(m->isVisible());
                visible = m->isVisible();
                if (visible) {
                    DuiOnDisplayChangeEvent event(DuiOnDisplayChangeEvent::FullyOnDisplay, dummyRectangle);
                    QList<QRectF> updateRegion;
                    updateRegion.append(changedRect);
                    sceneChanged(updateRegion);
                    scene->sendEvent(widget, &event);
                } else {
                    DuiOnDisplayChangeEvent event(DuiOnDisplayChangeEvent::FullyOffDisplay, dummyRectangle);
                    scene->sendEvent(widget, &event);
                }
            }
        }
        break;
    }

    case DuiAppletMessage::OrientationMessage: {
        const DuiAppletOrientationMessage *m = dynamic_cast<const DuiAppletOrientationMessage *>(&message);

        if (m != NULL) {
            // Inform the out of process device profile about orientation changes
            // TODO there's no way to know for now whether 180 should be added to the angle
            view->setOrientationAngle(m->orientation() == Dui::Landscape ? Dui::Angle0 : Dui::Angle90);

            // Update the geometry since style changes may have changed the size hints
            QGraphicsLayoutItem *item = dynamic_cast<QGraphicsLayoutItem *>(widget);
            if (item != NULL) {
                item->updateGeometry();
            }
        }
        break;
    }

    case DuiAppletMessage::SetGeometryMessage: {
        const DuiAppletSetGeometryMessage *m = dynamic_cast<const DuiAppletSetGeometryMessage *>(&message);
        if (m != NULL) {
            // Take the new X pixmap into use
            createAppletPixmap(m->handle());

            // Tell the host process that the old X pixmap can now be freed
            communicator->sendMessage(DuiAppletPixmapTakenIntoUseMessage(m->handle()));

            // Set the widget geometry
            QRect rect(0, 0, m->geometry().width(), m->geometry().height());
            parentWidget->setGeometry(rect);
            view->setGeometry(rect);
            view->viewport()->setGeometry(rect);
            scene->setSceneRect(m->geometry());
            if (widget != NULL) {
                widget->update();
            }
        }
        break;
    }

    case DuiAppletMessage::AppletAliveMessageRequest: {
        const DuiAppletAliveMessageRequest *m = dynamic_cast<const DuiAppletAliveMessageRequest *>(&message);
        if (m != NULL) {
            communicator->sendMessage(DuiAppletAliveMessageResponse());
        }
        break;
    }

    case DuiAppletMessage::ObjectMenuRequestMessage: {
        const DuiAppletObjectMenuRequestMessage *m = dynamic_cast<const DuiAppletObjectMenuRequestMessage *>(&message);

        if (m != NULL) {
            widgetObjectMenuActions.clear();

            bool foundWidgetWithActions = false;
            foreach(QGraphicsItem * item, scene->items(m->pos(), Qt::IntersectsItemShape, Qt::DescendingOrder)) {
                QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(item);
                if (widget != NULL) {
                    foreach(QAction * qAction, widget->actions()) {
                        DuiAction *action = qobject_cast<DuiAction *>(qAction);
                        if (action && action->isVisible() && (action->location() & DuiAction::ObjectMenuLocation)) {
                            widgetObjectMenuActions.append(action);
                            foundWidgetWithActions = true;
                        }
                    }
                    if (foundWidgetWithActions) {
                        break;
                    }
                }
            }

            DuiAppletObjectMenuMessage message(widgetObjectMenuActions);
            communicator->sendMessage(message);
        }
        break;
    }

    case DuiAppletMessage::ObjectMenuActionSelectedMessage: {
        const DuiAppletObjectMenuActionSelectedMessage *m = dynamic_cast<const DuiAppletObjectMenuActionSelectedMessage *>(&message);

        if (m != NULL) {
            widgetObjectMenuActions.at(m->index())->trigger();
        }
        break;
    }

    default:
        qWarning() << __func__  << "Unrecognized event id received: " << message.type();
        break;
    }
}

void DuiExtensionRunner::handleMouseEvent(const DuiAppletMouseMessage *message)
{
    // Convert our own event id to Qt mouse event id
    QEvent::Type type = QEvent::MouseButtonPress;
    switch (message->type()) {
    case DuiAppletMessage::MousePressMessage:
        type = QEvent::MouseButtonPress;
        break;
    case DuiAppletMessage::MouseReleaseMessage:
        type = QEvent::MouseButtonRelease;
        break;
    case DuiAppletMessage::MouseMoveMessage:
        type = QEvent::MouseMove;
        break;
    default:
        break;
    }

    // Create Qt mouse event and send it to the widget through viewport and scene
    // global position is used by the Qt to do some hit testing (don't know why?) so
    // it must provided also
    QPoint p(message->position().x(), message->position().y());
    QPoint p2(view->viewport()->pos() + p);
    QMouseEvent event(type, p, p2, message->button(), message->buttons(), 0);
    QCoreApplication::sendEvent(view->viewport(), &event);
}

void DuiExtensionRunner::handleCancelEvent(const DuiAppletCancelMessage *message)
{
    Q_UNUSED(message);
    // We need to send the cancel event to the widget that is currently
    // grabbing the mouse, because this will be the widget that got the
    // mousePressEvent in previously received message.
    QGraphicsWidget *obj = dynamic_cast<QGraphicsWidget *>(scene->mouseGrabberItem());
    if (!obj) return;

    DuiCancelEvent e;
    if (QCoreApplication::sendEvent(obj, &e)) {
        // Ungrab the mouse explicitly as the item still behaves as mouse grabber
        // and does not loose grabbing after mouse release event (for some reason).
        // Doing ungrabbing at this point allows the scene to ignore all mouse events
        // until a new item in the scene grabs the mouse (i.e., until a new item receives
        // a mouse press event).
        obj->ungrabMouse();
    }
}

void DuiExtensionRunner::createAppletPixmap(Qt::HANDLE handle)
{
    // Create a QPixmap out of the pixmap handle
    pixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);

#ifdef QT_OPENGL_LIB
    // Try to use opengl to render directly to the pixmap
    if (context != NULL)
        delete context;

    context = new QGLContext(QGLFormat::defaultFormat(), &pixmap);
    context->create();
    if (context->isValid()) {
        qDebug() << "Direct HW accelerated rendering to pixmap is supported.";
        view->setViewport(new QGLWidget(context));
    } else {
        qDebug() << "Direct HW accelerated rendering to pixmap is NOT supported.";
        delete context;
        context = NULL;
    }
#endif
}

void DuiExtensionRunner::sceneChanged(const QList<QRectF> &region)
{
    if (!region.empty() && !pixmap.isNull()) {
        // Lock the pixmap mutex: this blocking call can't really fail in our case (see man pthread_mutex_lock) and even if it would there's not much that can be done about it
        if (pixmapMutex->lock()) {
            QPainter painter(&pixmap);

            foreach(const QRectF & r, region) {
                // Don't draw areas that are outside the pixmap
                QRectF drawRect = r.intersected(pixmap.rect());

                if (visible && painter.isActive()) {
                    // Clear region with full transparency
                    painter.save();
                    painter.setCompositionMode(QPainter::CompositionMode_Clear);
                    painter.fillRect(drawRect, QBrush(QColor(0, 0, 0, 0)));
                    painter.restore();

                    // Render the changes to the scene
                    scene->render(&painter, drawRect, drawRect);
                }

                // Calculate the union of the changed regions
                changedRect = changedRect.united(drawRect);
            }

            if (visible && painter.isActive()) {
                // Synchronize X to get the changes drawn to the pixmap
                QApplication::syncX();
            }

            // Unlock the pixmap mutex
            pixmapMutex->unlock();

            // Inform the host process about the union of the changed regions (if any)
            if (!changedRect.isEmpty() && visible) {
                DuiAppletPixmapModifiedMessage m(changedRect);
                communicator->sendMessage(m);
                // Clear the changedRect, since it has been sent and the widget is visible
                changedRect = QRectF();
            }
        }
    }
}

void DuiExtensionRunner::hostProcessNotAlive()
{
    qApp->quit();
}

void DuiExtensionRunner::sendUpdateGeometryMessage()
{
    if (widget != NULL) {
        // Send the new size hints along with the update geometry message. Only constraint -1,-1 is supported.
        QVector<QSizeF> sizeHints;
        for (int i = 0; i < Qt::NSizeHints; ++i) {
            sizeHints.append(widget->effectiveSizeHint((Qt::SizeHint)i, QSizeF(-1, -1)));
        }
        communicator->sendMessage(DuiAppletUpdateGeometryMessage(sizeHints));
    }
}
