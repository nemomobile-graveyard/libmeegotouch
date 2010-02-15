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

#ifndef DUISETTINGSLANGUAGEBINARY_STUB
#define DUISETTINGSLANGUAGEBINARY_STUB

#include "duisettingslanguagebinary.h"
#include <stubbase.h>
#include <QStringList>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageBinaryStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageBinaryConstructor();
    virtual void DuiSettingsLanguageBinaryDestructor();
    virtual QStringList keys() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageBinaryStub::DuiSettingsLanguageBinaryConstructor()
{

}
void DuiSettingsLanguageBinaryStub::DuiSettingsLanguageBinaryDestructor()
{

}
QStringList DuiSettingsLanguageBinaryStub::keys() const
{
    stubMethodEntered("keys");
    return stubReturnValue<QStringList>("keys");
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageBinaryStub gDefaultDuiSettingsLanguageBinaryStub;
DuiSettingsLanguageBinaryStub *gDuiSettingsLanguageBinaryStub = &gDefaultDuiSettingsLanguageBinaryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageBinary::DuiSettingsLanguageBinary()
{
    gDuiSettingsLanguageBinaryStub->DuiSettingsLanguageBinaryConstructor();
}

DuiSettingsLanguageBinary::~DuiSettingsLanguageBinary()
{
    gDuiSettingsLanguageBinaryStub->DuiSettingsLanguageBinaryDestructor();
}

QStringList DuiSettingsLanguageBinary::keys() const
{
    return gDuiSettingsLanguageBinaryStub->keys();
}


#endif
