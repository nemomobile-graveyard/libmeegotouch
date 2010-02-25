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

#ifndef DUIEXTENSIONRUNNER_STUB
#define DUIEXTENSIONRUNNER_STUB

#include "duiextensionrunner.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiExtensionRunnerStub : public StubBase {
  public:
  virtual void DuiExtensionRunnerConstructor();
  virtual void DuiExtensionRunnerDestructor();
  virtual bool init(const QString &serverName);
  virtual void sendUpdateGeometryMessage();
  virtual void messageReceived(const DuiAppletMessage &message);
  virtual void sceneChanged(const QList<QRectF> &region);
  virtual void hostProcessNotAlive();
  virtual void teardown();
  virtual void handleMouseEvent(const DuiAppletMouseMessage *message);
  virtual void handleCancelEvent(const DuiAppletCancelMessage *message);
  virtual void createAppletPixmap(Qt::HANDLE handle);
}; 

// 2. IMPLEMENT STUB
void DuiExtensionRunnerStub::DuiExtensionRunnerConstructor() {

}
void DuiExtensionRunnerStub::DuiExtensionRunnerDestructor() {

}
bool DuiExtensionRunnerStub::init(const QString &serverName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(serverName));
  stubMethodEntered("init",params);
  return stubReturnValue<bool>("init");
}

void DuiExtensionRunnerStub::sendUpdateGeometryMessage() {
  stubMethodEntered("sendUpdateGeometryMessage");
}

void DuiExtensionRunnerStub::messageReceived(const DuiAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}

void DuiExtensionRunnerStub::sceneChanged(const QList<QRectF> &region) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<QRectF> & >(region));
  stubMethodEntered("sceneChanged",params);
}

void DuiExtensionRunnerStub::hostProcessNotAlive() {
  stubMethodEntered("hostProcessNotAlive");
}

void DuiExtensionRunnerStub::teardown() {
  stubMethodEntered("teardown");
}

void DuiExtensionRunnerStub::handleMouseEvent(const DuiAppletMouseMessage *message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletMouseMessage * >(message));
  stubMethodEntered("handleMouseEvent",params);
}

void DuiExtensionRunnerStub::handleCancelEvent(const DuiAppletCancelMessage *message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletCancelMessage * >(message));
  stubMethodEntered("handleCancelEvent",params);
}

void DuiExtensionRunnerStub::createAppletPixmap(Qt::HANDLE handle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::HANDLE >(handle));
  stubMethodEntered("createAppletPixmap",params);
}



// 3. CREATE A STUB INSTANCE
DuiExtensionRunnerStub gDefaultDuiExtensionRunnerStub;
DuiExtensionRunnerStub* gDuiExtensionRunnerStub = &gDefaultDuiExtensionRunnerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiExtensionRunner::DuiExtensionRunner() {
  gDuiExtensionRunnerStub->DuiExtensionRunnerConstructor();
}

DuiExtensionRunner::~DuiExtensionRunner() {
  gDuiExtensionRunnerStub->DuiExtensionRunnerDestructor();
}

bool DuiExtensionRunner::init(const QString &serverName) {
  return gDuiExtensionRunnerStub->init(serverName);
}

void DuiExtensionRunner::sendUpdateGeometryMessage() {
  gDuiExtensionRunnerStub->sendUpdateGeometryMessage();
}

void DuiExtensionRunner::messageReceived(const DuiAppletMessage &message) {
  gDuiExtensionRunnerStub->messageReceived(message);
}

void DuiExtensionRunner::sceneChanged(const QList<QRectF> &region) {
  gDuiExtensionRunnerStub->sceneChanged(region);
}

void DuiExtensionRunner::hostProcessNotAlive() {
  gDuiExtensionRunnerStub->hostProcessNotAlive();
}

void DuiExtensionRunner::teardown() {
  gDuiExtensionRunnerStub->teardown();
}

void DuiExtensionRunner::handleMouseEvent(const DuiAppletMouseMessage *message) {
  gDuiExtensionRunnerStub->handleMouseEvent(message);
}

void DuiExtensionRunner::handleCancelEvent(const DuiAppletCancelMessage *message) {
  gDuiExtensionRunnerStub->handleCancelEvent(message);
}

void DuiExtensionRunner::createAppletPixmap(Qt::HANDLE handle) {
  gDuiExtensionRunnerStub->createAppletPixmap(handle);
}


#endif
