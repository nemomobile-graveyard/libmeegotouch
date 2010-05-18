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

#ifndef MSETTINGSLANGUAGESETTINGS_STUB
#define MSETTINGSLANGUAGESETTINGS_STUB

#include "msettingslanguagesettings.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageSettingsStub : public StubBase
{
public:
    virtual void MSettingsLanguageSettingsConstructor();
    virtual void MSettingsLanguageSettingsDestructor();
};

// 2. IMPLEMENT STUB
void MSettingsLanguageSettingsStub::MSettingsLanguageSettingsConstructor()
{

}
void MSettingsLanguageSettingsStub::MSettingsLanguageSettingsDestructor()
{

}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageSettingsStub gDefaultMSettingsLanguageSettingsStub;
MSettingsLanguageSettingsStub *gMSettingsLanguageSettingsStub = &gDefaultMSettingsLanguageSettingsStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageSettings::MSettingsLanguageSettings()
  : _group(false)
{
    gMSettingsLanguageSettingsStub->MSettingsLanguageSettingsConstructor();
}

MSettingsLanguageSettings::~MSettingsLanguageSettings()
{
    gMSettingsLanguageSettingsStub->MSettingsLanguageSettingsDestructor();
}


#endif
