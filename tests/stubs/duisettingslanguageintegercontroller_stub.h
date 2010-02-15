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

#ifndef DUISETTINGSLANGUAGEINTEGERCONTROLLER_STUB
#define DUISETTINGSLANGUAGEINTEGERCONTROLLER_STUB

#include "duisettingslanguageintegercontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageIntegerControllerStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageIntegerControllerConstructor(QObject *parent);
    virtual void DuiSettingsLanguageIntegerControllerDestructor();
    virtual void changeValue(int newValue);
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageIntegerControllerStub::DuiSettingsLanguageIntegerControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiSettingsLanguageIntegerControllerStub::DuiSettingsLanguageIntegerControllerDestructor()
{

}
void DuiSettingsLanguageIntegerControllerStub::changeValue(int newValue)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(newValue));
    stubMethodEntered("changeValue", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageIntegerControllerStub gDefaultDuiSettingsLanguageIntegerControllerStub;
DuiSettingsLanguageIntegerControllerStub *gDuiSettingsLanguageIntegerControllerStub = &gDefaultDuiSettingsLanguageIntegerControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageIntegerController::DuiSettingsLanguageIntegerController(QObject *parent)
{
    gDuiSettingsLanguageIntegerControllerStub->DuiSettingsLanguageIntegerControllerConstructor(parent);
}

DuiSettingsLanguageIntegerController::~DuiSettingsLanguageIntegerController()
{
    gDuiSettingsLanguageIntegerControllerStub->DuiSettingsLanguageIntegerControllerDestructor();
}

void DuiSettingsLanguageIntegerController::changeValue(int newValue)
{
    gDuiSettingsLanguageIntegerControllerStub->changeValue(newValue);
}


#endif
