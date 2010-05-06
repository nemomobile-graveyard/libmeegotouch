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

#ifndef MSETTINGSLANGUAGEBINARY_STUB
#define MSETTINGSLANGUAGEBINARY_STUB

#include "msettingslanguagebinary.h"
#include <stubbase.h>
#include <QStringList>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageBinaryStub : public StubBase
{
public:
    virtual void MSettingsLanguageBinaryConstructor();
    virtual void MSettingsLanguageBinaryDestructor();
    virtual QStringList keys() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageBinaryStub::MSettingsLanguageBinaryConstructor()
{

}
void MSettingsLanguageBinaryStub::MSettingsLanguageBinaryDestructor()
{

}
QStringList MSettingsLanguageBinaryStub::keys() const
{
    stubMethodEntered("keys");
    return stubReturnValue<QStringList>("keys");
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageBinaryStub gDefaultMSettingsLanguageBinaryStub;
MSettingsLanguageBinaryStub *gMSettingsLanguageBinaryStub = &gDefaultMSettingsLanguageBinaryStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageBinary::MSettingsLanguageBinary()
{
    gMSettingsLanguageBinaryStub->MSettingsLanguageBinaryConstructor();
}

MSettingsLanguageBinary::~MSettingsLanguageBinary()
{
    gMSettingsLanguageBinaryStub->MSettingsLanguageBinaryDestructor();
}

QStringList MSettingsLanguageBinary::keys() const
{
    return gMSettingsLanguageBinaryStub->keys();
}


#endif
