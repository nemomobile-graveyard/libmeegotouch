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

#ifndef DUIAPPLETHANDLE_STUB
#define DUIAPPLETHANDLE_STUB

#include "duiapplethandle.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletHandleStub : public StubBase
{
public:
    virtual void DuiAppletHandleConstructor(DuiAppletHandle *handle);
    virtual void DuiAppletHandleDestructor();
    virtual void init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId);
    virtual void initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError);
    virtual void kill();
    virtual void reinit();
    virtual void setAliveResponseTimeout(uint timeout);
    virtual void sendGeometryMessage(QRectF appletRect, Qt::HANDLE pixmapHandle);
    virtual void removeApplet();
    virtual void stopCommunication();
    virtual void setAppletSpecificActions(QList<QString> items);
    virtual DuiAppletHandleModel::AppletState state() const;
    virtual void setAppletIcon(const QString &newIcon);
    virtual void setAppletTitle(const QString &newTitle);
    virtual void setAppletText(const QString &newText);
    virtual QString appletIcon() const;
    virtual QString appletTitle() const;
    virtual QString appletText() const;
    virtual void setSizeHints(const QVector<QSizeF> &sizeHints);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    virtual void connectionEstablished();
    virtual void run();
    virtual void messageReceived(const DuiAppletMessage &message);
    virtual void communicationTimerTimeout();
    virtual void sendAliveMessageRequest();
    virtual void visibilityEvent(bool visible);
    virtual void orientationEvent(const Dui::Orientation &);
    virtual void processStdErrorReady();
    virtual void processStdOutputReady();
    virtual void processError(QProcess::ProcessError error);
    virtual void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    virtual void appletSpecificActionTriggered();
    virtual void appletRemovalRequested(DuiAppletId appletId);
    virtual void appletIconChanged(const QString &newIcon);
    virtual void appletTitleChanged(const QString &newTitle);
    virtual void appletTextChanged(const QString &newText);
    virtual void pixmapTakenIntoUse(Qt::HANDLE pixmapHandle);
    virtual void appletPixmapModified(const QRectF &rect);
    virtual void setAppletBrokenState();
    virtual void displayContextMenu(QList<QString> actions);
};

// 2. IMPLEMENT STUB
void DuiAppletHandleStub::DuiAppletHandleConstructor(DuiAppletHandle *handle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletHandle * >(handle));
    stubMethodEntered("constructor", params);
}
void DuiAppletHandleStub::DuiAppletHandleDestructor()
{
    stubMethodEntered("destructor");
}
void DuiAppletHandleStub::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(runnerBinary));
    params.append(new Parameter<const QString & >(appletInstanceFileDataPath));
    params.append(new Parameter<const QString & >(metaDataFileName));
    params.append(new Parameter<const DuiAppletId & >(appletId));
    stubMethodEntered("init", params);
}

void DuiAppletHandleStub::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletId & >(appletId));
    params.append(new Parameter<QString>(packageName));
    params.append(new Parameter<QString>(installationError));
    stubMethodEntered("initPlaceHolder", params);
}

void DuiAppletHandleStub::kill()
{
    stubMethodEntered("kill");
}

void DuiAppletHandleStub::reinit()
{
    stubMethodEntered("reinit");
}

void DuiAppletHandleStub::setAliveResponseTimeout(uint timeout)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(timeout));
    stubMethodEntered("setAliveResponseTimeout", params);
}

void DuiAppletHandleStub::sendGeometryMessage(QRectF appletRect, Qt::HANDLE pixmapHandle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QRectF >(appletRect));
    params.append(new Parameter<Qt::HANDLE >(pixmapHandle));
    stubMethodEntered("sendGeometryMessage", params);
}

void DuiAppletHandleStub::removeApplet()
{
    stubMethodEntered("removeApplet");
}

void DuiAppletHandleStub::stopCommunication()
{
    stubMethodEntered("stopCommunication");
}

void DuiAppletHandleStub::setAppletSpecificActions(QList<QString> items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<QString> >(items));
    stubMethodEntered("setAppletSpecificActions", params);
}

DuiAppletHandleModel::AppletState DuiAppletHandleStub::state() const
{
    stubMethodEntered("state");
    return stubReturnValue<DuiAppletHandleModel::AppletState>("state");
}

void DuiAppletHandleStub::setAppletIcon(const QString &newIcon)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(newIcon));
    stubMethodEntered("setAppletIcon", params);
}

void DuiAppletHandleStub::setAppletTitle(const QString &newTitle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString >(newTitle));
    stubMethodEntered("setAppletTitle", params);
}

void DuiAppletHandleStub::setAppletText(const QString &newText)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(newText));
    stubMethodEntered("setAppletText", params);
}

QString DuiAppletHandleStub::appletIcon() const
{
    stubMethodEntered("appletIcon");
    return stubReturnValue<QString>("appletIcon");
}

QString DuiAppletHandleStub::appletTitle() const
{
    stubMethodEntered("appletTitle");
    return stubReturnValue<QString>("appletTitle");
}

QString DuiAppletHandleStub::appletText() const
{
    stubMethodEntered("appletText");
    return stubReturnValue<QString>("appletText");
}

void DuiAppletHandleStub::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QVector<QSizeF> >(sizeHints));
    stubMethodEntered("setSizeHints", params);
}

void DuiAppletHandleStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiAppletHandleStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiAppletHandleStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseMoveEvent", params);
}

void DuiAppletHandleStub::cancelEvent(DuiCancelEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiCancelEvent * >(event));
    stubMethodEntered("cancelEvent", params);
}

void DuiAppletHandleStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneContextMenuEvent * >(event));
    stubMethodEntered("contextMenuEvent", params);
}

void DuiAppletHandleStub::enterDisplayEvent()
{
    stubMethodEntered("enterDisplayEvent");
}

void DuiAppletHandleStub::exitDisplayEvent()
{
    stubMethodEntered("exitDisplayEvent");
}

void DuiAppletHandleStub::connectionEstablished()
{
    stubMethodEntered("connectionEstablished");
}

void DuiAppletHandleStub::run()
{
    stubMethodEntered("run");
}

void DuiAppletHandleStub::messageReceived(const DuiAppletMessage &message)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiAppletMessage & >(message));
    stubMethodEntered("messageReceived", params);
}

void DuiAppletHandleStub::communicationTimerTimeout()
{
    stubMethodEntered("communicationTimerTimeout");
}

void DuiAppletHandleStub::sendAliveMessageRequest()
{
    stubMethodEntered("sendAliveMessageRequest");
}

void DuiAppletHandleStub::visibilityEvent(bool visible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(visible));
    stubMethodEntered("visibilityEvent", params);
}

void DuiAppletHandleStub::orientationEvent(const Dui::Orientation &orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Dui::Orientation & >(orientation));
    stubMethodEntered("orientationEvent", params);
}

void DuiAppletHandleStub::processStdErrorReady()
{
    stubMethodEntered("processStdErrorReady");
}

void DuiAppletHandleStub::processStdOutputReady()
{
    stubMethodEntered("processStdOutputReady");
}

void DuiAppletHandleStub::processError(QProcess::ProcessError error)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QProcess::ProcessError >(error));
    stubMethodEntered("processError", params);
}

void DuiAppletHandleStub::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(exitCode));
    params.append(new Parameter<QProcess::ExitStatus >(exitStatus));
    stubMethodEntered("processFinished", params);
}

void DuiAppletHandleStub::appletSpecificActionTriggered()
{
    stubMethodEntered("appletSpecificActionTriggered");
}

void DuiAppletHandleStub::appletRemovalRequested(DuiAppletId appletId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAppletId >(appletId));
    stubMethodEntered("appletRemovalRequested", params);
}

void DuiAppletHandleStub::appletIconChanged(const QString &newIcon)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(newIcon));
    stubMethodEntered("appletIconChanged", params);
}

void DuiAppletHandleStub::appletTitleChanged(const QString &newTitle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(newTitle));
    stubMethodEntered("appletTitleChanged", params);
}

void DuiAppletHandleStub::appletTextChanged(const QString &newText)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(newText));
    stubMethodEntered("appletTextChanged", params);
}

void DuiAppletHandleStub::pixmapTakenIntoUse(Qt::HANDLE pixmapHandle)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::HANDLE >(pixmapHandle));
    stubMethodEntered("pixmapTakenIntoUse", params);
}

void DuiAppletHandleStub::appletPixmapModified(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("appletPixmapModified", params);
}

void DuiAppletHandleStub::setAppletBrokenState()
{
    stubMethodEntered("setAppletBrokenState");
}

void DuiAppletHandleStub::displayContextMenu(QList<QString> actions)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<QString> >(actions));
    stubMethodEntered("displayContextMenu", params);
}



// 3. CREATE A STUB INSTANCE
DuiAppletHandleStub gDefaultDuiAppletHandleStub;
DuiAppletHandleStub *gDuiAppletHandleStub = &gDefaultDuiAppletHandleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletHandle::DuiAppletHandle()
{
    gDuiAppletHandleStub->DuiAppletHandleConstructor(this);
}

DuiAppletHandle::~DuiAppletHandle()
{
    gDuiAppletHandleStub->DuiAppletHandleDestructor();
}

void DuiAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId)
{
    gDuiAppletHandleStub->init(runnerBinary, appletInstanceFileDataPath, metaDataFileName, appletId);
}

void DuiAppletHandle::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError)
{
    gDuiAppletHandleStub->initPlaceHolder(appletId, packageName, installationError);
}

void DuiAppletHandle::kill()
{
    gDuiAppletHandleStub->kill();
}

void DuiAppletHandle::reinit()
{
    gDuiAppletHandleStub->reinit();
}

void DuiAppletHandle::setAliveResponseTimeout(uint timeout)
{
    gDuiAppletHandleStub->setAliveResponseTimeout(timeout);
}

void DuiAppletHandle::sendGeometryMessage(QRectF appletRect, Qt::HANDLE pixmapHandle)
{
    gDuiAppletHandleStub->sendGeometryMessage(appletRect, pixmapHandle);
}

void DuiAppletHandle::removeApplet()
{
    gDuiAppletHandleStub->removeApplet();
}

void DuiAppletHandle::stopCommunication()
{
    gDuiAppletHandleStub->stopCommunication();
}

void DuiAppletHandle::setAppletSpecificActions(QList<QString> items)
{
    gDuiAppletHandleStub->setAppletSpecificActions(items);
}

DuiAppletHandleModel::AppletState DuiAppletHandle::state() const
{
    return gDuiAppletHandleStub->state();
}

void DuiAppletHandle::setAppletIcon(const QString &newIcon)
{
    gDuiAppletHandleStub->setAppletIcon(newIcon);
}

void DuiAppletHandle::setAppletTitle(const QString &newTitle)
{
    gDuiAppletHandleStub->setAppletTitle(newTitle);
}

void DuiAppletHandle::setAppletText(const QString &newText)
{
    gDuiAppletHandleStub->setAppletText(newText);
}

QString DuiAppletHandle::appletIcon() const
{
    return gDuiAppletHandleStub->appletIcon();
}

QString DuiAppletHandle::appletTitle() const
{
    return gDuiAppletHandleStub->appletTitle();
}

QString DuiAppletHandle::appletText() const
{
    return gDuiAppletHandleStub->appletText();
}

void DuiAppletHandle::setSizeHints(const QVector<QSizeF> &sizeHints)
{
    gDuiAppletHandleStub->setSizeHints(sizeHints);
}

void DuiAppletHandle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiAppletHandleStub->mousePressEvent(event);
}

void DuiAppletHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiAppletHandleStub->mouseReleaseEvent(event);
}

void DuiAppletHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiAppletHandleStub->mouseMoveEvent(event);
}

void DuiAppletHandle::cancelEvent(DuiCancelEvent *event)
{
    gDuiAppletHandleStub->cancelEvent(event);
}

void DuiAppletHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    gDuiAppletHandleStub->contextMenuEvent(event);
}

void DuiAppletHandle::enterDisplayEvent()
{
    gDuiAppletHandleStub->enterDisplayEvent();
}

void DuiAppletHandle::exitDisplayEvent()
{
    gDuiAppletHandleStub->exitDisplayEvent();
}

void DuiAppletHandle::connectionEstablished()
{
    gDuiAppletHandleStub->connectionEstablished();
}

void DuiAppletHandle::run()
{
    gDuiAppletHandleStub->run();
}

void DuiAppletHandle::messageReceived(const DuiAppletMessage &message)
{
    gDuiAppletHandleStub->messageReceived(message);
}

void DuiAppletHandle::communicationTimerTimeout()
{
    gDuiAppletHandleStub->communicationTimerTimeout();
}

void DuiAppletHandle::sendAliveMessageRequest()
{
    gDuiAppletHandleStub->sendAliveMessageRequest();
}

void DuiAppletHandle::visibilityEvent(bool visible)
{
    gDuiAppletHandleStub->visibilityEvent(visible);
}

void DuiAppletHandle::orientationEvent(const Dui::Orientation &orientation)
{
    gDuiAppletHandleStub->orientationEvent(orientation);
}

void DuiAppletHandle::processStdErrorReady()
{
    gDuiAppletHandleStub->processStdErrorReady();
}

void DuiAppletHandle::processStdOutputReady()
{
    gDuiAppletHandleStub->processStdOutputReady();
}

void DuiAppletHandle::processError(QProcess::ProcessError error)
{
    gDuiAppletHandleStub->processError(error);
}

void DuiAppletHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    gDuiAppletHandleStub->processFinished(exitCode, exitStatus);
}

void DuiAppletHandle::appletSpecificActionTriggered()
{
    gDuiAppletHandleStub->appletSpecificActionTriggered();
}

void DuiAppletHandle::setAppletBrokenState()
{
    gDuiAppletHandleStub->setAppletBrokenState();
}

void DuiAppletHandle::displayContextMenu(QList<QString> actions)
{
    gDuiAppletHandleStub->displayContextMenu(actions);
}

#endif
