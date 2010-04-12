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

#ifndef MAPPLICATIONEXTENSIONLOADER_STUB
#define MAPPLICATIONEXTENSIONLOADER_STUB

#include "mapplicationextensionloader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationExtensionLoaderStub : public StubBase {
  public:
  virtual MApplicationExtensionInterface * loadExtension(const MApplicationExtensionMetaData &metadata);
  virtual void MApplicationExtensionLoaderConstructor();
}; 

// 2. IMPLEMENT STUB
MApplicationExtensionInterface * MApplicationExtensionLoaderStub::loadExtension(const MApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("loadExtension",params);
  return stubReturnValue<MApplicationExtensionInterface *>("loadExtension");
}

void MApplicationExtensionLoaderStub::MApplicationExtensionLoaderConstructor() {

}


// 3. CREATE A STUB INSTANCE
MApplicationExtensionLoaderStub gDefaultMApplicationExtensionLoaderStub;
MApplicationExtensionLoaderStub* gMApplicationExtensionLoaderStub = &gDefaultMApplicationExtensionLoaderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplicationExtensionInterface * MApplicationExtensionLoader::loadExtension(const MApplicationExtensionMetaData &metadata) {
  return gMApplicationExtensionLoaderStub->loadExtension(metadata);
}

MApplicationExtensionLoader::MApplicationExtensionLoader() {
  gMApplicationExtensionLoaderStub->MApplicationExtensionLoaderConstructor();
}


#endif
