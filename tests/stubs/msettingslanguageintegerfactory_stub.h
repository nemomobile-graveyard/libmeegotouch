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

#ifndef MSETTINGSLANGUAGEINTEGERFACTORY_STUB
#define MSETTINGSLANGUAGEINTEGERFACTORY_STUB

#include "msettingslanguageintegerfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageIntegerFactoryStub : public StubBase
{
public:
    virtual MWidgetController *createWidget(const MSettingsLanguageInteger &settingsInteger, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
};

// 2. IMPLEMENT STUB
MWidgetController *MSettingsLanguageIntegerFactoryStub::createWidget(const MSettingsLanguageInteger &settingsInteger, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MSettingsLanguageInteger * >(&settingsInteger));
    params.append(new Parameter<MSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<MWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageIntegerFactoryStub gDefaultMSettingsLanguageIntegerFactoryStub;
MSettingsLanguageIntegerFactoryStub *gMSettingsLanguageIntegerFactoryStub = &gDefaultMSettingsLanguageIntegerFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetController *MSettingsLanguageIntegerFactory::createWidget(const MSettingsLanguageInteger &settingsInteger, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    return gMSettingsLanguageIntegerFactoryStub->createWidget(settingsInteger, rootWidget, dataStore);
}


#endif
