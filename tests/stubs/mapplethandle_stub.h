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

#ifndef MAPPLETHANDLE_STUB
#define MAPPLETHANDLE_STUB

#include "mapplethandle.h"
#include "mextensionhandle_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletHandleStub : public StubBase {
  public:
  virtual void MAppletHandleConstructor(QGraphicsItem *parent);
  virtual void MAppletHandleDestructor();
  virtual void init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const MAppletId &appletId);
  virtual void initPlaceHolder(const MAppletId &appletId, const QString &packageName, const QString &installationError);
  virtual void kill();
  virtual void removeApplet();
  virtual void setAppletIcon(const QString &newIcon);
  virtual void setAppletTitle(const QString &newTitle);
  virtual void setAppletText(const QString &newText);
  virtual QString appletIcon() const;
  virtual QString appletTitle() const;
  virtual QString appletText() const;
  virtual void messageReceived(const MAppletMessage &message);
}; 

// 2. IMPLEMENT STUB
void MAppletHandleStub::MAppletHandleConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void MAppletHandleStub::MAppletHandleDestructor() {

}
void MAppletHandleStub::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const MAppletId &appletId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(runnerBinary));
  params.append( new Parameter<QString >(appletInstanceFileDataPath));
  params.append( new Parameter<QString >(metaDataFileName));
  params.append( new Parameter<MAppletId >(appletId));
  stubMethodEntered("init",params);
}

void MAppletHandleStub::initPlaceHolder(const MAppletId &appletId, const QString &packageName, const QString &installationError) {
  QList<ParameterBase*> params;
  params.append( new Parameter<MAppletId >(appletId));
  params.append( new Parameter<QString >(packageName));
  params.append( new Parameter<QString >(installationError));
  stubMethodEntered("initPlaceHolder",params);
}

void MAppletHandleStub::kill() {
  stubMethodEntered("kill");
}

void MAppletHandleStub::removeApplet() {
  stubMethodEntered("removeApplet");
}

void MAppletHandleStub::setAppletIcon(const QString &newIcon) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newIcon));
  stubMethodEntered("setAppletIcon",params);
}

void MAppletHandleStub::setAppletTitle(const QString &newTitle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newTitle));
  stubMethodEntered("setAppletTitle",params);
}

void MAppletHandleStub::setAppletText(const QString &newText) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newText));
  stubMethodEntered("setAppletText",params);
}

QString MAppletHandleStub::appletIcon() const {
  stubMethodEntered("appletIcon");
  return stubReturnValue<QString>("appletIcon");
}

QString MAppletHandleStub::appletTitle() const {
  stubMethodEntered("appletTitle");
  return stubReturnValue<QString>("appletTitle");
}

QString MAppletHandleStub::appletText() const {
  stubMethodEntered("appletText");
  return stubReturnValue<QString>("appletText");
}

void MAppletHandleStub::messageReceived(const MAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}



// 3. CREATE A STUB INSTANCE
MAppletHandleStub gDefaultMAppletHandleStub;
MAppletHandleStub* gMAppletHandleStub = &gDefaultMAppletHandleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAppletHandle::MAppletHandle(QGraphicsItem *parent) {
  gMAppletHandleStub->MAppletHandleConstructor(parent);
}

MAppletHandle::~MAppletHandle() {
  gMAppletHandleStub->MAppletHandleDestructor();
}

void MAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const MAppletId &appletId) {
  gMAppletHandleStub->init(runnerBinary, appletInstanceFileDataPath, metaDataFileName, appletId);
}

void MAppletHandle::initPlaceHolder(const MAppletId &appletId, const QString &packageName, const QString &installationError) {
  gMAppletHandleStub->initPlaceHolder(appletId, packageName, installationError);
}

void MAppletHandle::kill() {
  gMAppletHandleStub->kill();
}

void MAppletHandle::removeApplet() {
  gMAppletHandleStub->removeApplet();
}

void MAppletHandle::setAppletIcon(const QString &newIcon) {
  gMAppletHandleStub->setAppletIcon(newIcon);
}

void MAppletHandle::setAppletTitle(const QString &newTitle) {
  gMAppletHandleStub->setAppletTitle(newTitle);
}

void MAppletHandle::setAppletText(const QString &newText) {
  gMAppletHandleStub->setAppletText(newText);
}

QString MAppletHandle::appletIcon() const {
  return gMAppletHandleStub->appletIcon();
}

QString MAppletHandle::appletTitle() const {
  return gMAppletHandleStub->appletTitle();
}

QString MAppletHandle::appletText() const {
  return gMAppletHandleStub->appletText();
}

void MAppletHandle::messageReceived(const MAppletMessage &message) {
  gMAppletHandleStub->messageReceived(message);
}


#endif
