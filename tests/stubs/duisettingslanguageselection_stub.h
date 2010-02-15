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

#ifndef DUISETTINGSLANGUAGESELECTION_STUB
#define DUISETTINGSLANGUAGESELECTION_STUB

#include "duisettingslanguageselection.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageSelectionStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageSelectionConstructor(const QString &key);
    virtual void DuiSettingsLanguageSelectionDestructor();
    virtual QString key() const;
    virtual DuiSettingsLanguageOption *addOption(const QString &title, int value);
    virtual DuiSettingsLanguageOption *addOption(DuiSettingsLanguageOption *option);
    virtual uint numOptions() const;
    virtual QList<const DuiSettingsLanguageOption *> options() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageSelectionStub::DuiSettingsLanguageSelectionConstructor(const QString &key)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    stubMethodEntered("constructor", params);
}
void DuiSettingsLanguageSelectionStub::DuiSettingsLanguageSelectionDestructor()
{

}
QString DuiSettingsLanguageSelectionStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelectionStub::addOption(const QString &title, int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString >(title));
    params.append(new Parameter<int >(value));
    stubMethodEntered("addOption", params);
    return stubReturnValue<DuiSettingsLanguageOption *>("addOption");
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelectionStub::addOption(DuiSettingsLanguageOption *option)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiSettingsLanguageOption * >(option));
    stubMethodEntered("addOption", params);
    return stubReturnValue<DuiSettingsLanguageOption *>("addOption");
}

uint DuiSettingsLanguageSelectionStub::numOptions() const
{
    stubMethodEntered("numOptions");
    return stubReturnValue<uint>("numOptions");
}

QList<const DuiSettingsLanguageOption *> DuiSettingsLanguageSelectionStub::options() const
{
    stubMethodEntered("options");
    return stubReturnValue<QList<const DuiSettingsLanguageOption *> >("options");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageSelectionStub gDefaultDuiSettingsLanguageSelectionStub;
DuiSettingsLanguageSelectionStub *gDuiSettingsLanguageSelectionStub = &gDefaultDuiSettingsLanguageSelectionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageSelection::DuiSettingsLanguageSelection(const QString &key)
{
    gDuiSettingsLanguageSelectionStub->DuiSettingsLanguageSelectionConstructor(key);
}

DuiSettingsLanguageSelection::~DuiSettingsLanguageSelection()
{
    gDuiSettingsLanguageSelectionStub->DuiSettingsLanguageSelectionDestructor();
}

QString DuiSettingsLanguageSelection::key() const
{
    return gDuiSettingsLanguageSelectionStub->key();
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelection::addOption(const QString &title, int value)
{
    return gDuiSettingsLanguageSelectionStub->addOption(title, value);
}

DuiSettingsLanguageOption *DuiSettingsLanguageSelection::addOption(DuiSettingsLanguageOption *option)
{
    return gDuiSettingsLanguageSelectionStub->addOption(option);
}

uint DuiSettingsLanguageSelection::numOptions() const
{
    return gDuiSettingsLanguageSelectionStub->numOptions();
}

QList<const DuiSettingsLanguageOption *> DuiSettingsLanguageSelection::options() const
{
    return gDuiSettingsLanguageSelectionStub->options();
}


#endif
