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

#ifndef MSETTINGSLANGUAGEGROUPFACTORY_STUB
#define MSETTINGSLANGUAGEGROUPFACTORY_STUB

#include "msettingslanguagegroupfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageGroupFactoryStub : public StubBase {
  public:
  virtual MWidgetController * createWidget(const MSettingsLanguageGroup &settingsGroup, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
}; 

// 2. IMPLEMENT STUB
MWidgetController * MSettingsLanguageGroupFactoryStub::createWidget(const MSettingsLanguageGroup &settingsGroup, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore) {
  QList<ParameterBase*> params;
  params.append( new Parameter<const MSettingsLanguageGroup * >(&settingsGroup));
  params.append( new Parameter<MSettingsLanguageWidget & >(rootWidget));
  params.append( new Parameter<MDataStore * >(dataStore));
  stubMethodEntered("createWidget",params);
  return stubReturnValue<MWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageGroupFactoryStub gDefaultMSettingsLanguageGroupFactoryStub;
MSettingsLanguageGroupFactoryStub* gMSettingsLanguageGroupFactoryStub = &gDefaultMSettingsLanguageGroupFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetController * MSettingsLanguageGroupFactory::createWidget(const MSettingsLanguageGroup &settingsGroup, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore) {
  return gMSettingsLanguageGroupFactoryStub->createWidget(settingsGroup, rootWidget, dataStore);
}


#endif
