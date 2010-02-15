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

#ifndef DUISETTINGSLANGUAGESELECTIONFACTORY_STUB
#define DUISETTINGSLANGUAGESELECTIONFACTORY_STUB

#include "duisettingslanguageselectionfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageSelectionFactoryStub : public StubBase
{
public:
    virtual DuiWidgetController *createWidget(const DuiSettingsLanguageSelection &settingsSelection, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
};

// 2. IMPLEMENT STUB
DuiWidgetController *DuiSettingsLanguageSelectionFactoryStub::createWidget(const DuiSettingsLanguageSelection &settingsSelection, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiSettingsLanguageSelection * >(&settingsSelection));
    params.append(new Parameter<DuiSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<DuiWidgetController *>("createWidget");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageSelectionFactoryStub gDefaultDuiSettingsLanguageSelectionFactoryStub;
DuiSettingsLanguageSelectionFactoryStub *gDuiSettingsLanguageSelectionFactoryStub = &gDefaultDuiSettingsLanguageSelectionFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetController *DuiSettingsLanguageSelectionFactory::createWidget(const DuiSettingsLanguageSelection &settingsSelection, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    return gDuiSettingsLanguageSelectionFactoryStub->createWidget(settingsSelection, rootWidget, dataStore);
}


#endif
