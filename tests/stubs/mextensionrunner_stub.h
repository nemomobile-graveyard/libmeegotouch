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

#ifndef MEXTENSIONRUNNER_STUB
#define MEXTENSIONRUNNER_STUB

#include "mextensionrunner.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MExtensionRunnerStub : public StubBase {
  public:
  virtual void MExtensionRunnerConstructor();
  virtual void MExtensionRunnerDestructor();
  virtual bool init(const QString &serverName);
  virtual void sendUpdateGeometryMessage();
  virtual void messageReceived(const MAppletMessage &message);
  virtual void sceneChanged(const QList<QRectF> &region);
  virtual void hostProcessNotAlive();
  virtual void teardown();
  virtual void handleMouseEvent(const MAppletMouseMessage *message);
  virtual void handleCancelEvent(const MAppletCancelMessage *message);
  virtual void createAppletPixmap(Qt::HANDLE handle);
}; 

// 2. IMPLEMENT STUB
void MExtensionRunnerStub::MExtensionRunnerConstructor() {

}
void MExtensionRunnerStub::MExtensionRunnerDestructor() {

}
bool MExtensionRunnerStub::init(const QString &serverName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(serverName));
  stubMethodEntered("init",params);
  return stubReturnValue<bool>("init");
}

void MExtensionRunnerStub::sendUpdateGeometryMessage() {
  stubMethodEntered("sendUpdateGeometryMessage");
}

void MExtensionRunnerStub::messageReceived(const MAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}

void MExtensionRunnerStub::sceneChanged(const QList<QRectF> &region) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QList<QRectF> & >(region));
  stubMethodEntered("sceneChanged",params);
}

void MExtensionRunnerStub::hostProcessNotAlive() {
  stubMethodEntered("hostProcessNotAlive");
}

void MExtensionRunnerStub::teardown() {
  stubMethodEntered("teardown");
}

void MExtensionRunnerStub::handleMouseEvent(const MAppletMouseMessage *message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MAppletMouseMessage * >(message));
  stubMethodEntered("handleMouseEvent",params);
}

void MExtensionRunnerStub::handleCancelEvent(const MAppletCancelMessage *message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MAppletCancelMessage * >(message));
  stubMethodEntered("handleCancelEvent",params);
}

void MExtensionRunnerStub::createAppletPixmap(Qt::HANDLE handle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<Qt::HANDLE >(handle));
  stubMethodEntered("createAppletPixmap",params);
}



// 3. CREATE A STUB INSTANCE
MExtensionRunnerStub gDefaultMExtensionRunnerStub;
MExtensionRunnerStub* gMExtensionRunnerStub = &gDefaultMExtensionRunnerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MExtensionRunner::MExtensionRunner()
  : communicator( NULL ),
    parentWidget( NULL ),
    widget( NULL ),
    scene( NULL ),
    view( NULL ),
#ifdef HAVE_CONTEXTSUBSCRIBER
    screenBlankProperty(0),
#endif
    displayBlanked(false),
#ifdef QT_OPENGL_LIB
    context( NULL ),
#endif
    aliveTimer( NULL ),
    pixmapMutex( NULL ),
    visible( false )
{
  gMExtensionRunnerStub->MExtensionRunnerConstructor();
}

MExtensionRunner::~MExtensionRunner() {
  gMExtensionRunnerStub->MExtensionRunnerDestructor();
}

bool MExtensionRunner::init(const QString &serverName) {
  return gMExtensionRunnerStub->init(serverName);
}

void MExtensionRunner::sendUpdateGeometryMessage() {
  gMExtensionRunnerStub->sendUpdateGeometryMessage();
}

void MExtensionRunner::messageReceived(const MAppletMessage &message) {
  gMExtensionRunnerStub->messageReceived(message);
}

void MExtensionRunner::sceneChanged(const QList<QRectF> &region) {
  gMExtensionRunnerStub->sceneChanged(region);
}

void MExtensionRunner::hostProcessNotAlive() {
  gMExtensionRunnerStub->hostProcessNotAlive();
}

void MExtensionRunner::teardown() {
  gMExtensionRunnerStub->teardown();
}

void MExtensionRunner::handleMouseEvent(const MAppletMouseMessage *message) {
  gMExtensionRunnerStub->handleMouseEvent(message);
}

void MExtensionRunner::handleCancelEvent(const MAppletCancelMessage *message) {
  gMExtensionRunnerStub->handleCancelEvent(message);
}

void MExtensionRunner::createAppletPixmap(Qt::HANDLE handle) {
  gMExtensionRunnerStub->createAppletPixmap(handle);
}


#endif
