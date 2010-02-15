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

#ifndef DUISETTINGSLANGUAGESETTINGSFACTORY_STUB
#define DUISETTINGSLANGUAGESETTINGSFACTORY_STUB

#include "duisettingslanguagesettingsfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageSettingsFactoryStub : public StubBase
{
public:
    virtual DuiWidgetController *createWidget(const DuiSettingsLanguageSettings &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
    virtual void createChildren(QGraphicsLinearLayout &layout, const DuiSettingsLanguageNode &node, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
};

// 2. IMPLEMENT STUB
DuiWidgetController *DuiSettingsLanguageSettingsFactoryStub::createWidget(const DuiSettingsLanguageSettings &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const DuiSettingsLanguageSettings * >(&settingsItem));
    params.append(new Parameter<DuiSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<DuiWidgetController *>("createWidget");
}

void DuiSettingsLanguageSettingsFactoryStub::createChildren(QGraphicsLinearLayout &layout, const DuiSettingsLanguageNode &node, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLinearLayout & >(layout));
    params.append(new Parameter<const DuiSettingsLanguageNode & >(node));
    params.append(new Parameter<DuiSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<DuiDataStore * >(dataStore));
    stubMethodEntered("createChildren", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageSettingsFactoryStub gDefaultDuiSettingsLanguageSettingsFactoryStub;
DuiSettingsLanguageSettingsFactoryStub *gDuiSettingsLanguageSettingsFactoryStub = &gDefaultDuiSettingsLanguageSettingsFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetController *DuiSettingsLanguageSettingsFactory::createWidget(const DuiSettingsLanguageSettings &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    return gDuiSettingsLanguageSettingsFactoryStub->createWidget(settingsItem, rootWidget, dataStore);
}

void DuiSettingsLanguageSettingsFactory::createChildren(QGraphicsLinearLayout &layout, const DuiSettingsLanguageNode &node, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore)
{
    gDuiSettingsLanguageSettingsFactoryStub->createChildren(layout, node, rootWidget, dataStore);
}


#endif
