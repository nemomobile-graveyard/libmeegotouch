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

#ifndef DUISETTINGSLANGUAGEINTEGERFACTORY_STUB
#define DUISETTINGSLANGUAGEINTEGERFACTORY_STUB

#include "duisettingslanguageintegerfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageIntegerFactoryStub : public StubBase
{
public:
    virtual DuiWidgetController *createWidget(const DuiSettingsLanguageInteger &settingsInteger, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
};

// 2. IMPLEMENT STUB
DuiWidgetController *DuiSettingsLanguageIntegerFactoryStub::createWidget(const DuiSettingsLanguageInteger &settingsInteger, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiSettingsLanguageInteger * >(&settingsInteger));
    params.append(new Parameter<DuiSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<DuiWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageIntegerFactoryStub gDefaultDuiSettingsLanguageIntegerFactoryStub;
DuiSettingsLanguageIntegerFactoryStub *gDuiSettingsLanguageIntegerFactoryStub = &gDefaultDuiSettingsLanguageIntegerFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetController *DuiSettingsLanguageIntegerFactory::createWidget(const DuiSettingsLanguageInteger &settingsInteger, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    return gDuiSettingsLanguageIntegerFactoryStub->createWidget(settingsInteger, rootWidget, dataStore);
}


#endif
