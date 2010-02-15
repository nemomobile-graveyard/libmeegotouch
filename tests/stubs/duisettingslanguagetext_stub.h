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

#ifndef DUISETTINGSLANGUAGETEXT_STUB
#define DUISETTINGSLANGUAGETEXT_STUB

#include "duisettingslanguagetext.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageTextStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageTextConstructor(const QString &key, const QString &title);
    virtual void DuiSettingsLanguageTextDestructor();
    virtual QString key() const;
    virtual QString title() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageTextStub::DuiSettingsLanguageTextConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void DuiSettingsLanguageTextStub::DuiSettingsLanguageTextDestructor()
{

}
QString DuiSettingsLanguageTextStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString DuiSettingsLanguageTextStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageTextStub gDefaultDuiSettingsLanguageTextStub;
DuiSettingsLanguageTextStub *gDuiSettingsLanguageTextStub = &gDefaultDuiSettingsLanguageTextStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageText::DuiSettingsLanguageText(const QString &key, const QString &title)
{
    gDuiSettingsLanguageTextStub->DuiSettingsLanguageTextConstructor(key, title);
}

DuiSettingsLanguageText::~DuiSettingsLanguageText()
{
    gDuiSettingsLanguageTextStub->DuiSettingsLanguageTextDestructor();
}

QString DuiSettingsLanguageText::key() const
{
    return gDuiSettingsLanguageTextStub->key();
}

QString DuiSettingsLanguageText::title() const
{
    return gDuiSettingsLanguageTextStub->title();
}


#endif
