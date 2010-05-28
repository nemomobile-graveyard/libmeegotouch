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

#ifndef MAPPLICATIONEXTENSIONMANAGER_STUB
#define MAPPLICATIONEXTENSIONMANAGER_STUB

#include "mapplicationextensionmanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionManagerStub : public StubBase {
  public:
  virtual void MApplicationExtensionManagerConstructor(const QString &interface);
  virtual void MApplicationExtensionManagerDestructor();
  virtual void setInProcessFilter(const QRegExp &inProcessFilter);
  virtual void setOutOfProcessFilter(const QRegExp &outOfProcessFilter);
  virtual bool init();
  virtual QList<MApplicationExtensionInterface *> extensions();
  virtual void updateAvailableExtensions(const QString &path);
  virtual QString dataPath() const;
  virtual bool isInProcess(const MApplicationExtensionMetaData &metaData);
  virtual bool createDataStore();
  virtual bool instantiateExtension(const MApplicationExtensionMetaData &metadata);
  virtual bool instantiateInProcessExtension(const QString &binary);
  virtual bool instantiateOutOfProcessExtension(const MApplicationExtensionMetaData &metadata);
  virtual void removeExtension(const MApplicationExtensionMetaData &metadata);
  virtual void removeInProcessExtension(const QString &library);
  virtual void removeOutOfProcessExtension(const MApplicationExtensionMetaData &metadata);
  virtual QString createApplicationExtensionDataFileName(const QString &interface) const;
}; 

// 2. IMPLEMENT STUB
void MApplicationExtensionManagerStub::MApplicationExtensionManagerConstructor(const QString &interface) {
  Q_UNUSED(interface);

}
void MApplicationExtensionManagerStub::MApplicationExtensionManagerDestructor() {

}
void MApplicationExtensionManagerStub::setInProcessFilter(const QRegExp &inProcessFilter) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QRegExp >(inProcessFilter));
  stubMethodEntered("setInProcessFilter",params);
}

void MApplicationExtensionManagerStub::setOutOfProcessFilter(const QRegExp &outOfProcessFilter) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QRegExp >(outOfProcessFilter));
  stubMethodEntered("setOutOfProcessFilter",params);
}

bool MApplicationExtensionManagerStub::init() {
  stubMethodEntered("init");
  return stubReturnValue<bool>("init");
}

QList<MApplicationExtensionInterface *> MApplicationExtensionManagerStub::extensions() {
  stubMethodEntered("extensions");
  return stubReturnValue<QList<MApplicationExtensionInterface *> >("extensions");
}

void MApplicationExtensionManagerStub::updateAvailableExtensions(const QString &path) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(path));
  stubMethodEntered("updateAvailableExtensions",params);
}

QString MApplicationExtensionManagerStub::dataPath() const {
  stubMethodEntered("dataPath");
  return stubReturnValue<QString>("dataPath");
}

bool MApplicationExtensionManagerStub::isInProcess(const MApplicationExtensionMetaData &metaData) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metaData));
  stubMethodEntered("isInProcess",params);
  return stubReturnValue<bool>("isInProcess");
}

bool MApplicationExtensionManagerStub::createDataStore() {
  stubMethodEntered("createDataStore");
  return stubReturnValue<bool>("createDataStore");
}

bool MApplicationExtensionManagerStub::instantiateExtension(const MApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("instantiateExtension",params);
  return stubReturnValue<bool>("instantiateExtension");
}

bool MApplicationExtensionManagerStub::instantiateInProcessExtension(const QString &binary) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(binary));
  stubMethodEntered("instantiateInProcessExtension",params);
  return stubReturnValue<bool>("instantiateInProcessExtension");
}

bool MApplicationExtensionManagerStub::instantiateOutOfProcessExtension(const MApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("instantiateOutOfProcessExtension",params);
  return stubReturnValue<bool>("instantiateOutOfProcessExtension");
}

void MApplicationExtensionManagerStub::removeExtension(const MApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("removeExtension",params);
}

void MApplicationExtensionManagerStub::removeInProcessExtension(const QString &library) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(library));
  stubMethodEntered("removeInProcessExtension",params);
}

void MApplicationExtensionManagerStub::removeOutOfProcessExtension(const MApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("removeOutOfProcessExtension",params);
}

QString MApplicationExtensionManagerStub::createApplicationExtensionDataFileName(const QString &interface) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(interface));
  stubMethodEntered("createApplicationExtensionDataFileName",params);
  return stubReturnValue<QString>("createApplicationExtensionDataFileName");
}



// 3. CREATE A STUB INSTANCE
MApplicationExtensionManagerStub gDefaultMApplicationExtensionManagerStub;
MApplicationExtensionManagerStub* gMApplicationExtensionManagerStub = &gDefaultMApplicationExtensionManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplicationExtensionManager::MApplicationExtensionManager(const QString &interface) {
  gMApplicationExtensionManagerStub->MApplicationExtensionManagerConstructor(interface);
}

MApplicationExtensionManager::~MApplicationExtensionManager() {
  gMApplicationExtensionManagerStub->MApplicationExtensionManagerDestructor();
}

void MApplicationExtensionManager::setInProcessFilter(const QRegExp &inProcessFilter) {
  gMApplicationExtensionManagerStub->setInProcessFilter(inProcessFilter);
}

void MApplicationExtensionManager::setOutOfProcessFilter(const QRegExp &outOfProcessFilter) {
  gMApplicationExtensionManagerStub->setOutOfProcessFilter(outOfProcessFilter);
}

bool MApplicationExtensionManager::init() {
  return gMApplicationExtensionManagerStub->init();
}

QList<MApplicationExtensionInterface *> MApplicationExtensionManager::extensions() {
  return gMApplicationExtensionManagerStub->extensions();
}

void MApplicationExtensionManager::updateAvailableExtensions(const QString &path) {
  gMApplicationExtensionManagerStub->updateAvailableExtensions(path);
}

QString MApplicationExtensionManager::dataPath() const {
  return gMApplicationExtensionManagerStub->dataPath();
}

bool MApplicationExtensionManager::isInProcess(const MApplicationExtensionMetaData &metaData) {
  return gMApplicationExtensionManagerStub->isInProcess(metaData);
}

bool MApplicationExtensionManager::createDataStore() {
  return gMApplicationExtensionManagerStub->createDataStore();
}

bool MApplicationExtensionManager::instantiateExtension(const MApplicationExtensionMetaData &metadata) {
  return gMApplicationExtensionManagerStub->instantiateExtension(metadata);
}

bool MApplicationExtensionManager::instantiateInProcessExtension(const QString &binary) {
  return gMApplicationExtensionManagerStub->instantiateInProcessExtension(binary);
}

bool MApplicationExtensionManager::instantiateOutOfProcessExtension(const MApplicationExtensionMetaData &metadata) {
  return gMApplicationExtensionManagerStub->instantiateOutOfProcessExtension(metadata);
}

void MApplicationExtensionManager::removeExtension(const MApplicationExtensionMetaData &metadata) {
  gMApplicationExtensionManagerStub->removeExtension(metadata);
}

void MApplicationExtensionManager::removeInProcessExtension(const QString &library) {
  gMApplicationExtensionManagerStub->removeInProcessExtension(library);
}

void MApplicationExtensionManager::removeOutOfProcessExtension(const MApplicationExtensionMetaData &metadata) {
  gMApplicationExtensionManagerStub->removeOutOfProcessExtension(metadata);
}

QString MApplicationExtensionManager::createApplicationExtensionDataFileName(const QString &interface) const {
  return gMApplicationExtensionManagerStub->createApplicationExtensionDataFileName(interface);
}


#endif
