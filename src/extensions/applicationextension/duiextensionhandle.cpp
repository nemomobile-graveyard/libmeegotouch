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

#include "duiextensionhandle.h"
#include "duiextensionhandle_p.h"
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
DUI_REGISTER_WIDGET(DuiExtensionHandle)

//! D-Bus path of the Package Manager
const QString DuiExtensionHandlePrivate::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";

//! D-Bus interface of the Package Manager
const QString DuiExtensionHandlePrivate::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";

DuiExtensionHandlePrivate::DuiExtensionHandlePrivate() :
    restartCount(0),
    aliveResponseTimeout(3000),
    runnerConnectionTimeout(20000),
    applicationVisible(true),
    widgetVisible(true),
    remoteActions(),
    q_ptr(NULL)
{
    communicationTimer.setSingleShot(true);

    // Connect to the system bus to receive the Package Manager signals
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationProgress", this, SLOT(operationProgress(QString, QString, int)));
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationComplete", this, SLOT(operationComplete(QString, QString, QString)));
}

DuiExtensionHandlePrivate::~DuiExtensionHandlePrivate()
{
}

void DuiExtensionHandlePrivate::visibilityChanged()
{
    communicator.sendMessage(DuiAppletVisibilityMessage(applicationVisible && widgetVisible));
}

void DuiExtensionHandlePrivate::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    Q_Q(DuiExtensionHandle);

    if (operation == "Install" && q->model()->packageName() == pkg && !error.isEmpty()) {
        // If the installation of a package being installed fails go to the broken state
        q->model()->setInstallationError(error);
        q->model()->setCurrentState(DuiExtensionHandleModel::BROKEN);
    }
}

void DuiExtensionHandlePrivate::operationProgress(const QString &operation, const QString &pkg, int percentage)
{
    Q_Q(DuiExtensionHandle);

    // Applet Instance Manager handles the situation when installation compeletes
    if (operation == "Install" && q->model()->packageName() == pkg) {
        q->model()->setInstallationProgress(percentage);
    }
}

DuiExtensionHandle::DuiExtensionHandle(QGraphicsItem *parent) :
    DuiWidgetController(new DuiExtensionHandleModel, parent),
    d_ptr(new DuiExtensionHandlePrivate())
{
    Q_D(DuiExtensionHandle);

    if (d)
        d->q_ptr = this;

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

DuiExtensionHandle::DuiExtensionHandle(DuiExtensionHandlePrivate *dd, DuiExtensionHandleModel *model, QGraphicsItem *parent) :
    DuiWidgetController(model, parent),
    d_ptr(dd)
{
    Q_D(DuiExtensionHandle);
    if (d)
        d->q_ptr = this;

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

DuiExtensionHandle::~DuiExtensionHandle()
{
    kill();
}

void DuiExtensionHandle::initPlaceHolder(const QString &packageName, const QString &installationError)
{
    model()->setPackageName(packageName);
    if (installationError.isEmpty()) {
        model()->setCurrentState(DuiExtensionHandleModel::INSTALLING);
    } else {
        model()->setCurrentState(DuiExtensionHandleModel::BROKEN);
        model()->setInstallationError(installationError);
    }
}

void DuiExtensionHandle::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments)
{
    Q_D(DuiExtensionHandle);

    // Construct a server name
    // Hash this object's pointer so we don't reveal the real address. This
    // is a security protection mechanism against any kind of
    // hacks revealing the address might make possible.
    QByteArray const digest = QCryptographicHash::hash(QByteArray::number((quintptr)this), QCryptographicHash::Md5);
    d->serverName = QString::number(QCoreApplication::applicationPid()) + '_' + QString::fromLatin1(digest.toHex());

    // Store the arguments and the binary
    d->arguments.clear();
    d->arguments << d->serverName << metaDataFileName << extraArguments;
    d->runnerBinary = runnerBinary;

    // Set the base name
    model()->setBaseName(QFileInfo(metaDataFileName).baseName());

    // Start the runner process
    run();
}

void DuiExtensionHandle::run()
{
    Q_D(DuiExtensionHandle);

    // Create a shared pixmap mutex after destroying any previous mutex
    delete model()->pixmapMutex();
    DuiAppletSharedMutex *mutex = new DuiAppletSharedMutex;
    if (!mutex->init(d->serverName)) {
        model()->setCurrentState(DuiExtensionHandleModel::BROKEN);
        return;
    }

    // Put it in the model so the view can use it
    model()->setPixmapMutex(mutex);

    // Set the state to starting
    model()->setCurrentState(DuiExtensionHandleModel::STARTING);

    // Listen for connections from the runner
    connect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));

    // Initialize socket server
    if (!d->communicator.startServer(d->serverName)) {
        model()->setCurrentState(DuiExtensionHandleModel::BROKEN);
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

    // Create a timeout for establishing connection from the runner to the handle
    d->communicationTimer.start(d->runnerConnectionTimeout);
}

void DuiExtensionHandle::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        setBrokenState();
    }
}

void DuiExtensionHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    setBrokenState();
}

void DuiExtensionHandle::connectionEstablished()
{
    Q_D(DuiExtensionHandle);

    if (state() != DuiExtensionHandleModel::STARTING) {
        qWarning() << "DuiExtensionHandle::connectionEstablished() - Internal error, DuiExtensionHandle state differs from DuiExtensionHandleModel::STARTING";
        return;
    }

    // Stop runner connection timeout timer
    d->communicationTimer.stop();

    // Don't accept any more connections
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    // Listen for received messages
    connect(&d->communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));

    // Set the state to running
    model()->setCurrentState(DuiExtensionHandleModel::RUNNING);

    // Send alive request every 5 seconds. Send the first alive request immediately.
    d->aliveTimer.start(5000);
    sendAliveMessageRequest();

    if (d->oldGeometry.isValid()) {
        // Restore the old geometry
        setGeometry(d->oldGeometry);
    }

    // Ask the runner for the size hints
    updateGeometry();

    update();
}

void DuiExtensionHandle::kill()
{
    Q_D(DuiExtensionHandle);

    // Stop communication with the runner
    stopCommunication();

    // Stop the runner process
    d->process.disconnect();
    d->process.terminate();
    d->process.waitForFinished();

    // Set the state to stopped
    model()->setCurrentState(DuiExtensionHandleModel::STOPPED);

    // Destroy the shared mutex
    delete model()->pixmapMutex();
    model()->setPixmapMutex(NULL);
}

void DuiExtensionHandle::reinit()
{
    Q_D(DuiExtensionHandle);

    // Get the geometry
    d->oldGeometry = geometry();

    // Stop the current instance
    kill();

    // Restart the runner
    run();
}

void DuiExtensionHandle::setAliveResponseTimeout(uint timeout)
{
    Q_D(DuiExtensionHandle);

    d->aliveResponseTimeout = timeout;
}

void DuiExtensionHandle::messageReceived(const DuiAppletMessage &message)
{
    Q_D(DuiExtensionHandle);

    switch (message.type()) {
    case DuiAppletMessage::PixmapTakenIntoUseMessage: {
        const DuiAppletPixmapTakenIntoUseMessage *m = dynamic_cast<const DuiAppletPixmapTakenIntoUseMessage *>(&message);

        if (m != NULL) {
            emit pixmapTakenIntoUse(m->handle());
        }
        break;
    }

    case DuiAppletMessage::AppletAliveMessageResponse: {
        const DuiAppletAliveMessageResponse *m = dynamic_cast<const DuiAppletAliveMessageResponse *>(&message);
        if (m != NULL) {
            d->communicationTimer.stop();
        }
        break;
    }

    case DuiAppletMessage::UpdateGeometryMessage: {
        const DuiAppletUpdateGeometryMessage *m = dynamic_cast<const DuiAppletUpdateGeometryMessage *>(&message);
        if (m != NULL) {
            // Put the size hints in the model and update the geometry
            model()->setSizeHints(m->sizeHints());
            updateGeometry();
        }
        break;
    }

    case DuiAppletMessage::PixmapModifiedMessage: {
        const DuiAppletPixmapModifiedMessage *m = dynamic_cast<const DuiAppletPixmapModifiedMessage *>(&message);
        if (m != NULL) {
            emit pixmapModified(m->geometry());
        }
        break;
    }

    case DuiAppletMessage::ObjectMenuMessage: {
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

void DuiExtensionHandle::displayContextMenu(QList<QString> actions)
{
    setRemoteActions(actions);
    DuiObjectMenu *menu = new DuiObjectMenu(this);
    sceneManager()->appearSceneWindow(menu, DuiSceneWindow::DestroyWhenDone);
}

void DuiExtensionHandle::stopCommunication()
{
    Q_D(DuiExtensionHandle);

    disconnect(&d->communicator, SIGNAL(messageReceived(DuiAppletMessage)), this, SLOT(messageReceived(DuiAppletMessage)));
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    d->aliveTimer.stop();
    d->communicationTimer.stop();

    d->communicator.closeConnection();
}

void DuiExtensionHandle::setBrokenState()
{
    Q_D(DuiExtensionHandle);

    model()->setCurrentState(DuiExtensionHandleModel::BROKEN);
    stopCommunication();
    setRemoteActions();

    if (d->processStartTime.elapsed() >= 30000) {
        d->restartCount = 0;
    }

    if (d->restartCount < 1) {
        d->restartCount++;
        reinit();
    }
}

void DuiExtensionHandle::communicationTimerTimeout()
{
    setBrokenState();
}

void DuiExtensionHandle::sendAliveMessageRequest()
{
    Q_D(DuiExtensionHandle);

    d->communicator.sendMessage(DuiAppletAliveMessageRequest());
    d->communicationTimer.start(d->aliveResponseTimeout);
}

void DuiExtensionHandle::visibilityEvent(bool visible)
{
    Q_D(DuiExtensionHandle);

    d->widgetVisible = visible;
    d->visibilityChanged();
}

void DuiExtensionHandle::enterDisplayEvent()
{
    Q_D(DuiExtensionHandle);

    d->applicationVisible = true;
    d->visibilityChanged();
}

void DuiExtensionHandle::exitDisplayEvent()
{
    Q_D(DuiExtensionHandle);

    d->applicationVisible = false;
    d->visibilityChanged();
}

void DuiExtensionHandle::orientationEvent(Dui::Orientation orientation)
{
    Q_D(DuiExtensionHandle);

    d->communicator.sendMessage(DuiAppletOrientationMessage(orientation));
}

void DuiExtensionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiExtensionHandle);

    // Scale the mouse events based on the current pixmap scaling factor
    d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MousePressMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
}

void DuiExtensionHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (state() == DuiExtensionHandleModel::RUNNING) {
        Q_D(DuiExtensionHandle);
        QPointF scaledPos = event->pos() / model()->scale();
        d->communicator.sendMessage(DuiAppletObjectMenuRequestMessage(scaledPos));
    } else {
        DuiWidgetController::contextMenuEvent(event);
    }
}

void DuiExtensionHandle::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(DuiExtensionHandle);

    d->communicator.sendMessage(DuiAppletCancelMessage());
}

void DuiExtensionHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state() == DuiExtensionHandleModel::BROKEN) {
        DuiWidgetController::mouseReleaseEvent(event);
    } else {
        Q_D(DuiExtensionHandle);

        // Scale the mouse events based on the current pixmap scaling factor
        d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MouseReleaseMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
    }
}

void DuiExtensionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(DuiExtensionHandle);

    // Scale the mouse events based on the current pixmap scaling factor
    d->communicator.sendMessage(DuiAppletMouseMessage(DuiAppletMessage::MouseMoveMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
}

void DuiExtensionHandle::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle)
{
#ifndef Q_WS_X11
    Q_UNUSED(rect);
    Q_UNUSED(pixmapHandle);
#else
    Q_D(DuiExtensionHandle);

    // Send the size the extension should know about and the X Pixmap handle to the extension
    d->communicator.sendMessage(DuiAppletSetGeometryMessage(rect, pixmapHandle));
#endif
}

void DuiExtensionHandle::processStdOutputReady()
{
    Q_D(DuiExtensionHandle);
    duiDebug("DuiExtensionHandle") << "processStdOutputReady()\n   -------------->";
    duiDebug("") << qPrintable(QString(d->process.readAllStandardOutput()));
    duiDebug("") << " <--------------\n";
}

void DuiExtensionHandle::processStdErrorReady()
{
    Q_D(DuiExtensionHandle);
    duiDebug("DuiExtensionHandle") << "processStdErrorReady()\n   -------------->";
    duiDebug("") << qPrintable(QString(d->process.readAllStandardError()));
    duiDebug("") << " <--------------\n";
}

void DuiExtensionHandle::setRemoteActions(QList<QString> items)
{
    Q_D(DuiExtensionHandle);

    // Remove previous remote actions
    foreach(DuiAction * action, d->remoteActions) {
        removeAction(action);
        delete action;
    }
    d->remoteActions.clear();

    foreach(const QString & item, items) {
        DuiAction *action = new DuiAction(item, this);
        d->remoteActions.append(action);
        connect(action, SIGNAL(triggered()), this, SLOT(remoteActionTriggered()));
        addAction(action);
    }
}

void DuiExtensionHandle::remoteActionTriggered()
{
    Q_D(DuiExtensionHandle);

    DuiAction *action = qobject_cast<DuiAction *>(sender());
    if (action != NULL) {
        int index = d->remoteActions.indexOf(action);
        if (index >= 0) {
            d->communicator.sendMessage(DuiAppletObjectMenuActionSelectedMessage(index));
        }
    }
}

DuiExtensionHandleModel::State DuiExtensionHandle::state() const
{
    return model()->currentState();
}

void DuiExtensionHandle::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    model()->setSizeHints(sizeHints);
}

#include "moc_duiextensionhandle.cpp"
