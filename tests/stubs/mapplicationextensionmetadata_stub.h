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

#ifndef MAPPLICATIONEXTENSIONMETADATA_STUB
#define MAPPLICATIONEXTENSIONMETADATA_STUB

#include "mapplicationextensionmetadata.h"
#include "mdesktopentry_stub.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionMetaDataStub : public MDesktopEntryStub {
  public:
  virtual void MApplicationExtensionMetaDataConstructor(const QString &filename);
  virtual void MApplicationExtensionMetaDataDestructor();
  virtual bool isValid() const;
  virtual QString runnerBinary() const;
  virtual QString extensionBinary() const;
  virtual QString interface() const;
  virtual QString resourceIdentifier() const;
  virtual QString extractLibraryName(const QString &libFileName);

  void stubSetExtensionBinaryMultiple(const QString &value);

private:
  mutable QList<QString> extensionBinaryMultiple;
};

// 2. IMPLEMENT STUB
void MApplicationExtensionMetaDataStub::MApplicationExtensionMetaDataConstructor(const QString &filename) {
  Q_UNUSED(filename);
}
void MApplicationExtensionMetaDataStub::MApplicationExtensionMetaDataDestructor() {

}
bool MApplicationExtensionMetaDataStub::isValid() const {
  stubMethodEntered("isValid");
  return stubReturnValue<bool>("isValid");
}

QString MApplicationExtensionMetaDataStub::runnerBinary() const {
  stubMethodEntered("runnerBinary");
  return stubReturnValue<QString>("runnerBinary");
}

void MApplicationExtensionMetaDataStub::stubSetExtensionBinaryMultiple(const QString &value) {
    extensionBinaryMultiple.append(value);
}

QString MApplicationExtensionMetaDataStub::extensionBinary() const {
  stubMethodEntered("extensionBinary");

  // Look first from the multiple values
  if (!extensionBinaryMultiple.isEmpty()) {
      return extensionBinaryMultiple.takeFirst();
  }

  return stubReturnValue<QString>("extensionBinary");
}

QString MApplicationExtensionMetaDataStub::interface() const {
  stubMethodEntered("interface");
  return stubReturnValue<QString>("interface");
}

QString MApplicationExtensionMetaDataStub::resourceIdentifier() const {
  stubMethodEntered("resourceIdentifier");
  return stubReturnValue<QString>("resourceIdentifier");
}

QString MApplicationExtensionMetaDataStub::extractLibraryName(const QString &libFileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(libFileName));
  stubMethodEntered("extractLibraryName",params);
  return stubReturnValue<QString>("extractLibraryName");
}



// 3. CREATE A STUB INSTANCE
MApplicationExtensionMetaDataStub gDefaultMApplicationExtensionMetaDataStub;
MApplicationExtensionMetaDataStub* gMApplicationExtensionMetaDataStub = &gDefaultMApplicationExtensionMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplicationExtensionMetaData::MApplicationExtensionMetaData(const QString &filename) : MDesktopEntry(filename) {
  gMApplicationExtensionMetaDataStub->MApplicationExtensionMetaDataConstructor(filename);
}

MApplicationExtensionMetaData::~MApplicationExtensionMetaData() {
  gMApplicationExtensionMetaDataStub->MApplicationExtensionMetaDataDestructor();
}

bool MApplicationExtensionMetaData::isValid() const {
  return gMApplicationExtensionMetaDataStub->isValid();
}

QString MApplicationExtensionMetaData::runnerBinary() const {
  return gMApplicationExtensionMetaDataStub->runnerBinary();
}

QString MApplicationExtensionMetaData::extensionBinary() const {
  return gMApplicationExtensionMetaDataStub->extensionBinary();
}

QString MApplicationExtensionMetaData::interface() const {
  return gMApplicationExtensionMetaDataStub->interface();
}

QString MApplicationExtensionMetaData::resourceIdentifier() const {
  return gMApplicationExtensionMetaDataStub->resourceIdentifier();
}

QString MApplicationExtensionMetaData::extractLibraryName(const QString &libFileName) {
  return gMApplicationExtensionMetaDataStub->extractLibraryName(libFileName);
}


#endif
