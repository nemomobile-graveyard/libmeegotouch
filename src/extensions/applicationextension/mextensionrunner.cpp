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

#include "mextensionrunner.h"
#include <mappletclient.h>
#include <mappletmessage.h>
#include <mappletmousemessage.h>
#include <mappletorientationmessage.h>
#include <mappletvisibilitymessage.h>
#include <mappletsetgeometrymessage.h>
#include <mappletpixmaptakenintousemessage.h>
#include <mappletalivemessagerequest.h>
#include <mappletalivemessageresponse.h>
#include <mappleticonchangedmessage.h>
#include <mapplettitlechangedmessage.h>
#include <mapplettextchangedmessage.h>
#include <mappletupdategeometrymessage.h>
#include <mappletpixmapmodifiedmessage.h>
#include <mappletcancelmessage.h>
#include <mappletobjectmenumessage.h>
#include <mappletobjectmenurequestmessage.h>
#include <mappletobjectmenuactionselectedmessage.h>
#include <mappletinterface.h>
#include <mappletmetadata.h>
#include <mappletsettings.h>
#include <mfiledatastore.h>
#include <mcancelevent.h>
#include <mondisplaychangeevent.h>
#include <MScene>
#include <MSceneManager>
#include <MAction>
#include <MDebug>
#include <QGraphicsView>
#include <QLocalSocket>
#include <QPluginLoader>
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaProperty>
#include <QTimer>
#include <QAction>
#include <QGraphicsLinearLayout>
#ifdef Q_WS_X11
#include <QX11Info>
#endif
#include "mappletsharedmutex.h"
#ifdef QT_OPENGL_LIB
#include <QGLWidget>
#endif
#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include <typeinfo>

//! The number of milliseconds to wait for a message from the host process until it is considered to be dead
#define ALIVE_TIMER_TIMEOUT 20000

//! \internal An internal widget for listening to LayoutRequests
class LayoutRequestListenerWidget : public QGraphicsWidget
{
public:
    LayoutRequestListenerWidget(MExtensionRunner *parentRunner);

protected:
    MExtensionRunner *parentRunner;
    bool event(QEvent *event);
};
//! \internal_end

LayoutRequestListenerWidget::LayoutRequestListenerWidget(MExtensionRunner *parentRunner) :
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

MExtensionRunner::MExtensionRunner() :
    communicator(new MAppletClient),
    widget(NULL),
    scene(NULL),
    view(NULL),
#ifdef HAVE_CONTEXTSUBSCRIBER
    screenBlankProperty(new ContextProperty("Session.State", this)),
#endif
    displayBlanked(false),
    aliveTimer(new QTimer),
    pixmapMutex(NULL),
    changedRect(QRectF()),
    visible(true)
{
    aliveTimer->setSingleShot(true);
    connect(communicator, SIGNAL(messageReceived(MAppletMessage)), this, SLOT(messageReceived(MAppletMessage)));
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(hostProcessNotAlive()));
    connect(communicator, SIGNAL(connectionLost()), this, SLOT(hostProcessNotAlive()));
#ifdef HAVE_CONTEXTSUBSCRIBER
    connect(screenBlankProperty, SIGNAL(valueChanged()), this, SLOT(updateDisplayState()));
#endif
}

MExtensionRunner::~MExtensionRunner()
{
    teardown();
    delete aliveTimer;
    delete communicator;
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete screenBlankProperty;
#endif
}

bool MExtensionRunner::init(const QString &serverName)
{
    // Take shared memory into use
    pixmapMutex = new MAppletSharedMutex;
    if (!pixmapMutex->init(serverName)) {
        mWarning("MExtensionRunner") << "Unable to attach to shared memory.";
        return false;
    }

    // Create a view for the scene
    view = new MWindow();

    // Remove "everything" from the view so that it only contains the viewport
    // and no scrollbars or borders
    // TODO FIXME: The borders are still appearing making viewport size
    //             a bit too small
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setWindowFlags(Qt::FramelessWindowHint);

    // Create a scene for the widget
    scene = new MScene();

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

void MExtensionRunner::updateDisplayState()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    bool blanked = screenBlankProperty->value().toString() == "blanked";

    if(displayBlanked != blanked) {
        displayBlanked = blanked;

        if(blanked) {
            aliveTimer->stop();
        } else {
            aliveTimer->start(ALIVE_TIMER_TIMEOUT);
        }
    }
#endif
}


void MExtensionRunner::teardown()
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

void MExtensionRunner::messageReceived(const MAppletMessage &message)
{
    if (view == NULL) {
        return;
    }

    aliveTimer->stop();
    if(!displayBlanked) {
        aliveTimer->start(ALIVE_TIMER_TIMEOUT);
    }

    switch (message.type()) {
    case MAppletMessage::MousePressMessage:
    case MAppletMessage::MouseReleaseMessage:
    case MAppletMessage::MouseMoveMessage: {
        const MAppletMouseMessage *m = dynamic_cast<const MAppletMouseMessage *>(&message);
        if (m != NULL) {
            handleMouseEvent(m);
        }
        break;
    }

    case MAppletMessage::CancelMessage: {
        const MAppletCancelMessage *m = dynamic_cast<const MAppletCancelMessage *>(&message);
        if (m != NULL) {
            handleCancelEvent(m);
        }
        break;
    }

    case MAppletMessage::VisibilityMessage: {
        const MAppletVisibilityMessage *m = dynamic_cast<const MAppletVisibilityMessage *>(&message);
        if (m != NULL) {
            if (m->isVisible() != visible) {
                // Visible area rectangle is irrelevant when sending
                // on or off display events.
                QRectF dummyRectangle;
                emit visibilityChanged();
                visible = m->isVisible();
                if (visible) {
                    MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOnDisplay, dummyRectangle);
                    QList<QRectF> updateRegion;
                    updateRegion.append(changedRect);
                    sceneChanged(updateRegion);
                    scene->sendEvent(widget, &event);
                } else {
                    MOnDisplayChangeEvent event(MOnDisplayChangeEvent::FullyOffDisplay, dummyRectangle);
                    scene->sendEvent(widget, &event);
                }
            }
        }
        break;
    }

    case MAppletMessage::OrientationMessage: {
        const MAppletOrientationMessage *m = dynamic_cast<const MAppletOrientationMessage *>(&message);

        if (m != NULL) {
            // Inform the out of process device profile about orientation changes
            // TODO there's no way to know for now whether 180 should be added to the angle
            view->setOrientationAngle(m->orientation() == M::Landscape ? M::Angle0 : M::Angle90);

            // Update the geometry since style changes may have changed the size hints
            QGraphicsLayoutItem *item = dynamic_cast<QGraphicsLayoutItem *>(widget);
            if (item != NULL) {
                item->updateGeometry();
            }
        }
        break;
    }

    case MAppletMessage::SetGeometryMessage: {
        const MAppletSetGeometryMessage *m = dynamic_cast<const MAppletSetGeometryMessage *>(&message);
        if (m != NULL) {
            // Take the new X pixmap into use
            createAppletPixmap(m->handle());

            // Tell the host process that the old X pixmap can now be freed
            communicator->sendMessage(MAppletPixmapTakenIntoUseMessage(m->handle()));

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

    case MAppletMessage::AppletAliveMessageRequest: {
        const MAppletAliveMessageRequest *m = dynamic_cast<const MAppletAliveMessageRequest *>(&message);
        if (m != NULL) {
            communicator->sendMessage(MAppletAliveMessageResponse());
        }
        break;
    }

    case MAppletMessage::ObjectMenuRequestMessage: {
        const MAppletObjectMenuRequestMessage *m = dynamic_cast<const MAppletObjectMenuRequestMessage *>(&message);

        if (m != NULL) {
            widgetObjectMenuActions.clear();

            bool foundWidgetWithActions = false;
            foreach(QGraphicsItem * item, scene->items(m->pos(), Qt::IntersectsItemShape, Qt::DescendingOrder)) {
                QGraphicsWidget *widget = dynamic_cast<QGraphicsWidget *>(item);
                if (widget != NULL) {
                    foreach(QAction * qAction, widget->actions()) {
                        MAction *action = qobject_cast<MAction *>(qAction);
                        if (action && action->isVisible() && (action->location() & MAction::ObjectMenuLocation)) {
                            widgetObjectMenuActions.append(action);
                            foundWidgetWithActions = true;
                        }
                    }
                    if (foundWidgetWithActions) {
                        break;
                    }
                }
            }

            MAppletObjectMenuMessage message(widgetObjectMenuActions);
            communicator->sendMessage(message);
        }
        break;
    }

    case MAppletMessage::ObjectMenuActionSelectedMessage: {
        const MAppletObjectMenuActionSelectedMessage *m = dynamic_cast<const MAppletObjectMenuActionSelectedMessage *>(&message);

        if (m != NULL) {
            widgetObjectMenuActions.at(m->index())->trigger();
        }
        break;
    }

    default:
        mWarning("MExtensionRunner") << "Unrecognized event id received: " << message.type();
        break;
    }
}

void MExtensionRunner::handleMouseEvent(const MAppletMouseMessage *message)
{
    // Convert our own event id to Qt mouse event id
    QEvent::Type type = QEvent::MouseButtonPress;
    switch (message->type()) {
    case MAppletMessage::MousePressMessage:
        type = QEvent::MouseButtonPress;
        break;
    case MAppletMessage::MouseReleaseMessage:
        type = QEvent::MouseButtonRelease;
        break;
    case MAppletMessage::MouseMoveMessage:
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

void MExtensionRunner::handleCancelEvent(const MAppletCancelMessage *message)
{
    Q_UNUSED(message);
    // We need to send the cancel event to the widget that is currently
    // grabbing the mouse, because this will be the widget that got the
    // mousePressEvent in previously received message.
    QGraphicsWidget *obj = dynamic_cast<QGraphicsWidget *>(scene->mouseGrabberItem());
    if (!obj) return;

    MCancelEvent e;
    if (QCoreApplication::sendEvent(obj, &e)) {
        // Ungrab the mouse explicitly as the item still behaves as mouse grabber
        // and does not loose grabbing after mouse release event (for some reason).
        // Doing ungrabbing at this point allows the scene to ignore all mouse events
        // until a new item in the scene grabs the mouse (i.e., until a new item receives
        // a mouse press event).
        obj->ungrabMouse();
    }
}

void MExtensionRunner::createAppletPixmap(Qt::HANDLE handle)
{
    // Create a QPixmap out of the pixmap handle
    pixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);

#ifdef QT_OPENGL_LIB
    // The QGLWidget takes the ownership of the context
    QGLContext *context = new QGLContext(QGLFormat::defaultFormat(), &pixmap);
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

void MExtensionRunner::sceneChanged(const QList<QRectF> &region)
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
                MAppletPixmapModifiedMessage m(changedRect);
                communicator->sendMessage(m);
                // Clear the changedRect, since it has been sent and the widget is visible
                changedRect = QRectF();
            }
        }
    }
}

void MExtensionRunner::hostProcessNotAlive()
{
    qApp->quit();
}

void MExtensionRunner::sendUpdateGeometryMessage()
{
    if (widget != NULL) {
        // Send the new size hints along with the update geometry message. Only constraint -1,-1 is supported.
        QVector<QSizeF> sizeHints;
        for (int i = 0; i < Qt::NSizeHints; ++i) {
            sizeHints.append(widget->effectiveSizeHint((Qt::SizeHint)i, QSizeF(-1, -1)));
        }
        communicator->sendMessage(MAppletUpdateGeometryMessage(sizeHints));
    }
}
