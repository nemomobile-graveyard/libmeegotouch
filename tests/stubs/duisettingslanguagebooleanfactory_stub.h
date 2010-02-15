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

#ifndef DUISETTINGSLANGUAGEBOOLEANFACTORY_STUB
#define DUISETTINGSLANGUAGEBOOLEANFACTORY_STUB

#include "duisettingslanguagebooleanfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageBooleanFactoryStub : public StubBase
{
public:
    virtual DuiWidgetController *createWidget(const DuiSettingsLanguageBoolean &settingsBool, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
};

// 2. IMPLEMENT STUB
DuiWidgetController *DuiSettingsLanguageBooleanFactoryStub::createWidget(const DuiSettingsLanguageBoolean &settingsBool, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiSettingsLanguageBoolean * >(&settingsBool));
    params.append(new Parameter<DuiSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<DuiWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageBooleanFactoryStub gDefaultDuiSettingsLanguageBooleanFactoryStub;
DuiSettingsLanguageBooleanFactoryStub *gDuiSettingsLanguageBooleanFactoryStub = &gDefaultDuiSettingsLanguageBooleanFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetController *DuiSettingsLanguageBooleanFactory::createWidget(const DuiSettingsLanguageBoolean &settingsBool, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    return gDuiSettingsLanguageBooleanFactoryStub->createWidget(settingsBool, rootWidget, dataStore);
}


#endif
