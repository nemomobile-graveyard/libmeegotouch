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

#ifndef DUIFILEDATASTORE_STUB
#define DUIFILEDATASTORE_STUB

#include "duifiledatastore.h"
#include <stubbase.h>
#include <QSettings>
#include <QMap>
#include <QScopedPointer>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiFileDataStoreStub : public StubBase {
  public:
  virtual void DuiFileDataStoreConstructor(const QString &filePath);
  virtual bool createValue(const QString &key, const QVariant &value);
  virtual bool setValue(const QString &key, const QVariant &value);
  virtual QVariant value(const QString &key) const;
  virtual QStringList allKeys() const;
  virtual void remove(const QString &key);
  virtual void clear();
  virtual bool contains(const QString &key) const;
  virtual bool isReadable() const;
  virtual bool isWritable() const;
  virtual void takeSnapshot();
  virtual void fileChanged(const QString &fileName);
  virtual void directoryChanged(const QString &fileName);
  void * d_ptr;
}; 

// 2. IMPLEMENT STUB
void DuiFileDataStoreStub::DuiFileDataStoreConstructor(const QString &filePath) {
  Q_UNUSED(filePath);
  d_ptr=0;
}
bool DuiFileDataStoreStub::createValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(key));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("createValue",params);
  return stubReturnValue<bool>("createValue");
}

bool DuiFileDataStoreStub::setValue(const QString &key, const QVariant &value) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(key));
  params.append( new Parameter<const QVariant & >(value));
  stubMethodEntered("setValue",params);
  return stubReturnValue<bool>("setValue");
}

QVariant DuiFileDataStoreStub::value(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(key));
  stubMethodEntered("value",params);
  return stubReturnValue<QVariant>("value");
}

QStringList DuiFileDataStoreStub::allKeys() const {
  stubMethodEntered("allKeys");
  return stubReturnValue<QStringList>("allKeys");
}

void DuiFileDataStoreStub::remove(const QString &key) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(key));
  stubMethodEntered("remove",params);
}

void DuiFileDataStoreStub::clear() {
  stubMethodEntered("clear");
}

bool DuiFileDataStoreStub::contains(const QString &key) const {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(key));
  stubMethodEntered("contains",params);
  return stubReturnValue<bool>("contains");
}

bool DuiFileDataStoreStub::isReadable() const {
  stubMethodEntered("isReadable");
  return stubReturnValue<bool>("isReadable");
}

bool DuiFileDataStoreStub::isWritable() const {
  stubMethodEntered("isWritable");
  return stubReturnValue<bool>("isWritable");
}

void DuiFileDataStoreStub::takeSnapshot() {
  stubMethodEntered("takeSnapshot");
}

void DuiFileDataStoreStub::fileChanged(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(fileName));
  stubMethodEntered("fileChanged",params);
}

void DuiFileDataStoreStub::directoryChanged(const QString &fileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(fileName));
  stubMethodEntered("directoryChanged",params);
}



// 3. CREATE A STUB INSTANCE
DuiFileDataStoreStub gDefaultDuiFileDataStoreStub;
DuiFileDataStoreStub* gDuiFileDataStoreStub = &gDefaultDuiFileDataStoreStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiFileDataStore::DuiFileDataStore(const QString &filePath) :
  d_ptr(0)
{
  gDuiFileDataStoreStub->DuiFileDataStoreConstructor(filePath);
}

DuiFileDataStore::~DuiFileDataStore()
{
}

bool DuiFileDataStore::createValue(const QString &key, const QVariant &value) {
  return gDuiFileDataStoreStub->createValue(key, value);
}

bool DuiFileDataStore::setValue(const QString &key, const QVariant &value) {
  return gDuiFileDataStoreStub->setValue(key, value);
}

QVariant DuiFileDataStore::value(const QString &key) const {
  return gDuiFileDataStoreStub->value(key);
}

QStringList DuiFileDataStore::allKeys() const {
  return gDuiFileDataStoreStub->allKeys();
}

void DuiFileDataStore::remove(const QString &key) {
  gDuiFileDataStoreStub->remove(key);
}

void DuiFileDataStore::clear() {
  gDuiFileDataStoreStub->clear();
}

bool DuiFileDataStore::contains(const QString &key) const {
  return gDuiFileDataStoreStub->contains(key);
}

bool DuiFileDataStore::isReadable() const {
  return gDuiFileDataStoreStub->isReadable();
}

bool DuiFileDataStore::isWritable() const {
  return gDuiFileDataStoreStub->isWritable();
}

void DuiFileDataStore::takeSnapshot() {
  gDuiFileDataStoreStub->takeSnapshot();
}

void DuiFileDataStore::fileChanged(const QString &fileName) {
  gDuiFileDataStoreStub->fileChanged(fileName);
}

void DuiFileDataStore::directoryChanged(const QString &fileName) {
  gDuiFileDataStoreStub->directoryChanged(fileName);
}


#endif
