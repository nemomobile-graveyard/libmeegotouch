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

#ifndef MSETTINGSLANGUAGEBOOLEAN_STUB
#define MSETTINGSLANGUAGEBOOLEAN_STUB

#include "msettingslanguageboolean.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageBooleanStub : public StubBase
{
public:
    virtual void MSettingsLanguageBooleanConstructor(const QString &key, const QString &title);
    virtual void MSettingsLanguageBooleanDestructor();
    virtual QString key() const;
    virtual QString title() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageBooleanStub::MSettingsLanguageBooleanConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageBooleanStub::MSettingsLanguageBooleanDestructor()
{

}
QString MSettingsLanguageBooleanStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString MSettingsLanguageBooleanStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageBooleanStub gDefaultMSettingsLanguageBooleanStub;
MSettingsLanguageBooleanStub *gMSettingsLanguageBooleanStub = &gDefaultMSettingsLanguageBooleanStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageBoolean::MSettingsLanguageBoolean(const QString &key, const QString &title)
{
    gMSettingsLanguageBooleanStub->MSettingsLanguageBooleanConstructor(key, title);
}

MSettingsLanguageBoolean::~MSettingsLanguageBoolean()
{
    gMSettingsLanguageBooleanStub->MSettingsLanguageBooleanDestructor();
}

QString MSettingsLanguageBoolean::key() const
{
    return gMSettingsLanguageBooleanStub->key();
}

QString MSettingsLanguageBoolean::title() const
{
    return gMSettingsLanguageBooleanStub->title();
}


#endif
