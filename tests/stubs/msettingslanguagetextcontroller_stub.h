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

#ifndef MSETTINGSLANGUAGETEXTCONTROLLER_STUB
#define MSETTINGSLANGUAGETEXTCONTROLLER_STUB

#include "msettingslanguagetextcontroller.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageTextControllerStub : public StubBase
{
public:
    virtual void MSettingsLanguageTextControllerConstructor(QObject *parent);
    virtual void MSettingsLanguageTextControllerDestructor();
    virtual void textEditLostFocus(Qt::FocusReason reason);
};

// 2. IMPLEMENT STUB
void MSettingsLanguageTextControllerStub::MSettingsLanguageTextControllerConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MSettingsLanguageTextControllerStub::MSettingsLanguageTextControllerDestructor()
{

}
void MSettingsLanguageTextControllerStub::textEditLostFocus(Qt::FocusReason reason)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::FocusReason >(reason));
    stubMethodEntered("textEditLostFocus", params);
}



// 3. CREATE A STUB INSTANCE
MSettingsLanguageTextControllerStub gDefaultMSettingsLanguageTextControllerStub;
MSettingsLanguageTextControllerStub *gMSettingsLanguageTextControllerStub = &gDefaultMSettingsLanguageTextControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageTextController::MSettingsLanguageTextController(QObject *parent)
{
    gMSettingsLanguageTextControllerStub->MSettingsLanguageTextControllerConstructor(parent);
}

MSettingsLanguageTextController::~MSettingsLanguageTextController()
{
    gMSettingsLanguageTextControllerStub->MSettingsLanguageTextControllerDestructor();
}

void MSettingsLanguageTextController::textEditLostFocus(Qt::FocusReason reason)
{
    gMSettingsLanguageTextControllerStub->textEditLostFocus(reason);
}


#endif
