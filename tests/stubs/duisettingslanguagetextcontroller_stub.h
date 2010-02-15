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

#ifndef DUISETTINGSLANGUAGETEXTCONTROLLER_STUB
#define DUISETTINGSLANGUAGETEXTCONTROLLER_STUB

#include "duisettingslanguagetextcontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageTextControllerStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageTextControllerConstructor(QObject *parent);
    virtual void DuiSettingsLanguageTextControllerDestructor();
    virtual void textEditLostFocus(Qt::FocusReason reason);
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageTextControllerStub::DuiSettingsLanguageTextControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiSettingsLanguageTextControllerStub::DuiSettingsLanguageTextControllerDestructor()
{

}
void DuiSettingsLanguageTextControllerStub::textEditLostFocus(Qt::FocusReason reason)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::FocusReason >(reason));
    stubMethodEntered("textEditLostFocus", params);
}



// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageTextControllerStub gDefaultDuiSettingsLanguageTextControllerStub;
DuiSettingsLanguageTextControllerStub *gDuiSettingsLanguageTextControllerStub = &gDefaultDuiSettingsLanguageTextControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageTextController::DuiSettingsLanguageTextController(QObject *parent)
{
    gDuiSettingsLanguageTextControllerStub->DuiSettingsLanguageTextControllerConstructor(parent);
}

DuiSettingsLanguageTextController::~DuiSettingsLanguageTextController()
{
    gDuiSettingsLanguageTextControllerStub->DuiSettingsLanguageTextControllerDestructor();
}

void DuiSettingsLanguageTextController::textEditLostFocus(Qt::FocusReason reason)
{
    gDuiSettingsLanguageTextControllerStub->textEditLostFocus(reason);
}


#endif
