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

#ifndef DUISETTINGSLANGUAGESELECTIONCONTROLLER_STUB
#define DUISETTINGSLANGUAGESELECTIONCONTROLLER_STUB

#include "duisettingslanguageselectioncontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageSelectionControllerStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageSelectionControllerConstructor(QObject *parent);
    virtual void DuiSettingsLanguageSelectionControllerDestructor();
    virtual void buttonClicked(DuiButton *button);
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageSelectionControllerStub::DuiSettingsLanguageSelectionControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiSettingsLanguageSelectionControllerStub::DuiSettingsLanguageSelectionControllerDestructor()
{

}
void DuiSettingsLanguageSelectionControllerStub::buttonClicked(DuiButton *button)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiButton * >(button));
    stubMethodEntered("buttonClicked", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageSelectionControllerStub gDefaultDuiSettingsLanguageSelectionControllerStub;
DuiSettingsLanguageSelectionControllerStub *gDuiSettingsLanguageSelectionControllerStub = &gDefaultDuiSettingsLanguageSelectionControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageSelectionController::DuiSettingsLanguageSelectionController(QObject *parent)
{
    gDuiSettingsLanguageSelectionControllerStub->DuiSettingsLanguageSelectionControllerConstructor(parent);
}

DuiSettingsLanguageSelectionController::~DuiSettingsLanguageSelectionController()
{
    gDuiSettingsLanguageSelectionControllerStub->DuiSettingsLanguageSelectionControllerDestructor();
}

void DuiSettingsLanguageSelectionController::buttonClicked(DuiButton *button)
{
    gDuiSettingsLanguageSelectionControllerStub->buttonClicked(button);
}


#endif
