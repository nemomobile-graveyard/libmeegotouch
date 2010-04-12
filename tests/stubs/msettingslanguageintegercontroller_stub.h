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

#ifndef MSETTINGSLANGUAGEINTEGERCONTROLLER_STUB
#define MSETTINGSLANGUAGEINTEGERCONTROLLER_STUB

#include "msettingslanguageintegercontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageIntegerControllerStub : public StubBase
{
public:
    virtual void MSettingsLanguageIntegerControllerConstructor(QObject *parent);
    virtual void MSettingsLanguageIntegerControllerDestructor();
    virtual void changeValue(int newValue);
};

// 2. IMPLEMENT STUB
void MSettingsLanguageIntegerControllerStub::MSettingsLanguageIntegerControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MSettingsLanguageIntegerControllerStub::MSettingsLanguageIntegerControllerDestructor()
{

}
void MSettingsLanguageIntegerControllerStub::changeValue(int newValue)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newValue));
    stubMethodEntered("changeValue", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageIntegerControllerStub gDefaultMSettingsLanguageIntegerControllerStub;
MSettingsLanguageIntegerControllerStub *gMSettingsLanguageIntegerControllerStub = &gDefaultMSettingsLanguageIntegerControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageIntegerController::MSettingsLanguageIntegerController(QObject *parent)
{
    gMSettingsLanguageIntegerControllerStub->MSettingsLanguageIntegerControllerConstructor(parent);
}

MSettingsLanguageIntegerController::~MSettingsLanguageIntegerController()
{
    gMSettingsLanguageIntegerControllerStub->MSettingsLanguageIntegerControllerDestructor();
}

void MSettingsLanguageIntegerController::changeValue(int newValue)
{
    gMSettingsLanguageIntegerControllerStub->changeValue(newValue);
}


#endif
