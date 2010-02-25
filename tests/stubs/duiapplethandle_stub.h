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
#include "duiextensionhandle_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletHandleStub : public StubBase {
  public:
  virtual void DuiAppletHandleConstructor(QGraphicsItem *parent);
  virtual void DuiAppletHandleDestructor();
  virtual void init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId);
  virtual void initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError);
  virtual void kill();
  virtual void removeApplet();
  virtual void setAppletIcon(const QString &newIcon);
  virtual void setAppletTitle(const QString &newTitle);
  virtual void setAppletText(const QString &newText);
  virtual QString appletIcon() const;
  virtual QString appletTitle() const;
  virtual QString appletText() const;
  virtual void messageReceived(const DuiAppletMessage &message);
}; 

// 2. IMPLEMENT STUB
void DuiAppletHandleStub::DuiAppletHandleConstructor(QGraphicsItem *parent) {
  Q_UNUSED(parent);

}
void DuiAppletHandleStub::DuiAppletHandleDestructor() {

}
void DuiAppletHandleStub::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(runnerBinary));
  params.append( new Parameter<QString >(appletInstanceFileDataPath));
  params.append( new Parameter<QString >(metaDataFileName));
  params.append( new Parameter<const DuiAppletId & >(appletId));
  stubMethodEntered("init",params);
}

void DuiAppletHandleStub::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletId & >(appletId));
  params.append( new Parameter<QString >(packageName));
  params.append( new Parameter<QString >(installationError));
  stubMethodEntered("initPlaceHolder",params);
}

void DuiAppletHandleStub::kill() {
  stubMethodEntered("kill");
}

void DuiAppletHandleStub::removeApplet() {
  stubMethodEntered("removeApplet");
}

void DuiAppletHandleStub::setAppletIcon(const QString &newIcon) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newIcon));
  stubMethodEntered("setAppletIcon",params);
}

void DuiAppletHandleStub::setAppletTitle(const QString &newTitle) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newTitle));
  stubMethodEntered("setAppletTitle",params);
}

void DuiAppletHandleStub::setAppletText(const QString &newText) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(newText));
  stubMethodEntered("setAppletText",params);
}

QString DuiAppletHandleStub::appletIcon() const {
  stubMethodEntered("appletIcon");
  return stubReturnValue<QString>("appletIcon");
}

QString DuiAppletHandleStub::appletTitle() const {
  stubMethodEntered("appletTitle");
  return stubReturnValue<QString>("appletTitle");
}

QString DuiAppletHandleStub::appletText() const {
  stubMethodEntered("appletText");
  return stubReturnValue<QString>("appletText");
}

void DuiAppletHandleStub::messageReceived(const DuiAppletMessage &message) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiAppletMessage & >(message));
  stubMethodEntered("messageReceived",params);
}



// 3. CREATE A STUB INSTANCE
DuiAppletHandleStub gDefaultDuiAppletHandleStub;
DuiAppletHandleStub* gDuiAppletHandleStub = &gDefaultDuiAppletHandleStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAppletHandle::DuiAppletHandle(QGraphicsItem *parent) {
  gDuiAppletHandleStub->DuiAppletHandleConstructor(parent);
}

DuiAppletHandle::~DuiAppletHandle() {
  gDuiAppletHandleStub->DuiAppletHandleDestructor();
}

void DuiAppletHandle::init(const QString &runnerBinary, const QString &appletInstanceFileDataPath, const QString &metaDataFileName, const DuiAppletId &appletId) {
  gDuiAppletHandleStub->init(runnerBinary, appletInstanceFileDataPath, metaDataFileName, appletId);
}

void DuiAppletHandle::initPlaceHolder(const DuiAppletId &appletId, const QString &packageName, const QString &installationError) {
  gDuiAppletHandleStub->initPlaceHolder(appletId, packageName, installationError);
}

void DuiAppletHandle::kill() {
  gDuiAppletHandleStub->kill();
}

void DuiAppletHandle::removeApplet() {
  gDuiAppletHandleStub->removeApplet();
}

void DuiAppletHandle::setAppletIcon(const QString &newIcon) {
  gDuiAppletHandleStub->setAppletIcon(newIcon);
}

void DuiAppletHandle::setAppletTitle(const QString &newTitle) {
  gDuiAppletHandleStub->setAppletTitle(newTitle);
}

void DuiAppletHandle::setAppletText(const QString &newText) {
  gDuiAppletHandleStub->setAppletText(newText);
}

QString DuiAppletHandle::appletIcon() const {
  return gDuiAppletHandleStub->appletIcon();
}

QString DuiAppletHandle::appletTitle() const {
  return gDuiAppletHandleStub->appletTitle();
}

QString DuiAppletHandle::appletText() const {
  return gDuiAppletHandleStub->appletText();
}

void DuiAppletHandle::messageReceived(const DuiAppletMessage &message) {
  gDuiAppletHandleStub->messageReceived(message);
}


#endif
