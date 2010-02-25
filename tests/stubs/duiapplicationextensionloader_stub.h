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

#ifndef DUIAPPLICATIONEXTENSIONLOADER_STUB
#define DUIAPPLICATIONEXTENSIONLOADER_STUB

#include "duiapplicationextensionloader.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationExtensionLoaderStub : public StubBase {
  public:
  virtual DuiApplicationExtensionInterface * loadExtension(const DuiApplicationExtensionMetaData &metadata);
  virtual void DuiApplicationExtensionLoaderConstructor();
}; 

// 2. IMPLEMENT STUB
DuiApplicationExtensionInterface * DuiApplicationExtensionLoaderStub::loadExtension(const DuiApplicationExtensionMetaData &metadata) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const DuiApplicationExtensionMetaData & >(metadata));
  stubMethodEntered("loadExtension",params);
  return stubReturnValue<DuiApplicationExtensionInterface *>("loadExtension");
}

void DuiApplicationExtensionLoaderStub::DuiApplicationExtensionLoaderConstructor() {

}


// 3. CREATE A STUB INSTANCE
DuiApplicationExtensionLoaderStub gDefaultDuiApplicationExtensionLoaderStub;
DuiApplicationExtensionLoaderStub* gDuiApplicationExtensionLoaderStub = &gDefaultDuiApplicationExtensionLoaderStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiApplicationExtensionInterface * DuiApplicationExtensionLoader::loadExtension(const DuiApplicationExtensionMetaData &metadata) {
  return gDuiApplicationExtensionLoaderStub->loadExtension(metadata);
}

DuiApplicationExtensionLoader::DuiApplicationExtensionLoader() {
  gDuiApplicationExtensionLoaderStub->DuiApplicationExtensionLoaderConstructor();
}


#endif
