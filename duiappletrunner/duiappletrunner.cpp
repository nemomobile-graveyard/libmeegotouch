/****************************************************************************
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

#include "duiappletrunner.h"
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
#include <duiappletloader.h>
#include <duigconfdatastore.h>
#include <duisettingslanguagebinary.h>
#include <duisettingslanguageparser.h>
#include <duicancelevent.h>
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

//! An internal widget for listening to LayoutRequests
class LayoutRequestListenerWidget : public QGraphicsWidget
{
public:
    LayoutRequestListenerWidget(DuiAppletRunner *parentRunner);

protected:
    DuiAppletRunner *parentRunner;
    bool event(QEvent *event);
};

LayoutRequestListenerWidget::LayoutRequestListenerWidget(DuiAppletRunner *parentRunner) :
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

DuiAppletRunner::DuiAppletRunner() :
    communicator(new DuiAppletClient),
    widget(NULL),
    scene(NULL),
    view(NULL),
    appletInstanceDataStore(NULL),
#ifdef QT_OPENGL_LIB
    context(NULL),
#endif
    aliveTimer(new QTimer),
    pixmapMutex(NULL),
    appletSettings(NULL),
    changedRect(QRectF()),
    appletVisible(true)
{
    aliveTimer->setSingleShot(true);
    connect(communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));
    connect(aliveTimer, SIGNAL(timeout()), this, SLOT(hostProcessNotAlive()));
    connect(communicator, SIGNAL(connectionLost()), this, SLOT(hostProcessNotAlive()));
}

DuiAppletRunner::~DuiAppletRunner()
{
    teardown();
    delete appletSettings;
    delete aliveTimer;
    delete communicator;
#ifdef QT_OPENGL_LIB
    delete context;
#endif
}

bool DuiAppletRunner::init(const QString &appletId, const QString &appletInstanceFileDataPath, DuiAppletMetaData &metaData, const QString &serverName)
{
    // Clear any previous state
    teardown();

    // Check whether the applet ID is valid
    if (appletId.isEmpty()) {
        qWarning() << "Applet ID is not valid.";
        return false;
    }

    // Take shared memory into use
    pixmapMutex = new DuiAppletSharedMutex;
    if (!pixmapMutex->init(serverName)) {
        qWarning() << "Unable to attach to shared memory.";
        return false;
    }

    // Check whether the metadata is valid
    if (!metaData.isValid()) {
        qWarning() << "Applet metadata is not valid.";
        return false;
    }

    // Try to construct a valid instance data store and bail out if not successful
    appletInstanceDataStore = new DuiFileDataStore(appletInstanceFileDataPath);
    if (!appletInstanceDataStore->isReadable() || !appletInstanceDataStore->isWritable()) {
        qWarning() << "Couldn't create applet instance data store";
        teardown();
        return false;
    }

    appletSettings = new DuiAppletSettings(metaData.fileName(), appletId);

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

    // Load the applet
    widget = DuiAppletLoader::loadApplet(metaData, *appletInstanceDataStore, *appletSettings->dataAccess());

    if (widget == NULL) {
        qWarning() << "Failed to construct actual applet widget.";
        teardown();
        return false;
    }

    QGraphicsLinearLayout *widgetLayout = new QGraphicsLinearLayout;
    widgetLayout->setContentsMargins(0, 0, 0, 0);

    parentWidget = new LayoutRequestListenerWidget(this);
    parentWidget->setLayout(widgetLayout);

    widgetLayout->addItem(widget);

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

    setAppletProperties();

    return true;
}

void DuiAppletRunner::setAppletProperties()
{
    if (widget == NULL) {
        return;
    }

    const QMetaObject *mob = widget->metaObject();

    // use applet properties to fill header of the container
    int iconProperty = mob->indexOfProperty("appletIcon");
    int titleProperty = mob->indexOfProperty("appletTitle");
    int textProperty = mob->indexOfProperty("appletText");

    int iconSignal  = mob->indexOfSignal("appletIconChanged(QString)");
    int titleSignal = mob->indexOfSignal("appletTitleChanged(QString)");
    int textSignal  = mob->indexOfSignal("appletTextChanged(QString)");

    if (iconProperty != -1) {
        QString icon((mob->property(iconProperty).read(widget)).toString());
        if (!icon.isNull()) {
            appletIconChanged(icon);
        }
    }

    if (titleProperty != -1) {
        QString title((mob->property(titleProperty).read(widget)).toString());
        if (!title.isNull()) {
            appletTitleChanged(title);
        }
    }

    if (textProperty != -1) {
        QString text((mob->property(textProperty).read(widget)).toString());
        if (!text.isNull()) {
            appletTextChanged(text);
        }
    }

    if (iconSignal != -1) {
        connect(widget, SIGNAL(appletIconChanged(QString)), this, SLOT(appletIconChanged(QString)));
    }

    if (titleSignal != -1) {
        connect(widget, SIGNAL(appletTitleChanged(QString)), this, SLOT(appletTitleChanged(QString)));
    }

    if (textSignal != -1) {
        connect(widget, SIGNAL(appletTextChanged(QString)), this, SLOT(appletTextChanged(QString)));
    }

    // Connect the deprecated signals
    // TODO remove these after the deprecation period
    iconSignal  = mob->indexOfSignal("setAppletIcon(QString)");
    titleSignal = mob->indexOfSignal("setAppletTitle(QString)");
    textSignal  = mob->indexOfSignal("setAppletText(QString)");
    if (iconSignal != -1) {
        connect(widget, SIGNAL(setAppletIcon(QString)), this, SLOT(appletIconChanged(QString)));
    }
    if (titleSignal != -1) {
        connect(widget, SIGNAL(setAppletTitle(QString)), this, SLOT(appletTitleChanged(QString)));
    }
    if (textSignal != -1) {
        connect(widget, SIGNAL(setAppletText(QString)), this, SLOT(appletTextChanged(QString)));
    }

    connect(this, SIGNAL(visibilityChanged(bool)), widget, SIGNAL(visibilityChanged(bool)));
    connect(view, SIGNAL(orientationChanged(Dui::Orientation)), widget, SIGNAL(orientationChanged(Dui::Orientation)));
}

void DuiAppletRunner::teardown()
{
    delete scene;
    scene = NULL;
    // Set the widgets to NULL. The scene has destroyed them already.
    parentWidget = NULL;
    widget = NULL;
    delete view;
    view = NULL;

    communicator->closeConnection();

    delete appletInstanceDataStore;
    appletInstanceDataStore = NULL;

    delete pixmapMutex;
    pixmapMutex = NULL;
}

void DuiAppletRunner::messageReceived(const DuiAppletMessage &message)
{
    if (widget == NULL) {
        return;
    }

    aliveTimer->stop();
    aliveTimer->start(ALIVE_TIMER_TIMEOUT);

    switch (message.type()) {
    case DuiAppletMessage::MOUSE_PRESS_MESSAGE:
    case DuiAppletMessage::MOUSE_RELEASE_MESSAGE:
    case DuiAppletMessage::MOUSE_MOVE_MESSAGE: {
        const DuiAppletMouseMessage *m = dynamic_cast<const DuiAppletMouseMessage *>(&message);
        if (m != NULL) {
            handleMouseEvent(m);
        }
        break;
    }

    case DuiAppletMessage::CANCEL_MESSAGE: {
        const DuiAppletCancelMessage *m = dynamic_cast<const DuiAppletCancelMessage *>(&message);
        if (m != NULL) {
            handleCancelEvent(m);
        }
        break;
    }

    case DuiAppletMessage::VISIBILITY_MESSAGE: {
        const DuiAppletVisibilityMessage *m = dynamic_cast<const DuiAppletVisibilityMessage *>(&message);
        if (m != NULL) {
            if (m->visible() != appletVisible) {
                emit visibilityChanged(m->visible());
                appletVisible = m->visible();
                if (appletVisible) {
                    QList<QRectF> updateRegion;
                    updateRegion.append(changedRect);
                    sceneChanged(updateRegion);
                }
            }
        }
        break;
    }

    case DuiAppletMessage::ORIENTATION_MESSAGE: {
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

    case DuiAppletMessage::SET_GEOMETRY_MESSAGE: {
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
            widget->update();
        }
        break;
    }

    case DuiAppletMessage::APPLET_ALIVE_MESSAGE_REQUEST: {
        const DuiAppletAliveMessageRequest *m = dynamic_cast<const DuiAppletAliveMessageRequest *>(&message);
        if (m != NULL) {
            communicator->sendMessage(DuiAppletAliveMessageResponse());
        }
        break;
    }

    case DuiAppletMessage::OBJECT_MENU_REQUEST_MESSAGE: {
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

    case DuiAppletMessage::OBJECT_MENU_ACTION_SELECTED_MESSAGE: {
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

void DuiAppletRunner::appletIconChanged(const QString &newIcon)
{
    DuiAppletIconChangedMessage m;
    m.setIcon(newIcon);
    communicator->sendMessage(m);
}

void DuiAppletRunner::appletTitleChanged(const QString &newTitle)
{
    DuiAppletTitleChangedMessage m;
    m.setTitle(newTitle);
    communicator->sendMessage(m);
}

void DuiAppletRunner::appletTextChanged(const QString &newText)
{
    DuiAppletTextChangedMessage m;
    m.setText(newText);
    communicator->sendMessage(m);
}

void DuiAppletRunner::handleMouseEvent(const DuiAppletMouseMessage *message)
{
    //convert our own event id to qt mouse event id
    QEvent::Type type = QEvent::MouseButtonPress;
    switch (message->type()) {
    case DuiAppletMessage::MOUSE_PRESS_MESSAGE:
        type = QEvent::MouseButtonPress;
        break;
    case DuiAppletMessage::MOUSE_RELEASE_MESSAGE:
        type = QEvent::MouseButtonRelease;
        break;
    case DuiAppletMessage::MOUSE_MOVE_MESSAGE:
        type = QEvent::MouseMove;
        break;
    default:
        break;
    }

    //create Qt mouse event and send it to the widget through viewport and scene
    //global position is used by the Qt to do some hit testing (don't know why?) so
    //it must provided also
    QPoint p(message->position().x(), message->position().y());
    QPoint p2(view->viewport()->pos() + p);
    QMouseEvent event(type, p, p2, message->button(), message->buttons(), 0);
    QCoreApplication::sendEvent(view->viewport(), &event);
}

void DuiAppletRunner::handleCancelEvent(const DuiAppletCancelMessage *message)
{
    Q_UNUSED(message);
    //We need to send the cancel event to the widget that is currently
    //grabbing the mouse, because this will be the widget that got the
    //mousePressEvent in previously received message.
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

void DuiAppletRunner::createAppletPixmap(Qt::HANDLE handle)
{
    // Create a QPixmap out of the pixmap handle
    appletPixmap = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);

#ifdef QT_OPENGL_LIB
    // Try to use opengl to render directly to the pixmap
    if (context != NULL)
        delete context;

    context = new QGLContext(QGLFormat::defaultFormat(), &appletPixmap);
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

void DuiAppletRunner::sceneChanged(const QList<QRectF> &region)
{
    if (!region.empty() && !appletPixmap.isNull()) {
        // Lock the pixmap mutex: this blocking call can't really fail in our case (see man pthread_mutex_lock) and even if it would there's not much that can be done about it
        if (pixmapMutex->lock()) {
            QPainter painter(&appletPixmap);

            foreach(const QRectF & r, region) {
                // Don't draw areas that are outside the pixmap
                QRectF drawRect = r.intersected(appletPixmap.rect());

                if (appletVisible && painter.isActive()) {
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

            if (appletVisible && painter.isActive()) {
                // Synchronize X to get the changes drawn to the pixmap
                QApplication::syncX();
            }

            // Unlock the pixmap mutex
            pixmapMutex->unlock();

            // Inform the host process about the union of the changed regions (if any)
            if (!changedRect.isEmpty() && appletVisible) {
                DuiAppletPixmapModifiedMessage m(changedRect);
                communicator->sendMessage(m);
                // Clear the changedRect, since it has been sent and applet is visible
                changedRect = QRectF();
            }
        }
    }
}

void DuiAppletRunner::hostProcessNotAlive()
{
    qApp->quit();
}

void DuiAppletRunner::sendUpdateGeometryMessage()
{
    // Send the new size hints along with the update geometry message. Only constraint -1,-1 is supported.
    QVector<QSizeF> sizeHints;
    for (int i = 0; i < Qt::NSizeHints; ++i) {
        sizeHints.append(widget->effectiveSizeHint((Qt::SizeHint)i, QSizeF(-1, -1)));
    }
    communicator->sendMessage(DuiAppletUpdateGeometryMessage(sizeHints));
}
