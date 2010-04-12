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

#ifndef MSETTINGSLANGUAGEBOOLEANCONTROLLER_STUB
#define MSETTINGSLANGUAGEBOOLEANCONTROLLER_STUB

#include "msettingslanguagebooleancontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageBooleanControllerStub : public StubBase
{
public:
    virtual void MSettingsLanguageBooleanControllerConstructor(QObject *parent);
    virtual void MSettingsLanguageBooleanControllerDestructor();
    virtual void buttonToggled(bool checked);
};

// 2. IMPLEMENT STUB
void MSettingsLanguageBooleanControllerStub::MSettingsLanguageBooleanControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MSettingsLanguageBooleanControllerStub::MSettingsLanguageBooleanControllerDestructor()
{

}
void MSettingsLanguageBooleanControllerStub::buttonToggled(bool checked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(checked));
    stubMethodEntered("buttonToggled", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageBooleanControllerStub gDefaultMSettingsLanguageBooleanControllerStub;
MSettingsLanguageBooleanControllerStub *gMSettingsLanguageBooleanControllerStub = &gDefaultMSettingsLanguageBooleanControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageBooleanController::MSettingsLanguageBooleanController(QObject *parent)
{
    gMSettingsLanguageBooleanControllerStub->MSettingsLanguageBooleanControllerConstructor(parent);
}

MSettingsLanguageBooleanController::~MSettingsLanguageBooleanController()
{
    gMSettingsLanguageBooleanControllerStub->MSettingsLanguageBooleanControllerDestructor();
}

void MSettingsLanguageBooleanController::buttonToggled(bool checked)
{
    gMSettingsLanguageBooleanControllerStub->buttonToggled(checked);
}


#endif
