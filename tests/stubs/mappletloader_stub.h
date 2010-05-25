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

#ifndef MAPPLETLOADER_STUB
#define MAPPLETLOADER_STUB

#include "mappletloader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MAppletLoaderStub : public StubBase {
  public:
  virtual QGraphicsWidget * loadApplet(const MAppletMetaData &metadata, MDataStore &dataStore, MDataAccess &settings);
  virtual void MAppletLoaderConstructor();
}; 

// 2. IMPLEMENT STUB
QGraphicsWidget * MAppletLoaderStub::loadApplet(const MAppletMetaData &metadata, MDataStore &dataStore, MDataAccess &settings) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QString >(metadata.fileName()));
  params.append( new Parameter<MDataStore & >(dataStore));
  params.append( new Parameter<MDataAccess & >(settings));
  stubMethodEntered("loadApplet",params);
  return stubReturnValue<QGraphicsWidget *>("loadApplet");
}

void MAppletLoaderStub::MAppletLoaderConstructor() {

}


// 3. CREATE A STUB INSTANCE
MAppletLoaderStub gDefaultMAppletLoaderStub;
MAppletLoaderStub* gMAppletLoaderStub = &gDefaultMAppletLoaderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
QGraphicsWidget * MAppletLoader::loadApplet(const MAppletMetaData &metadata, MDataStore &dataStore, MDataAccess &settings) {
  return gMAppletLoaderStub->loadApplet(metadata, dataStore, settings);
}

MAppletLoader::MAppletLoader() {
  gMAppletLoaderStub->MAppletLoaderConstructor();
}


#endif
