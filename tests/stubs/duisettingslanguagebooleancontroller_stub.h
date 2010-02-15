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

#ifndef DUISETTINGSLANGUAGEBOOLEANCONTROLLER_STUB
#define DUISETTINGSLANGUAGEBOOLEANCONTROLLER_STUB

#include "duisettingslanguagebooleancontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageBooleanControllerStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageBooleanControllerConstructor(QObject *parent);
    virtual void DuiSettingsLanguageBooleanControllerDestructor();
    virtual void buttonToggled(bool checked);
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageBooleanControllerStub::DuiSettingsLanguageBooleanControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiSettingsLanguageBooleanControllerStub::DuiSettingsLanguageBooleanControllerDestructor()
{

}
void DuiSettingsLanguageBooleanControllerStub::buttonToggled(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("buttonToggled", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageBooleanControllerStub gDefaultDuiSettingsLanguageBooleanControllerStub;
DuiSettingsLanguageBooleanControllerStub *gDuiSettingsLanguageBooleanControllerStub = &gDefaultDuiSettingsLanguageBooleanControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageBooleanController::DuiSettingsLanguageBooleanController(QObject *parent)
{
    gDuiSettingsLanguageBooleanControllerStub->DuiSettingsLanguageBooleanControllerConstructor(parent);
}

DuiSettingsLanguageBooleanController::~DuiSettingsLanguageBooleanController()
{
    gDuiSettingsLanguageBooleanControllerStub->DuiSettingsLanguageBooleanControllerDestructor();
}

void DuiSettingsLanguageBooleanController::buttonToggled(bool checked)
{
    gDuiSettingsLanguageBooleanControllerStub->buttonToggled(checked);
}


#endif
