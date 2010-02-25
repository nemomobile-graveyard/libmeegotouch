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

#ifndef DUIAPPLICATIONEXTENSIONMETADATA_STUB
#define DUIAPPLICATIONEXTENSIONMETADATA_STUB

#include "duiapplicationextensionmetadata.h"
#include "duidesktopentry_stub.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationExtensionMetaDataStub : public DuiDesktopEntryStub {
  public:
  virtual void DuiApplicationExtensionMetaDataConstructor(const QString &filename);
  virtual void DuiApplicationExtensionMetaDataDestructor();
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
void DuiApplicationExtensionMetaDataStub::DuiApplicationExtensionMetaDataConstructor(const QString &filename) {
  Q_UNUSED(filename);
}
void DuiApplicationExtensionMetaDataStub::DuiApplicationExtensionMetaDataDestructor() {

}
bool DuiApplicationExtensionMetaDataStub::isValid() const {
  stubMethodEntered("isValid");
  return stubReturnValue<bool>("isValid");
}

QString DuiApplicationExtensionMetaDataStub::runnerBinary() const {
  stubMethodEntered("runnerBinary");
  return stubReturnValue<QString>("runnerBinary");
}

void DuiApplicationExtensionMetaDataStub::stubSetExtensionBinaryMultiple(const QString &value) {
    extensionBinaryMultiple.append(value);
}

QString DuiApplicationExtensionMetaDataStub::extensionBinary() const {
  stubMethodEntered("extensionBinary");

  // Look first from the multiple values
  if (!extensionBinaryMultiple.isEmpty()) {
      return extensionBinaryMultiple.takeFirst();
  }

  return stubReturnValue<QString>("extensionBinary");
}

QString DuiApplicationExtensionMetaDataStub::interface() const {
  stubMethodEntered("interface");
  return stubReturnValue<QString>("interface");
}

QString DuiApplicationExtensionMetaDataStub::resourceIdentifier() const {
  stubMethodEntered("resourceIdentifier");
  return stubReturnValue<QString>("resourceIdentifier");
}

QString DuiApplicationExtensionMetaDataStub::extractLibraryName(const QString &libFileName) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const QString & >(libFileName));
  stubMethodEntered("extractLibraryName",params);
  return stubReturnValue<QString>("extractLibraryName");
}



// 3. CREATE A STUB INSTANCE
DuiApplicationExtensionMetaDataStub gDefaultDuiApplicationExtensionMetaDataStub;
DuiApplicationExtensionMetaDataStub* gDuiApplicationExtensionMetaDataStub = &gDefaultDuiApplicationExtensionMetaDataStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiApplicationExtensionMetaData::DuiApplicationExtensionMetaData(const QString &filename) : DuiDesktopEntry(filename) {
  gDuiApplicationExtensionMetaDataStub->DuiApplicationExtensionMetaDataConstructor(filename);
}

DuiApplicationExtensionMetaData::~DuiApplicationExtensionMetaData() {
  gDuiApplicationExtensionMetaDataStub->DuiApplicationExtensionMetaDataDestructor();
}

bool DuiApplicationExtensionMetaData::isValid() const {
  return gDuiApplicationExtensionMetaDataStub->isValid();
}

QString DuiApplicationExtensionMetaData::runnerBinary() const {
  return gDuiApplicationExtensionMetaDataStub->runnerBinary();
}

QString DuiApplicationExtensionMetaData::extensionBinary() const {
  return gDuiApplicationExtensionMetaDataStub->extensionBinary();
}

QString DuiApplicationExtensionMetaData::interface() const {
  return gDuiApplicationExtensionMetaDataStub->interface();
}

QString DuiApplicationExtensionMetaData::resourceIdentifier() const {
  return gDuiApplicationExtensionMetaDataStub->resourceIdentifier();
}

QString DuiApplicationExtensionMetaData::extractLibraryName(const QString &libFileName) {
  return gDuiApplicationExtensionMetaDataStub->extractLibraryName(libFileName);
}


#endif
