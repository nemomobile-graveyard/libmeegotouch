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

#ifndef DUISETTINGSLANGUAGEBOOLEAN_STUB
#define DUISETTINGSLANGUAGEBOOLEAN_STUB

#include "duisettingslanguageboolean.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageBooleanStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageBooleanConstructor(const QString &key, const QString &title);
    virtual void DuiSettingsLanguageBooleanDestructor();
    virtual QString key() const;
    virtual QString title() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageBooleanStub::DuiSettingsLanguageBooleanConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void DuiSettingsLanguageBooleanStub::DuiSettingsLanguageBooleanDestructor()
{

}
QString DuiSettingsLanguageBooleanStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString DuiSettingsLanguageBooleanStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageBooleanStub gDefaultDuiSettingsLanguageBooleanStub;
DuiSettingsLanguageBooleanStub *gDuiSettingsLanguageBooleanStub = &gDefaultDuiSettingsLanguageBooleanStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageBoolean::DuiSettingsLanguageBoolean(const QString &key, const QString &title)
{
    gDuiSettingsLanguageBooleanStub->DuiSettingsLanguageBooleanConstructor(key, title);
}

DuiSettingsLanguageBoolean::~DuiSettingsLanguageBoolean()
{
    gDuiSettingsLanguageBooleanStub->DuiSettingsLanguageBooleanDestructor();
}

QString DuiSettingsLanguageBoolean::key() const
{
    return gDuiSettingsLanguageBooleanStub->key();
}

QString DuiSettingsLanguageBoolean::title() const
{
    return gDuiSettingsLanguageBooleanStub->title();
}


#endif
