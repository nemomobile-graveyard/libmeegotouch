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

#include "mextensionhandle.h"
#include "mextensionhandle_p.h"
#include "mappletsharedmutex.h"
#include <mnamespace.h>
#include <mapplication.h>
#include <mwindow.h>
#include "mscenemanager.h"
#include <mlocale.h>
#include <mapplication.h>
#include <mappletmousemessage.h>
#include <mappletorientationmessage.h>
#include <mappletvisibilitymessage.h>
#include <mappletalivemessagerequest.h>
#include <mappletalivemessageresponse.h>
#include <mappletsetgeometrymessage.h>
#include <mappletpixmaptakenintousemessage.h>
#include <mappletpixmapmodifiedmessage.h>
#include <mappletobjectmenumessage.h>
#include <mappletobjectmenurequestmessage.h>
#include <mappletobjectmenuactionselectedmessage.h>
#include <msettingslanguageparser.h>
#include <mappletupdategeometrymessage.h>
#include <mappletsettings.h>
#include <mappletmetadata.h>
#include <mappletcancelmessage.h>

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <MDebug>
#include <QFile>
#include <QLocalSocket>
#include <QByteArray>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>
#include <QDBusConnection>
#include <MAction>
#include <mobjectmenu.h>

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MExtensionHandle)

//! D-Bus path of the Package Manager
const QString MExtensionHandlePrivate::PACKAGE_MANAGER_DBUS_PATH = "/com/nokia/package_manager";

//! D-Bus interface of the Package Manager
const QString MExtensionHandlePrivate::PACKAGE_MANAGER_DBUS_INTERFACE = "com.nokia.package_manager";

//! Interval for checking that the extension is alive
static const uint ALIVE_TIMER_INTERVAL = 5000;

MExtensionHandlePrivate::MExtensionHandlePrivate() :
    restartCount(0),
    aliveResponseTimeout(3000),
    runnerConnectionTimeout(20000),
    applicationVisible(true),
    remoteActions(),
#ifdef HAVE_CONTEXTSUBSCRIBER
    screenBlankProperty(new ContextProperty("Session.State", this)),
#endif
    q_ptr(NULL)
{
    communicationTimer.setSingleShot(true);

    // Connect to the system bus to receive the Package Manager signals
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationProgress", this, SLOT(operationProgress(QString, QString, int)));
    QDBusConnection::systemBus().connect(QString(), PACKAGE_MANAGER_DBUS_PATH, PACKAGE_MANAGER_DBUS_INTERFACE, "OperationComplete", this, SLOT(operationComplete(QString, QString, QString)));
}

MExtensionHandlePrivate::~MExtensionHandlePrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete screenBlankProperty;
#endif
}

void MExtensionHandlePrivate::visibilityChanged()
{
    Q_Q(MExtensionHandle);
    communicator.sendMessage(MAppletVisibilityMessage(q->isVisible() && applicationVisible));
}

void MExtensionHandlePrivate::operationComplete(const QString &operation, const QString &pkg, const QString &error)
{
    Q_Q(MExtensionHandle);

    if (operation == "Install" && q->model()->packageName() == pkg && !error.isEmpty()) {
        // If the installation of a package being installed fails go to the broken state
        q->model()->setInstallationError(error);
        q->model()->setCurrentState(MExtensionHandleModel::BROKEN);
    }
}

void MExtensionHandlePrivate::operationProgress(const QString &operation, const QString &pkg, int percentage)
{
    Q_Q(MExtensionHandle);

    // Applet Instance Manager handles the situation when installation compeletes
    if (operation == "Install" && q->model()->packageName() == pkg) {
        q->model()->setInstallationProgress(percentage);
    }
}

MExtensionHandle::MExtensionHandle(QGraphicsItem *parent) :
    MWidgetController(new MExtensionHandleModel, parent),
    d_ptr(new MExtensionHandlePrivate())
{
    initialize();
}

MExtensionHandle::MExtensionHandle(MExtensionHandlePrivate *dd, MExtensionHandleModel *model, QGraphicsItem *parent) :
    MWidgetController(model, parent),
    d_ptr(dd)
{
    initialize();
}

void MExtensionHandle::initialize()
{
    Q_D(MExtensionHandle);

    if (d)
        d->q_ptr = this;

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active MWindow's scene.
    connect(MApplication::activeWindow(),
            SIGNAL(orientationChanged(M::Orientation)),
            this, SLOT(orientationEvent(M::Orientation)));

    connect(this, SIGNAL(visibleChanged()), this, SLOT(visibilityChanged()));

    // Configure the timers
    connect(&d->aliveTimer, SIGNAL(timeout()), this, SLOT(sendAliveMessageRequest()));
    connect(&d->communicationTimer, SIGNAL(timeout()), this, SLOT(communicationTimerTimeout()));

#ifdef HAVE_CONTEXTSUBSCRIBER
    connect(d->screenBlankProperty, SIGNAL(valueChanged()), this, SLOT(updateDisplayState()));
#endif
}

MExtensionHandle::~MExtensionHandle()
{
    kill();

    delete d_ptr;
}

void MExtensionHandle::updateDisplayState()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_D(MExtensionHandle);

    if(state() == MExtensionHandleModel::RUNNING) {
        bool blanked = d->screenBlankProperty->value().toString() == "blanked";

        if(blanked && d->aliveTimer.isActive()) {
            d->aliveTimer.stop();
            d->communicationTimer.stop();
        } else if(!d->aliveTimer.isActive()) {
            d->aliveTimer.start(ALIVE_TIMER_INTERVAL);
            sendAliveMessageRequest();
        }
    }
#endif
}

void MExtensionHandle::initPlaceHolder(const QString &packageName, const QString &installationError)
{
    model()->setPackageName(packageName);
    if (installationError.isEmpty()) {
        model()->setCurrentState(MExtensionHandleModel::INSTALLING);
    } else {
        model()->setCurrentState(MExtensionHandleModel::BROKEN);
        model()->setInstallationError(installationError);
    }
}

void MExtensionHandle::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments)
{
    Q_D(MExtensionHandle);

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

void MExtensionHandle::run()
{
    Q_D(MExtensionHandle);

    // Create a shared pixmap mutex after destroying any previous mutex
    delete model()->pixmapMutex();
    MAppletSharedMutex *mutex = new MAppletSharedMutex;
    if (!mutex->init(d->serverName)) {
        model()->setCurrentState(MExtensionHandleModel::BROKEN);
        delete mutex;
        mutex = NULL;
        return;
    }

    // Put it in the model so the view can use it
    model()->setPixmapMutex(mutex);

    // Set the state to starting
    model()->setCurrentState(MExtensionHandleModel::STARTING);

    // Listen for connections from the runner
    connect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));

    // Initialize socket server
    if (!d->communicator.startServer(d->serverName)) {
        model()->setCurrentState(MExtensionHandleModel::BROKEN);
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

void MExtensionHandle::processError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart) {
        setBrokenState();
    }
}

void MExtensionHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    setBrokenState();
}

void MExtensionHandle::connectionEstablished()
{
    Q_D(MExtensionHandle);

    if (state() != MExtensionHandleModel::STARTING) {
        mWarning("MExtensionHandle") << "Internal error: MExtensionHandle state differs from MExtensionHandleModel::STARTING";
        return;
    }

    // Stop runner connection timeout timer
    d->communicationTimer.stop();

    // Don't accept any more connections
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    // Listen for received messages
    connect(&d->communicator, SIGNAL(messageReceived(MAppletMessage)), this, SLOT(messageReceived(MAppletMessage)));

    // Set the state to running
    model()->setCurrentState(MExtensionHandleModel::RUNNING);

    // Send alive requests at certain interval. Send the first alive request immediately.
    d->aliveTimer.start(ALIVE_TIMER_INTERVAL);
    sendAliveMessageRequest();

    // Set the current display state
    updateDisplayState();

    // Send the visibility changed message to the runner
    d->visibilityChanged();

    if (d->oldGeometry.isValid()) {
        // Restore the old geometry
        setGeometry(d->oldGeometry);
    }

    // Ask the runner for the size hints
    updateGeometry();

    update();
}

void MExtensionHandle::kill()
{
    Q_D(MExtensionHandle);

    // Stop communication with the runner
    stopCommunication();

    // Stop the runner process
    d->process.disconnect();
    d->process.terminate();
    d->process.waitForFinished();

    // Set the state to stopped
    model()->setCurrentState(MExtensionHandleModel::STOPPED);

    // Destroy the shared mutex
    delete model()->pixmapMutex();
    model()->setPixmapMutex(NULL);
}

void MExtensionHandle::reinit()
{
    Q_D(MExtensionHandle);

    // Get the geometry
    d->oldGeometry = geometry();

    // Stop the current instance
    kill();

    // Restart the runner
    run();
}

void MExtensionHandle::setAliveResponseTimeout(uint timeout)
{
    Q_D(MExtensionHandle);

    d->aliveResponseTimeout = timeout;
}

void MExtensionHandle::messageReceived(const MAppletMessage &message)
{
    Q_D(MExtensionHandle);

    switch (message.type()) {
    case MAppletMessage::PixmapTakenIntoUseMessage: {
        const MAppletPixmapTakenIntoUseMessage *m = dynamic_cast<const MAppletPixmapTakenIntoUseMessage *>(&message);

        if (m != NULL) {
            emit pixmapTakenIntoUse(m->handle());
        }
        break;
    }

    case MAppletMessage::AppletAliveMessageResponse: {
        const MAppletAliveMessageResponse *m = dynamic_cast<const MAppletAliveMessageResponse *>(&message);
        if (m != NULL) {
            d->communicationTimer.stop();
        }
        break;
    }

    case MAppletMessage::UpdateGeometryMessage: {
        const MAppletUpdateGeometryMessage *m = dynamic_cast<const MAppletUpdateGeometryMessage *>(&message);
        if (m != NULL) {
            // Put the size hints in the model and update the geometry
            model()->setSizeHints(m->sizeHints());
            updateGeometry();
        }
        break;
    }

    case MAppletMessage::PixmapModifiedMessage: {
        const MAppletPixmapModifiedMessage *m = dynamic_cast<const MAppletPixmapModifiedMessage *>(&message);
        if (m != NULL) {
            emit pixmapModified(m->geometry());
        }
        break;
    }

    case MAppletMessage::ObjectMenuMessage: {
        //Got the actions from the message. Now draw them.
        const MAppletObjectMenuMessage *m = dynamic_cast<const MAppletObjectMenuMessage *>(&message);
        if (m != NULL) {
            displayContextMenu(m->actionList());
        }
        break;
    }
    default:
        mWarning("MExtensionHandle") << "IPC protocol error! Got message type:" << message.type();
        break;
    }
}

void MExtensionHandle::displayContextMenu(QList<QString> actions)
{
    setRemoteActions(actions);
    MObjectMenu *menu = new MObjectMenu(this);
    sceneManager()->appearSceneWindow(menu, MSceneWindow::DestroyWhenDone);
}

void MExtensionHandle::stopCommunication()
{
    Q_D(MExtensionHandle);

    disconnect(&d->communicator, SIGNAL(messageReceived(MAppletMessage)), this, SLOT(messageReceived(MAppletMessage)));
    disconnect(&d->communicator, SIGNAL(connectionEstablished()), this, SLOT(connectionEstablished()));
    d->aliveTimer.stop();
    d->communicationTimer.stop();

    d->communicator.closeConnection();
}

void MExtensionHandle::setBrokenState()
{
    Q_D(MExtensionHandle);

    model()->setCurrentState(MExtensionHandleModel::BROKEN);
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

void MExtensionHandle::communicationTimerTimeout()
{
    setBrokenState();
}

void MExtensionHandle::sendAliveMessageRequest()
{
    Q_D(MExtensionHandle);

    d->communicator.sendMessage(MAppletAliveMessageRequest());
    d->communicationTimer.start(d->aliveResponseTimeout);
}

void MExtensionHandle::visibilityChanged()
{
    Q_D(MExtensionHandle);
    d->visibilityChanged();
}

//TODO: Remove this redundant slot in next API/ABI break window.
void MExtensionHandle::visibilityEvent(bool visible)
{
    Q_UNUSED(visible);
}

void MExtensionHandle::enterDisplayEvent()
{
    Q_D(MExtensionHandle);

    d->applicationVisible = true;
    d->visibilityChanged();
}

void MExtensionHandle::exitDisplayEvent()
{
    Q_D(MExtensionHandle);

    d->applicationVisible = false;
    d->visibilityChanged();
}

void MExtensionHandle::orientationEvent(M::Orientation orientation)
{
    Q_D(MExtensionHandle);

    d->communicator.sendMessage(MAppletOrientationMessage(orientation));
}

void MExtensionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MExtensionHandle);

    // Scale the mouse events based on the current pixmap scaling factor
    d->communicator.sendMessage(MAppletMouseMessage(MAppletMessage::MousePressMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
}

void MExtensionHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (state() == MExtensionHandleModel::RUNNING) {
        Q_D(MExtensionHandle);
        QPointF scaledPos = event->pos() / model()->scale();
        d->communicator.sendMessage(MAppletObjectMenuRequestMessage(scaledPos));
    } else {
        MWidgetController::contextMenuEvent(event);
    }
}

void MExtensionHandle::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    Q_D(MExtensionHandle);

    d->communicator.sendMessage(MAppletCancelMessage());
}

void MExtensionHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (state() == MExtensionHandleModel::BROKEN) {
        MWidgetController::mouseReleaseEvent(event);
    } else {
        Q_D(MExtensionHandle);

        // Scale the mouse events based on the current pixmap scaling factor
        d->communicator.sendMessage(MAppletMouseMessage(MAppletMessage::MouseReleaseMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
    }
}

void MExtensionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_D(MExtensionHandle);

    // Scale the mouse events based on the current pixmap scaling factor
    d->communicator.sendMessage(MAppletMouseMessage(MAppletMessage::MouseMoveMessage, event->pos() / model()->scale(), event->button(), event->buttons()));
}

void MExtensionHandle::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle)
{
#ifndef Q_WS_X11
    Q_UNUSED(rect);
    Q_UNUSED(pixmapHandle);
#else
    Q_D(MExtensionHandle);

    // Send the size the extension should know about and the X Pixmap handle to the extension
    d->communicator.sendMessage(MAppletSetGeometryMessage(rect, pixmapHandle));
#endif
}

void MExtensionHandle::processStdOutputReady()
{
    Q_D(MExtensionHandle);
    mDebug("MExtensionHandle") << "processStdOutputReady()\n   -------------->";
    mDebug("") << qPrintable(QString(d->process.readAllStandardOutput()));
    mDebug("") << " <--------------\n";
}

void MExtensionHandle::processStdErrorReady()
{
    Q_D(MExtensionHandle);
    mDebug("MExtensionHandle") << "processStdErrorReady()\n   -------------->";
    mDebug("") << qPrintable(QString(d->process.readAllStandardError()));
    mDebug("") << " <--------------\n";
}

void MExtensionHandle::setRemoteActions(QList<QString> items)
{
    Q_D(MExtensionHandle);

    // Remove previous remote actions
    foreach(MAction * action, d->remoteActions) {
        removeAction(action);
        delete action;
    }
    d->remoteActions.clear();

    foreach(const QString & item, items) {
        MAction *action = new MAction(item, this);
        d->remoteActions.append(action);
        connect(action, SIGNAL(triggered()), this, SLOT(remoteActionTriggered()));
        addAction(action);
    }
}

void MExtensionHandle::remoteActionTriggered()
{
    Q_D(MExtensionHandle);

    MAction *action = qobject_cast<MAction *>(sender());
    if (action != NULL) {
        int index = d->remoteActions.indexOf(action);
        if (index >= 0) {
            d->communicator.sendMessage(MAppletObjectMenuActionSelectedMessage(index));
        }
    }
}

MExtensionHandleModel::State MExtensionHandle::state() const
{
    return model()->currentState();
}

void MExtensionHandle::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    model()->setSizeHints(sizeHints);
}

#include "moc_mextensionhandle.cpp"
