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

#ifndef MSETTINGSLANGUAGETEXT_STUB
#define MSETTINGSLANGUAGETEXT_STUB

#include "msettingslanguagetext.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageTextStub : public StubBase
{
public:
    virtual void MSettingsLanguageTextConstructor(const QString &key, const QString &title);
    virtual void MSettingsLanguageTextDestructor();
    virtual QString key() const;
    virtual QString title() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageTextStub::MSettingsLanguageTextConstructor(const QString &key, const QString &title)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(key));
    params.append(new Parameter<QString>(title));
    stubMethodEntered("constructor", params);
}
void MSettingsLanguageTextStub::MSettingsLanguageTextDestructor()
{

}
QString MSettingsLanguageTextStub::key() const
{
    stubMethodEntered("key");
    return stubReturnValue<QString>("key");
}

QString MSettingsLanguageTextStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageTextStub gDefaultMSettingsLanguageTextStub;
MSettingsLanguageTextStub *gMSettingsLanguageTextStub = &gDefaultMSettingsLanguageTextStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageText::MSettingsLanguageText(const QString &key, const QString &title)
{
    gMSettingsLanguageTextStub->MSettingsLanguageTextConstructor(key, title);
}

MSettingsLanguageText::~MSettingsLanguageText()
{
    gMSettingsLanguageTextStub->MSettingsLanguageTextDestructor();
}

QString MSettingsLanguageText::key() const
{
    return gMSettingsLanguageTextStub->key();
}

QString MSettingsLanguageText::title() const
{
    return gMSettingsLanguageTextStub->title();
}


#endif
