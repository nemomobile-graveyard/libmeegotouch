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

#ifndef DUISETTINGSLANGUAGEOPTION_STUB
#define DUISETTINGSLANGUAGEOPTION_STUB

#include "duisettingslanguageoption.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageOptionStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageOptionConstructor(const QString &title, int value);
    virtual void DuiSettingsLanguageOptionDestructor();
    virtual QString title() const;
    virtual int value() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageOptionStub::DuiSettingsLanguageOptionConstructor(const QString &title, int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(title));
    params.append(new Parameter<int>(value));
    stubMethodEntered("constructor", params);
}
void DuiSettingsLanguageOptionStub::DuiSettingsLanguageOptionDestructor()
{

}
QString DuiSettingsLanguageOptionStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

int DuiSettingsLanguageOptionStub::value() const
{
    stubMethodEntered("value");
    return stubReturnValue<int>("value");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageOptionStub gDefaultDuiSettingsLanguageOptionStub;
DuiSettingsLanguageOptionStub *gDuiSettingsLanguageOptionStub = &gDefaultDuiSettingsLanguageOptionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageOption::DuiSettingsLanguageOption(const QString &title, int value)
{
    gDuiSettingsLanguageOptionStub->DuiSettingsLanguageOptionConstructor(title, value);
}

DuiSettingsLanguageOption::~DuiSettingsLanguageOption()
{
    gDuiSettingsLanguageOptionStub->DuiSettingsLanguageOptionDestructor();
}

QString DuiSettingsLanguageOption::title() const
{
    return gDuiSettingsLanguageOptionStub->title();
}

int DuiSettingsLanguageOption::value() const
{
    return gDuiSettingsLanguageOptionStub->value();
}


#endif
