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

#ifndef MSETTINGSLANGUAGESELECTIONCONTROLLER_STUB
#define MSETTINGSLANGUAGESELECTIONCONTROLLER_STUB

#include "msettingslanguageselectioncontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageSelectionControllerStub : public StubBase
{
public:
    virtual void MSettingsLanguageSelectionControllerConstructor(QObject *parent);
    virtual void MSettingsLanguageSelectionControllerDestructor();
    virtual void buttonClicked(MButton *button);
};

// 2. IMPLEMENT STUB
void MSettingsLanguageSelectionControllerStub::MSettingsLanguageSelectionControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MSettingsLanguageSelectionControllerStub::MSettingsLanguageSelectionControllerDestructor()
{

}
void MSettingsLanguageSelectionControllerStub::buttonClicked(MButton *button)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MButton * >(button));
    stubMethodEntered("buttonClicked", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageSelectionControllerStub gDefaultMSettingsLanguageSelectionControllerStub;
MSettingsLanguageSelectionControllerStub *gMSettingsLanguageSelectionControllerStub = &gDefaultMSettingsLanguageSelectionControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageSelectionController::MSettingsLanguageSelectionController(QObject *parent)
{
    gMSettingsLanguageSelectionControllerStub->MSettingsLanguageSelectionControllerConstructor(parent);
}

MSettingsLanguageSelectionController::~MSettingsLanguageSelectionController()
{
    gMSettingsLanguageSelectionControllerStub->MSettingsLanguageSelectionControllerDestructor();
}

void MSettingsLanguageSelectionController::buttonClicked(MButton *button)
{
    gMSettingsLanguageSelectionControllerStub->buttonClicked(button);
}


#endif
