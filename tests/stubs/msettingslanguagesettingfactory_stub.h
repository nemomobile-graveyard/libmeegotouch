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

#ifndef MSETTINGSLANGUAGESETTINGFACTORY_STUB
#define MSETTINGSLANGUAGESETTINGFACTORY_STUB

#include "msettingslanguagegroupfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageSettingFactoryStub : public StubBase
{
public:
    virtual MWidgetController *createWidget(const MSettingsLanguageSetting &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
};

// 2. IMPLEMENT STUB
MWidgetController *MSettingsLanguageSettingFactoryStub::createWidget(const MSettingsLanguageSetting &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MSettingsLanguageSetting & >(settingsItem));
    params.append(new Parameter<MSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<MWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageSettingFactoryStub gDefaultMSettingsLanguageSettingFactoryStub;
MSettingsLanguageSettingFactoryStub *gMSettingsLanguageSettingFactoryStub = &gDefaultMSettingsLanguageSettingFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetController *MSettingsLanguageSettingFactory::createWidget(const MSettingsLanguageSetting &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    return gMSettingsLanguageSettingFactoryStub->createWidget(settingsItem, rootWidget, dataStore);
}


#endif
