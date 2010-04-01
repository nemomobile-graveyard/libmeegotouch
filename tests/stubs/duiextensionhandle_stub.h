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

#ifndef DUIEXTENSIONHANDLE_STUB
#define DUIEXTENSIONHANDLE_STUB

#include "duiextensionhandle.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiExtensionHandleStub : public StubBase {
  public:
  virtual void DuiExtensionHandleConstructor(QGraphicsItem *parent);
  virtual void DuiExtensionHandleDestructor();
  virtual void init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &);
  virtual void initPlaceHolder(const QString &packageName, const QString &installationError);
  virtual void kill();
  virtual void reinit();
  virtual void setAliveResponseTimeout(uint timeout);
  virtual void sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle);
  virtual void stopCommunication();
  virtual void setRemoteActions(QList<QString> items);
  virtual DuiExtensionHandleModel::State state() const;
  virtual void setSizeHints(const QVector<QSizeF> &sizeHints);
  virtual void DuiExtensionHandleConstructor(DuiExtensionHandlePrivate *dd, DuiExtensionHandleModel *model, QGraphicsItem *parent);
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
  virtual void remoteActionTriggered();
  virtual void setBrokenState();
  virtual void displayContextMenu(QList<QString> actions);
}; 

// 2. IMPLEMENT STUB
void DuiExtensionHandleStub::DuiExtensionHandleConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void DuiExtensionHandleStub::DuiExtensionHandleDestructor() {

}
void DuiExtensionHandleStub::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(runnerBinary));
  params.append( new Parameter<const QString & >(metaDataFileName));
  params.append( new Parameter<const QStringList & >(extraArguments));
  stubMethodEntered("init",params);
}

void DuiExtensionHandleStub::initPlaceHolder(const QString &packageName, const QString &installationError) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(installationError));
  stubMethodEntered("initPlaceHolder",params);
}

void DuiExtensionHandleStub::kill() {
  stubMethodEntered("kill");
}

void DuiExtensionHandleStub::reinit() {
  stubMethodEntered("reinit");
}

void DuiExtensionHandleStub::setAliveResponseTimeout(uint timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(timeout));
  stubMethodEntered("setAliveResponseTimeout",params);
}

void DuiExtensionHandleStub::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QRectF >(rect));
  params.append( new Parameter<Qt::HANDLE >(pixmapHandle));
  stubMethodEntered("sendGeometryMessage",params);
}

void DuiExtensionHandleStub::stopCommunication() {
  stubMethodEntered("stopCommunication");
}

void DuiExtensionHandleStub::setRemoteActions(QList<QString> items) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QString> >(items));
  stubMethodEntered("setRemoteActions",params);
}

DuiExtensionHandleModel::State DuiExtensionHandleStub::state() const {
  stubMethodEntered("state");
  return stubReturnValue<DuiExtensionHandleModel::State>("state");
}

void DuiExtensionHandleStub::setSizeHints(const QVector<QSizeF> &sizeHints) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QVector<QSizeF> >(sizeHints));
  stubMethodEntered("setSizeHints",params);
}

void DuiExtensionHandleStub::DuiExtensionHandleConstructor(DuiExtensionHandlePrivate *dd, DuiExtensionHandleModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
void DuiExtensionHandleStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void DuiExtensionHandleStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void DuiExtensionHandleStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void DuiExtensionHandleStub::cancelEvent(DuiCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<DuiCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void DuiExtensionHandleStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneContextMenuEvent * >(event));
  stubMethodEntered("contextMenuEvent",params);
}

void DuiExtensionHandleStub::enterDisplayEvent() {
  stubMethodEntered("enterDisplayEvent");
}

void DuiExtensionHandleStub::exitDisplayEvent() {
  stubMethodEntered("exitDisplayEvent");
}

void DuiExtensionHandleStub::connectionEstablished() {
  stubMethodEntered("connectionEstablished");
}

void DuiExtensionHandleStub::run() {
  stubMethodEntered("run");
}

void DuiExtensionHandleStub::messageReceived(const DuiAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}

void DuiExtensionHandleStub::communicationTimerTimeout() {
  stubMethodEntered("communicationTimerTimeout");
}

void DuiExtensionHandleStub::sendAliveMessageRequest() {
  stubMethodEntered("sendAliveMessageRequest");
}

void DuiExtensionHandleStub::visibilityEvent(bool visible) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(visible));
  stubMethodEntered("visibilityEvent",params);
}

void DuiExtensionHandleStub::orientationEvent(const Dui::Orientation &orientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const Dui::Orientation & >(orientation));
  stubMethodEntered("orientationEvent",params);
}

void DuiExtensionHandleStub::processStdErrorReady() {
  stubMethodEntered("processStdErrorReady");
}

void DuiExtensionHandleStub::processStdOutputReady() {
  stubMethodEntered("processStdOutputReady");
}

void DuiExtensionHandleStub::processError(QProcess::ProcessError error) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QProcess::ProcessError >(error));
  stubMethodEntered("processError",params);
}

void DuiExtensionHandleStub::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(exitCode));
  params.append( new Parameter<QProcess::ExitStatus >(exitStatus));
  stubMethodEntered("processFinished",params);
}

void DuiExtensionHandleStub::remoteActionTriggered() {
  stubMethodEntered("remoteActionTriggered");
}

void DuiExtensionHandleStub::setBrokenState() {
  stubMethodEntered("setBrokenState");
}

void DuiExtensionHandleStub::displayContextMenu(QList<QString> actions) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QString> >(actions));
  stubMethodEntered("displayContextMenu",params);
}



// 3. CREATE A STUB INSTANCE
DuiExtensionHandleStub gDefaultDuiExtensionHandleStub;
DuiExtensionHandleStub* gDuiExtensionHandleStub = &gDefaultDuiExtensionHandleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiExtensionHandle::DuiExtensionHandle(QGraphicsItem *parent) : d_ptr(NULL) {
  gDuiExtensionHandleStub->DuiExtensionHandleConstructor(parent);
}

DuiExtensionHandle::~DuiExtensionHandle() {
  gDuiExtensionHandleStub->DuiExtensionHandleDestructor();
}

void DuiExtensionHandle::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments) {
  gDuiExtensionHandleStub->init(runnerBinary, metaDataFileName, extraArguments);
}

void DuiExtensionHandle::initPlaceHolder(const QString &packageName, const QString &installationError) {
  gDuiExtensionHandleStub->initPlaceHolder(packageName, installationError);
}

void DuiExtensionHandle::kill() {
  gDuiExtensionHandleStub->kill();
}

void DuiExtensionHandle::reinit() {
  gDuiExtensionHandleStub->reinit();
}

void DuiExtensionHandle::setAliveResponseTimeout(uint timeout) {
  gDuiExtensionHandleStub->setAliveResponseTimeout(timeout);
}

void DuiExtensionHandle::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle) {
  gDuiExtensionHandleStub->sendGeometryMessage(rect, pixmapHandle);
}

void DuiExtensionHandle::stopCommunication() {
  gDuiExtensionHandleStub->stopCommunication();
}

void DuiExtensionHandle::setRemoteActions(QList<QString> items) {
  gDuiExtensionHandleStub->setRemoteActions(items);
}

DuiExtensionHandleModel::State DuiExtensionHandle::state() const {
  return gDuiExtensionHandleStub->state();
}

void DuiExtensionHandle::setSizeHints(const QVector<QSizeF> &sizeHints) {
  gDuiExtensionHandleStub->setSizeHints(sizeHints);
}

DuiExtensionHandle::DuiExtensionHandle(DuiExtensionHandlePrivate *dd, DuiExtensionHandleModel *model, QGraphicsItem *parent) : d_ptr(NULL)  {
  gDuiExtensionHandleStub->DuiExtensionHandleConstructor(dd, model, parent);
}

void DuiExtensionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gDuiExtensionHandleStub->mousePressEvent(event);
}

void DuiExtensionHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gDuiExtensionHandleStub->mouseReleaseEvent(event);
}

void DuiExtensionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gDuiExtensionHandleStub->mouseMoveEvent(event);
}

void DuiExtensionHandle::cancelEvent(DuiCancelEvent *event) {
  gDuiExtensionHandleStub->cancelEvent(event);
}

void DuiExtensionHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  gDuiExtensionHandleStub->contextMenuEvent(event);
}

void DuiExtensionHandle::enterDisplayEvent() {
  gDuiExtensionHandleStub->enterDisplayEvent();
}

void DuiExtensionHandle::exitDisplayEvent() {
  gDuiExtensionHandleStub->exitDisplayEvent();
}

void DuiExtensionHandle::connectionEstablished() {
  gDuiExtensionHandleStub->connectionEstablished();
}

void DuiExtensionHandle::run() {
  gDuiExtensionHandleStub->run();
}

void DuiExtensionHandle::messageReceived(const DuiAppletMessage &message) {
  gDuiExtensionHandleStub->messageReceived(message);
}

void DuiExtensionHandle::communicationTimerTimeout() {
  gDuiExtensionHandleStub->communicationTimerTimeout();
}

void DuiExtensionHandle::sendAliveMessageRequest() {
  gDuiExtensionHandleStub->sendAliveMessageRequest();
}

void DuiExtensionHandle::visibilityEvent(bool visible) {
  gDuiExtensionHandleStub->visibilityEvent(visible);
}

void DuiExtensionHandle::orientationEvent(Dui::Orientation orientation) {
  gDuiExtensionHandleStub->orientationEvent(orientation);
}

void DuiExtensionHandle::processStdErrorReady() {
  gDuiExtensionHandleStub->processStdErrorReady();
}

void DuiExtensionHandle::processStdOutputReady() {
  gDuiExtensionHandleStub->processStdOutputReady();
}

void DuiExtensionHandle::processError(QProcess::ProcessError error) {
  gDuiExtensionHandleStub->processError(error);
}

void DuiExtensionHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  gDuiExtensionHandleStub->processFinished(exitCode, exitStatus);
}

void DuiExtensionHandle::remoteActionTriggered() {
  gDuiExtensionHandleStub->remoteActionTriggered();
}

void DuiExtensionHandle::setBrokenState() {
  gDuiExtensionHandleStub->setBrokenState();
}

void DuiExtensionHandle::displayContextMenu(QList<QString> actions) {
  gDuiExtensionHandleStub->displayContextMenu(actions);
}


#endif
