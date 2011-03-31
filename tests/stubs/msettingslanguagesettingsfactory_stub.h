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

#ifndef MSETTINGSLANGUAGESETTINGSFACTORY_STUB
#define MSETTINGSLANGUAGESETTINGSFACTORY_STUB

#include "msettingslanguagesettingsfactory.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageSettingsFactoryStub : public StubBase
{
public:
    virtual MWidgetController *createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
    virtual void createChildren(QGraphicsLinearLayout *layout, const MSettingsLanguageNode &node, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore);
};

// 2. IMPLEMENT STUB
MWidgetController *MSettingsLanguageSettingsFactoryStub::createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const MSettingsLanguageSettings * >(&settingsItem));
    params.append(new Parameter<MSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("createWidget", params);
    return stubReturnValue<MWidgetController *>("createWidget");
}

void MSettingsLanguageSettingsFactoryStub::createChildren(QGraphicsLinearLayout *layout, const MSettingsLanguageNode &node, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLinearLayout * >(layout));
    params.append(new Parameter<const MSettingsLanguageNode & >(node));
    params.append(new Parameter<MSettingsLanguageWidget & >(rootWidget));
    params.append(new Parameter<MDataStore * >(dataStore));
    stubMethodEntered("createChildren", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageSettingsFactoryStub gDefaultMSettingsLanguageSettingsFactoryStub;
MSettingsLanguageSettingsFactoryStub *gMSettingsLanguageSettingsFactoryStub = &gDefaultMSettingsLanguageSettingsFactoryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetController *MSettingsLanguageSettingsFactory::createWidget(const MSettingsLanguageSettings &settingsItem, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    return gMSettingsLanguageSettingsFactoryStub->createWidget(settingsItem, rootWidget, dataStore);
}

void MSettingsLanguageSettingsFactory::createChildren(QGraphicsLinearLayout *layout, const MSettingsLanguageNode &node, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore)
{
    gMSettingsLanguageSettingsFactoryStub->createChildren(layout, node, rootWidget, dataStore);
}


#endif
