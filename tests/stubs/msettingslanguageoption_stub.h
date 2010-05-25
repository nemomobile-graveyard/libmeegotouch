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

#ifndef MSETTINGSLANGUAGEOPTION_STUB
#define MSETTINGSLANGUAGEOPTION_STUB

#include "msettingslanguageoption.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageOptionStub : public StubBase
{
public:
    virtual void MSettingsLanguageOptionConstructor(const QString &title, int value);
    virtual void MSettingsLanguageOptionDestructor();
    virtual QString title() const;
    virtual int value() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageOptionStub::MSettingsLanguageOptionConstructor(const QString &title, int value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(title));
    params.append(new Parameter<int>(value));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageOptionStub::MSettingsLanguageOptionDestructor()
{

}
QString MSettingsLanguageOptionStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

int MSettingsLanguageOptionStub::value() const
{
    stubMethodEntered("value");
    return stubReturnValue<int>("value");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageOptionStub gDefaultMSettingsLanguageOptionStub;
MSettingsLanguageOptionStub *gMSettingsLanguageOptionStub = &gDefaultMSettingsLanguageOptionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageOption::MSettingsLanguageOption(const QString &title, int value)
  : value_( 0 )
{
    gMSettingsLanguageOptionStub->MSettingsLanguageOptionConstructor(title, value);
}

MSettingsLanguageOption::~MSettingsLanguageOption()
{
    gMSettingsLanguageOptionStub->MSettingsLanguageOptionDestructor();
}

QString MSettingsLanguageOption::title() const
{
    return gMSettingsLanguageOptionStub->title();
}

int MSettingsLanguageOption::value() const
{
    return gMSettingsLanguageOptionStub->value();
}


#endif
