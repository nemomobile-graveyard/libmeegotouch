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

#ifndef DUIAPPLETLOADER_STUB
#define DUIAPPLETLOADER_STUB

#include "duiappletloader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiAppletLoaderStub : public StubBase {
  public:
  virtual DuiWidget * loadApplet(const DuiAppletMetaData &metadata, DuiDataStore &dataStore, DuiDataAccess &settings);
  virtual void DuiAppletLoaderConstructor();
}; 

// 2. IMPLEMENT STUB
DuiWidget * DuiAppletLoaderStub::loadApplet(const DuiAppletMetaData &metadata, DuiDataStore &dataStore, DuiDataAccess &settings) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(metadata.fileName()));
  params.append( new Parameter<DuiDataStore & >(dataStore));
  params.append( new Parameter<DuiDataAccess & >(settings));
  stubMethodEntered("loadApplet",params);
  return stubReturnValue<DuiWidget *>("loadApplet");
}

void DuiAppletLoaderStub::DuiAppletLoaderConstructor() {

}


// 3. CREATE A STUB INSTANCE
DuiAppletLoaderStub gDefaultDuiAppletLoaderStub;
DuiAppletLoaderStub* gDuiAppletLoaderStub = &gDefaultDuiAppletLoaderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidget * DuiAppletLoader::loadApplet(const DuiAppletMetaData &metadata, DuiDataStore &dataStore, DuiDataAccess &settings) {
  return gDuiAppletLoaderStub->loadApplet(metadata, dataStore, settings);
}

DuiAppletLoader::DuiAppletLoader() {
  gDuiAppletLoaderStub->DuiAppletLoaderConstructor();
}


#endif
