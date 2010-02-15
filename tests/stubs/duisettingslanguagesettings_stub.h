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

#ifndef DUISETTINGSLANGUAGESETTINGS_STUB
#define DUISETTINGSLANGUAGESETTINGS_STUB

#include "duisettingslanguagesettings.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageSettingsStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageSettingsConstructor();
    virtual void DuiSettingsLanguageSettingsDestructor();
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageSettingsStub::DuiSettingsLanguageSettingsConstructor()
{

}
void DuiSettingsLanguageSettingsStub::DuiSettingsLanguageSettingsDestructor()
{

}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageSettingsStub gDefaultDuiSettingsLanguageSettingsStub;
DuiSettingsLanguageSettingsStub *gDuiSettingsLanguageSettingsStub = &gDefaultDuiSettingsLanguageSettingsStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageSettings::DuiSettingsLanguageSettings()
{
    gDuiSettingsLanguageSettingsStub->DuiSettingsLanguageSettingsConstructor();
}

DuiSettingsLanguageSettings::~DuiSettingsLanguageSettings()
{
    gDuiSettingsLanguageSettingsStub->DuiSettingsLanguageSettingsDestructor();
}


#endif
