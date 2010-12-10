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

#ifndef MEXTENSIONHANDLE_STUB
#define MEXTENSIONHANDLE_STUB

#include "mextensionhandle.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MExtensionHandleStub : public StubBase {
  public:
  virtual void MExtensionHandleConstructor(QGraphicsItem *parent);
  virtual void MExtensionHandleDestructor();
  virtual void init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &);
  virtual void initPlaceHolder(const QString &packageName, const QString &installationError);
  virtual void kill();
  virtual void reinit();
  virtual void setAliveResponseTimeout(uint timeout);
  virtual void sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle);
  virtual void stopCommunication();
  virtual void setRemoteActions(QList<QString> items);
  virtual MExtensionHandleModel::State state() const;
  virtual void setSizeHints(const QVector<QSizeF> &sizeHints);
  virtual void MExtensionHandleConstructor(MExtensionHandlePrivate *dd, MExtensionHandleModel *model, QGraphicsItem *parent);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void cancelEvent(MCancelEvent *event);
  virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
  virtual void enterDisplayEvent();
  virtual void exitDisplayEvent();
  virtual void connectionEstablished();
  virtual void run();
  virtual void messageReceived(const MAppletMessage &message);
  virtual void communicationTimerTimeout();
  virtual void sendAliveMessageRequest();
  virtual void visibilityEvent(bool visible);
  virtual void visibilityChanged();
  virtual void orientationEvent(const M::Orientation &);
  virtual void processStdErrorReady();
  virtual void processStdOutputReady();
  virtual void processError(QProcess::ProcessError error);
  virtual void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
  virtual void remoteActionTriggered();
  virtual void setBrokenState();
  virtual void displayContextMenu(QList<QString> actions);
  virtual void updateDisplayState();
};

// 2. IMPLEMENT STUB
void MExtensionHandleStub::MExtensionHandleConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void MExtensionHandleStub::MExtensionHandleDestructor() {

}
void MExtensionHandleStub::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(runnerBinary));
  params.append( new Parameter<const QString & >(metaDataFileName));
  params.append( new Parameter<const QStringList & >(extraArguments));
  stubMethodEntered("init",params);
}
void MExtensionHandleStub::initPlaceHolder(const QString &packageName, const QString &installationError) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(packageName));
  params.append( new Parameter<const QString & >(installationError));
  stubMethodEntered("initPlaceHolder",params);
}

void MExtensionHandleStub::kill() {
  stubMethodEntered("kill");
}

void MExtensionHandleStub::reinit() {
  stubMethodEntered("reinit");
}

void MExtensionHandleStub::updateDisplayState() {
  stubMethodEntered("updateDisplayState");
}

void MExtensionHandleStub::setAliveResponseTimeout(uint timeout) {
  QList<ParameterBase*> params;
  params.append( new Parameter<uint >(timeout));
  stubMethodEntered("setAliveResponseTimeout",params);
}

void MExtensionHandleStub::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QRectF >(rect));
  params.append( new Parameter<Qt::HANDLE >(pixmapHandle));
  stubMethodEntered("sendGeometryMessage",params);
}

void MExtensionHandleStub::stopCommunication() {
  stubMethodEntered("stopCommunication");
}

void MExtensionHandleStub::setRemoteActions(QList<QString> items) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QString> >(items));
  stubMethodEntered("setRemoteActions",params);
}

MExtensionHandleModel::State MExtensionHandleStub::state() const {
  stubMethodEntered("state");
  return stubReturnValue<MExtensionHandleModel::State>("state");
}

void MExtensionHandleStub::setSizeHints(const QVector<QSizeF> &sizeHints) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QVector<QSizeF> >(sizeHints));
  stubMethodEntered("setSizeHints",params);
}

void MExtensionHandleStub::MExtensionHandleConstructor(MExtensionHandlePrivate *dd, MExtensionHandleModel *model, QGraphicsItem *parent) {
  Q_UNUSED(dd);
  Q_UNUSED(model);
  Q_UNUSED(parent);

}
void MExtensionHandleStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void MExtensionHandleStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void MExtensionHandleStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void MExtensionHandleStub::cancelEvent(MCancelEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MCancelEvent * >(event));
  stubMethodEntered("cancelEvent",params);
}

void MExtensionHandleStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneContextMenuEvent * >(event));
  stubMethodEntered("contextMenuEvent",params);
}

void MExtensionHandleStub::enterDisplayEvent() {
  stubMethodEntered("enterDisplayEvent");
}

void MExtensionHandleStub::exitDisplayEvent() {
  stubMethodEntered("exitDisplayEvent");
}

void MExtensionHandleStub::connectionEstablished() {
  stubMethodEntered("connectionEstablished");
}

void MExtensionHandleStub::run() {
  stubMethodEntered("run");
}

void MExtensionHandleStub::messageReceived(const MAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}

void MExtensionHandleStub::communicationTimerTimeout() {
  stubMethodEntered("communicationTimerTimeout");
}

void MExtensionHandleStub::sendAliveMessageRequest() {
  stubMethodEntered("sendAliveMessageRequest");
}

void MExtensionHandleStub::visibilityEvent(bool visible) {
  QList<ParameterBase*> params;
  params.append( new Parameter<bool >(visible));
  stubMethodEntered("visibilityEvent",params);
}

void MExtensionHandleStub::visibilityChanged() {
  stubMethodEntered("visibilityChanged");
}

void MExtensionHandleStub::orientationEvent(const M::Orientation &orientation) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const M::Orientation & >(orientation));
  stubMethodEntered("orientationEvent",params);
}

void MExtensionHandleStub::processStdErrorReady() {
  stubMethodEntered("processStdErrorReady");
}

void MExtensionHandleStub::processStdOutputReady() {
  stubMethodEntered("processStdOutputReady");
}

void MExtensionHandleStub::processError(QProcess::ProcessError error) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QProcess::ProcessError >(error));
  stubMethodEntered("processError",params);
}

void MExtensionHandleStub::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  QList<ParameterBase*> params;
  params.append( new Parameter<int >(exitCode));
  params.append( new Parameter<QProcess::ExitStatus >(exitStatus));
  stubMethodEntered("processFinished",params);
}

void MExtensionHandleStub::remoteActionTriggered() {
  stubMethodEntered("remoteActionTriggered");
}

void MExtensionHandleStub::setBrokenState() {
  stubMethodEntered("setBrokenState");
}

void MExtensionHandleStub::displayContextMenu(QList<QString> actions) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QList<QString> >(actions));
  stubMethodEntered("displayContextMenu",params);
}



// 3. CREATE A STUB INSTANCE
MExtensionHandleStub gDefaultMExtensionHandleStub;
MExtensionHandleStub* gMExtensionHandleStub = &gDefaultMExtensionHandleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MExtensionHandle::MExtensionHandle(QGraphicsItem *parent) : d_ptr(NULL) {
  gMExtensionHandleStub->MExtensionHandleConstructor(parent);
}

MExtensionHandle::~MExtensionHandle() {
  gMExtensionHandleStub->MExtensionHandleDestructor();
}

void MExtensionHandle::init(const QString &runnerBinary, const QString &metaDataFileName, const QStringList &extraArguments) {
  gMExtensionHandleStub->init(runnerBinary, metaDataFileName, extraArguments);
}

void MExtensionHandle::initPlaceHolder(const QString &packageName, const QString &installationError) {
  gMExtensionHandleStub->initPlaceHolder(packageName, installationError);
}

void MExtensionHandle::kill() {
  gMExtensionHandleStub->kill();
}

void MExtensionHandle::reinit() {
  gMExtensionHandleStub->reinit();
}

void MExtensionHandle::setAliveResponseTimeout(uint timeout) {
  gMExtensionHandleStub->setAliveResponseTimeout(timeout);
}

void MExtensionHandle::sendGeometryMessage(QRectF rect, Qt::HANDLE pixmapHandle) {
  gMExtensionHandleStub->sendGeometryMessage(rect, pixmapHandle);
}

void MExtensionHandle::stopCommunication() {
  gMExtensionHandleStub->stopCommunication();
}

void MExtensionHandle::setRemoteActions(QList<QString> items) {
  gMExtensionHandleStub->setRemoteActions(items);
}

MExtensionHandleModel::State MExtensionHandle::state() const {
  return gMExtensionHandleStub->state();
}

void MExtensionHandle::setSizeHints(const QVector<QSizeF> &sizeHints) {
  gMExtensionHandleStub->setSizeHints(sizeHints);
}

MExtensionHandle::MExtensionHandle(MExtensionHandlePrivate *dd, MExtensionHandleModel *model, QGraphicsItem *parent) : d_ptr(NULL)  {
  gMExtensionHandleStub->MExtensionHandleConstructor(dd, model, parent);
}

void MExtensionHandle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gMExtensionHandleStub->mousePressEvent(event);
}

void MExtensionHandle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gMExtensionHandleStub->mouseReleaseEvent(event);
}

void MExtensionHandle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gMExtensionHandleStub->mouseMoveEvent(event);
}

void MExtensionHandle::cancelEvent(MCancelEvent *event) {
  gMExtensionHandleStub->cancelEvent(event);
}

void MExtensionHandle::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
  gMExtensionHandleStub->contextMenuEvent(event);
}

void MExtensionHandle::enterDisplayEvent() {
  gMExtensionHandleStub->enterDisplayEvent();
}

void MExtensionHandle::exitDisplayEvent() {
  gMExtensionHandleStub->exitDisplayEvent();
}

void MExtensionHandle::connectionEstablished() {
  gMExtensionHandleStub->connectionEstablished();
}

void MExtensionHandle::run() {
  gMExtensionHandleStub->run();
}

void MExtensionHandle::messageReceived(const MAppletMessage &message) {
  gMExtensionHandleStub->messageReceived(message);
}

void MExtensionHandle::communicationTimerTimeout() {
  gMExtensionHandleStub->communicationTimerTimeout();
}

void MExtensionHandle::sendAliveMessageRequest() {
  gMExtensionHandleStub->sendAliveMessageRequest();
}

void MExtensionHandle::visibilityEvent(bool visible) {
  gMExtensionHandleStub->visibilityEvent(visible);
}

void MExtensionHandle::visibilityChanged() {
  gMExtensionHandleStub->visibilityChanged();
}

void MExtensionHandle::orientationEvent(M::Orientation orientation) {
  gMExtensionHandleStub->orientationEvent(orientation);
}

void MExtensionHandle::processStdErrorReady() {
  gMExtensionHandleStub->processStdErrorReady();
}

void MExtensionHandle::processStdOutputReady() {
  gMExtensionHandleStub->processStdOutputReady();
}

void MExtensionHandle::processError(QProcess::ProcessError error) {
  gMExtensionHandleStub->processError(error);
}

void MExtensionHandle::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  gMExtensionHandleStub->processFinished(exitCode, exitStatus);
}

void MExtensionHandle::remoteActionTriggered() {
  gMExtensionHandleStub->remoteActionTriggered();
}

void MExtensionHandle::setBrokenState() {
  gMExtensionHandleStub->setBrokenState();
}

void MExtensionHandle::displayContextMenu(QList<QString> actions) {
  gMExtensionHandleStub->displayContextMenu(actions);
}

void MExtensionHandle::updateDisplayState() {
  gMExtensionHandleStub->updateDisplayState();
}

#endif
