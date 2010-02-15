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

#include "duiapplethandle.h"
#include "duiapplethandle_p.h"
#include "duiapplethandlestyle.h"
#include "duiappletsharedmutex.h"
#include <duinamespace.h>
#include <duiapplication.h>
#include <duiwindow.h>
#include "duiscenemanager.h"
#include <duilocale.h>
#include <duiapplication.h>
#include <duiappletmousemessage.h>
#include <duiappletorientationmessage.h>
#include <duiappletvisibilitymessage.h>
#include <duiappletalivemessagerequest.h>
#include <duiappletalivemessageresponse.h>
#include <duiappletsetgeometrymessage.h>
#include <duiappletpixmaptakenintousemessage.h>
#include <duiappleticonchangedmessage.h>
#include <duiapplettitlechangedmessage.h>
#include <duiapplettextchangedmessage.h>
#include <duiappletpixmapmodifiedmessage.h>
#include <duiappletobjectmenumessage.h>
#include <duiappletobjectmenurequestmessage.h>
#include <duiappletobjectmenuactionselectedmessage.h>
#include <duisettingslanguageparser.h>
#include <duiappletupdategeometrymessage.h>
#include <duiappletsettings.h>
#include <duiappletmetadata.h>
#include <duiappletcancelmessage.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <DuiDebug>
#include <QFile>
#include <QLocalSocket>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>
#include <QDBusConnection>
#include <DuiAction>
#include <duiobjectmenu.h>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiAppletHandle)

//! D-Bus path of the Package Manager
const QString DuiAppletHandlePrivate::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";

//! D-Bus interface of the Package Manager
const QString DuiAppletHandlePrivate::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";

DuiAppletHandlePrivate::DuiAppletHandlePrivate() :
    restartCount(0),
    appletResponseTimeout(3000),
    runnerConnectionTimeout(20000),
    applicationVisible(true),
    widgetVisible(true),
    appletSpecificActions()
{
    communicationTimer.setSingleShot(true);

    // Connect to the system bus to receive the Package Manager signals
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationProgress", this, SLOT(operationProgress(QString, QString, int)));
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationComplete", this, SLOT(operationComplete(QString, QString, QString)));
}

DuiAppletHandlePrivate::~DuiAppletHandlePrivate()
{
}

void DuiAppletHandlePrivate::visibilityChanged()
{
    communicator.sendMessage(DuiAppletVisibilityMessage(applicationVisible && widgetVisible));
}

void DuiAppletHandlePrivate::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    Q_Q(DuiAppletHandle);

    if (operation == "Install" && q->model()->packageName() == pkg && !error.isEmpty()) {
        // If the installation of a package being installed fails go to the broken state
        q->model()->setInstallationError(error);
        q->model()->setState(DuiAppletHandleModel::BROKEN);
    }
}

void DuiAppletHandlePrivate::operationProgress(const QString &operation, const QString &pkg, int percentage)
{
    Q_Q(DuiAppletHandle);

    // Applet Instance Manager handles the situation when installation compeletes
    if (operation == "Install" && q->model()->packageName() == pkg) {
        q->model()->setInstallationProgress(percentage);
    }
}

DuiAppletHandle::DuiAppletHandle() :
    DuiWidgetController(new DuiAppletHandlePrivate, new DuiAppletHandleModel, NULL)
{
    Q_D(DuiAppletHandle);

#ifdef QT_OPENGL_ES_2
    if (!DuiApplication::softwareRendering()) {
        setViewType("gles");
    }
#endif

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active DuiWindow's scene.
    connect(DuiApplication::activeWindow(),
            SIGNAL(orientationChanged(Dui::Orientation)),
            this, SLOT(orientationEvent(Dui::Orientation)));

    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityEvent(bool)));

    // Configure the timers
    connect(&d->aliveTimer, SIGNAL(timeout()), this, SLOT(sendAliveMessageRequest()));
    connect(&d->communicationTimer, SIGNAL(timeout()), this, SLOT(communicationTimerTimeout()));
}

DuiAppletHandle::~DuiAppletHandle()
{
    kill();
}

void DuiAppletHandle::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError)
{
    model()->setAppletId(appletId);
    model()->setPackageName(packageName);
    if (installationError.isEmpty()) {
        model()->setState(DuiAppletHandleModel::INSTALLING);
    } else {
        model()->setState(DuiAppletHandleModel::BROKEN);
        model()->setInstallationError(installationError);
    }
}

void DuiAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId)
{
    Q_D(DuiAppletHandle);

    // Construct a server name
    // Hash this object's pointer so we don't reveal the real address. This
    // is a security protection mechanism against any kind of
    // hacks revealing the address might make possible.
    QByteArray const digest = QCryptographicHash::hash(QByteArray::number((quintptr)this), QCryptographicHash::Md5);
    d->serverName = QString::number(QCoreApplication::applicationPid()) + '_' + QString::fromLatin1(digest.toHex());

    // Store the arguments and the binary
    d->arguments.clear();
    d->arguments << appletId.toString() << appletInstanceFileDataPath << metaDataFileName << d->serverName;
    d->runnerBinary = runnerBinary;

    // Set the applet ID and applet base name
    DuiAppletMetaData metaData(metaDataFileName);
    model()->setAppletId(appletId);
    model()->setAppletBaseName(QFileInfo(metaDataFileName).baseName());

    // Create a new applet settings object and put it into the model
    model()->setAppletSettings(new DuiAppletSettings(metaDataFileName, appletId));

    // Start the applet process
    run();
}

void DuiAppletHandle::run()
{
    Q_D(DuiAppletHandle);

    // Create a shared pixmap mutex after destroying any previous mutex
    delete model()->pixmapMutex();
    DuiAppletSharedMutex *mutex = new DuiAppletSharedMutex;
    if (!mutex->init(d->serverName)) {
        model()->setState(DuiAppletHandleModel::BROKEN);
        return;
    }

    // Put it in the model so the view can use it
    model()->setPixmapMutex(mutex);

    // Set the state to applet starting
    model()->setState(DuiAppletHandleModel::STARTING);

    // Listen for connections from the runner
    connect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));

    // Initialize socket server
    if (!d->communicator.startServer(d->serverName)) {
        model()->setState(DuiAppletHandleModel::BROKEN);
        return;
    }

    // Connect a signal to recognize termination of the process
    connect(&d->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    // Connect a signal to recognize errors of the process
    connect(&d->process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));

    // Connect signals for receiving stdout and stderr data from the process
    connect(&d->process, SIGNAL(readyReadStandardOutput()), this, SLOT(processStdOutputReady()));
    connect(&d->process, SIGNAL(readyReadStandardError()), this, SLOT(processStdErrorReady()));

    d->process.start(d->runnerBinary, d->arguments);
    d->processStartTime.start();

#ifdef Q_OS_UNIX
    // Store the process pid as a property
    setProperty("pid", d->process.pid());
#endif

    // Create a timeout for establishing connection from runner to applet handle
    d->communicationTimer.start(d->runnerConnectionTimeout);
}

void DuiAppletHandle::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        setAppletBrokenState();
    }
}

void DuiAppletHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    setAppletBrokenState();
}

void DuiAppletHandle::connectionEstablished()
{
    Q_D(DuiAppletHandle);

    if (state() != DuiAppletHandleModel::STARTING) {
        qWarning() << "DuiAppletHandle::connectionEstablished() - Internal error, DuiAppletHandle state differs from DuiAppletHandleModel::STARTING";
        return;
    }

    // Stop runner connection timeout timer
    d->communicationTimer.stop();

    // Don't accept any more connections
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    // Listen for received messages
    connect(&d->communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));

    // Set the state to running
    model()->setState(DuiAppletHandleModel::RUNNING);

    // Send applet alive request every 5 seconds. Send the first alive request immediately.
    d->aliveTimer.start(5000);
    sendAliveMessageRequest();

    if (d->oldGeometry.isValid()) {
        // Restore the applet geometry
        setGeometry(d->oldGeometry);
    }

    // Ask the applet for the size hints
    updateGeometry();

    update();
}

void DuiAppletHandle::kill()
{
    Q_D(DuiAppletHandle);

    // Stop communication with the runner
    stopCommunication();

    // Stop the applet process
    d->process.disconnect();
    d->process.terminate();
    d->process.waitForFinished();

    // Set the state to stopped
    model()->setState(DuiAppletHandleModel::STOPPED);

    // Destroy the shared mutex
    delete model()->pixmapMutex();
    model()->setPixmapMutex(NULL);

    // Remove the applet settings
    delete model()->appletSettings();
    model()->setAppletSettings(NULL);
}

void DuiAppletHandle::reinit()
{
    Q_D(DuiAppletHandle);

    // Get the applet geometry
    d->oldGeometry = geometry();

    // Stop the current instance
    kill();

    // Restart the applet
    run();
}

void DuiAppletHandle::removeApplet()
{
    emit appletRemovalRequested(model()->appletId());
}

void DuiAppletHandle::setAliveResponseTimeout(uint timeout)
{
    Q_D(DuiAppletHandle);

    d->appletResponseTimeout = timeout;
}

void DuiAppletHandle::setAppletIcon(const QString &newIcon)
{
    model()->setAppletIcon(newIcon);
    emit appletIconChanged(newIcon);
}

void DuiAppletHandle::setAppletTitle(const QString &newTitle)
{
    model()->setAppletTitle(newTitle);
    emit appletTitleChanged(newTitle);
}

void DuiAppletHandle::setAppletText(const QString &newText)
{
    model()->setAppletText(newText);
    emit appletTextChanged(newText);
}

QString DuiAppletHandle::appletIcon() const
{
    return model()->appletIcon();
}

QString DuiAppletHandle::appletTitle() const
{
    return model()->appletTitle();
}

QString DuiAppletHandle::appletText() const
{
    return model()->appletText();
}

void DuiAppletHandle::messageReceived(const DuiAppletMessage &message)
{
    Q_D(DuiAppletHandle);

    switch (message.type()) {
    case DuiAppletMessage::PIXMAP_TAKEN_INTO_USE_MESSAGE: {
        const DuiAppletPixmapTakenIntoUseMessage *m = dynamic_cast<const DuiAppletPixmapTakenIntoUseMessage *>(&message);

        if (m != NULL) {
            emit pixmapTakenIntoUse(m->handle());
        }
        break;
    }

    case DuiAppletMessage::APPLET_ALIVE_MESSAGE_RESPONSE: {
        const DuiAppletAliveMessageResponse *m = dynamic_cast<const DuiAppletAliveMessageResponse *>(&message);
        if (m != NULL) {
            d->communicationTimer.stop();
        }
        break;
    }

    case DuiAppletMessage::APPLET_ICON_MESSAGE: {
        const DuiAppletIconChangedMessage *m = dynamic_cast<const DuiAppletIconChangedMessage *>(&message);
        if (m != NULL) {
            setAppletIcon(m->icon());
        }
        break;
    }

    case DuiAppletMessage::APPLET_TITLE_MESSAGE: {
        const DuiAppletTitleChangedMessage *m = dynamic_cast<const DuiAppletTitleChangedMessage *>(&message);
        if (m != NULL) {
            setAppletTitle(m->title());
        }
        break;
    }

    case DuiAppletMessage::APPLET_TEXT_MESSAGE: {
        const DuiAppletTextChangedMessage *m = dynamic_cast<const DuiAppletTextChangedMessage *>(&message);
        if (m != NULL) {
            setAppletText(m->text());
        }
        break;
    }

    case DuiAppletMessage::UPDATE_GEOMETRY_MESSAGE: {
        const DuiAppletUpdateGeometryMessage *m = dynamic_cast<const DuiAppletUpdateGeometryMessage *>(&message);
        if (m != NULL) {
            // Put the size hints in the model and update the geometry
            model()->setSizeHints(m->sizeHints());
            updateGeometry();
        }
        break;
    }

    case DuiAppletMessage::PIXMAP_MODIFIED_MESSAGE: {
        const DuiAppletPixmapModifiedMessage *m = dynamic_cast<const DuiAppletPixmapModifiedMessage *>(&message);
        if (m != NULL) {
            emit appletPixmapModified(m->geometry());
        }
        break;
    }

    case DuiAppletMessage::OBJECT_MENU_MESSAGE: {
        //Got the actions from the message. Now draw them.
        const DuiAppletObjectMenuMessage *m = dynamic_cast<const DuiAppletObjectMenuMessage *>(&message);
        if (m != NULL) {
            displayContextMenu(m->actionList());
        }
        break;
    }
    default:
        qWarning() << "IPC protocol error! Got message type:" << message.type();
        break;
    }
}

void DuiAppletHandle::displayContextMenu(QList<QString> actions)
{
    setAppletSpecificActions(actions);
    DuiObjectMenu *menu = new DuiObjectMenu(this);
    sceneManager()->showWindow(menu, DuiSceneWindow::DestroyWhenDone);
}

void DuiAppletHandle::stopCommunication()
{
    Q_D(DuiAppletHandle);

    disconnect(&d->communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    d->aliveTimer.stop();
    d->communicationTimer.stop();

    d->communicator.closeConnection();
}

void DuiAppletHandle::setAppletBrokenState()
{
    Q_D(DuiAppletHandle);

    model()->setState(DuiAppletHandleModel::BROKEN);
    stopCommunication();
    setAppletSpecificActions();

    if (d->processStartTime.elapsed() >= 30000) {
        d->restartCount = 0;
    }

    if (d->restartCount < 1) {
        d->restartCount++;
        reinit();
    }
}

void DuiAppletHandle::communicationTimerTimeout()
{
    setAppletBrokenState();
}

void DuiAppletHandle::sendAliveMessageRequest()
{
    Q_D(DuiAppletHandle);

    d->communicator.sendMessage(DuiAppletAliveMessageRequest());
    d->communicationTimer.start(d->appletResponseTimeout);
}

void DuiAppletHandle::visibilityEvent(bool visible)
{
    Q_D(DuiAppletHandle);

    d->widgetVisible = visible;
    d->visibilityChanged();
}

void DuiAppletHandle::enterDisplayEvent()
{
    Q_D(DuiAppletHandle);

    d->applicationVisible = true;
    d->visibilityChanged();
}

void DuiAppletHandle::exitDisplayEvent()
{
    Q_D(DuiAppletHandle);

    d->applicationVisible = false;
    d->visibilityChanged();
}

void DuiAppletHandle::orientationEvent(const Dui::Orientation &orientation)
{
    Q_D(DuiAppletHandle);

    d->communicator.sendMessage(DuiAppletOrientationMessage(orientation));
}

void DuiAppletHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiAppletHandle);

    // Scale the mouse events based on the current applet pixmap scaling factor
    d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MOUSE_PRESS_MESSAGE, event->pos() / model()->appletScale(), event->button(), event->buttons()));
}

void DuiAppletHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (state() == DuiAppletHandleModel::RUNNING) {
        Q_D(DuiAppletHandle);
        QPointF scaledPos = event->pos() / model()->appletScale();
        d->communicator.sendMessage(DuiAppletObjectMenuRequestMessage(scaledPos));
    } else {
        DuiWidgetController::contextMenuEvent(event);
    }
}

void DuiAppletHandle::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiAppletHandle);

    d->communicator.sendMessage(DuiAppletCancelMessage());
}

void DuiAppletHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state() == DuiAppletHandleModel::BROKEN) {
        DuiWidgetController::mouseReleaseEvent(event);
    } else {
        Q_D(DuiAppletHandle);

        // Scale the mouse events based on the current applet pixmap scaling factor
        d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MOUSE_RELEASE_MESSAGE, event->pos() / model()->appletScale(), event->button(), event->buttons()));
    }
}

void DuiAppletHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiAppletHandle);

    // Scale the mouse events based on the current applet pixmap scaling factor
    d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MOUSE_MOVE_MESSAGE, event->pos() / model()->appletScale(), event->button(), event->buttons()));
}

void DuiAppletHandle::sendGeometryMessage(QRectF appletRect, Qt::HANDLE pixmapHandle)
{
#ifndef Q_WS_X11
    Q_UNUSED(appletRect);
    Q_UNUSED(pixmapHandle);
#else
    Q_D(DuiAppletHandle);

    // Send the size the applet should know about and the X Pixmap handle to the applet
    d->communicator.sendMessage(DuiAppletSetGeometryMessage(appletRect, pixmapHandle));
#endif
}

void DuiAppletHandle::processStdOutputReady()
{
    Q_D(DuiAppletHandle);
    duiDebug("DuiAppletHandle") << "processStdOutputReady()\n   -------------->";
    duiDebug("") << qPrintable(QString(d->process.readAllStandardOutput()));
    duiDebug("") << " <--------------\n";
}

void DuiAppletHandle::processStdErrorReady()
{
    Q_D(DuiAppletHandle);
    duiDebug("DuiAppletHandle") << "processStdErrorReady()\n   -------------->";
    duiDebug("") << qPrintable(QString(d->process.readAllStandardError()));
    duiDebug("") << " <--------------\n";
}

void DuiAppletHandle::setAppletSpecificActions(QList<QString> items)
{
    Q_D(DuiAppletHandle);

    // Remove previous applet specific actions
    foreach(DuiAction * action, d->appletSpecificActions) {
        removeAction(action);
        delete action;
    }
    d->appletSpecificActions.clear();

    foreach(const QString & item, items) {
        DuiAction *action = new DuiAction(item, this);
        d->appletSpecificActions.append(action);
        connect(action, SIGNAL(triggered()), this, SLOT(appletSpecificActionTriggered()));
        addAction(action);
    }
}

void DuiAppletHandle::appletSpecificActionTriggered()
{
    Q_D(DuiAppletHandle);

    DuiAction *action = qobject_cast<DuiAction *>(sender());
    if (action != NULL) {
        int index = d->appletSpecificActions.indexOf(action);
        if (index >= 0) {
            d->communicator.sendMessage(DuiAppletObjectMenuActionSelectedMessage(index));
        }
    }
}

DuiAppletHandleModel::AppletState DuiAppletHandle::state() const
{
    return model()->state();
}

void DuiAppletHandle::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    model()->setSizeHints(sizeHints);
}

#include "moc_duiapplethandle.cpp"
