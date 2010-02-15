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

#ifndef DUIFEEDBACKPLAYERPRIVATE_STUB
#define DUIFEEDBACKPLAYERPRIVATE_STUB

#include <feedback/duifeedbackplayer_p.h>
#include <stubbase.h>

class DuiFeedbackPlayerPrivateStub : public StubBase
{
public:
    virtual void DuiFeedbackPlayerPrivateConstructor(QObject *parent = 0);
    virtual void DuiFeedbackPlayerPrivateDestructor();
    virtual bool init(const QString &applicationName);
};

void DuiFeedbackPlayerPrivateStub::DuiFeedbackPlayerPrivateConstructor(QObject *parent)
{
    Q_UNUSED(parent);
}

void DuiFeedbackPlayerPrivateStub::DuiFeedbackPlayerPrivateDestructor()
{
}

bool DuiFeedbackPlayerPrivateStub::init(const QString &applicationName)
{
    Q_UNUSED(applicationName);
    stubMethodEntered("init");
    return stubReturnValue<bool>("init");
}

DuiFeedbackPlayerPrivateStub gDefaultDuiFeedbackPlayerPrivateStub;
DuiFeedbackPlayerPrivateStub *gDuiFeedbackPlayerPrivateStub = &gDefaultDuiFeedbackPlayerPrivateStub;


DuiFeedbackPlayerPrivate::DuiFeedbackPlayerPrivate(QObject *parent)
    : QObject(parent)
{
    gDuiFeedbackPlayerPrivateStub->DuiFeedbackPlayerPrivateConstructor(parent);
}

DuiFeedbackPlayerPrivate::~DuiFeedbackPlayerPrivate()
{
    gDuiFeedbackPlayerPrivateStub->DuiFeedbackPlayerPrivateDestructor();
}

bool DuiFeedbackPlayerPrivate::init(const QString &applicationName)
{
    return gDuiFeedbackPlayerPrivateStub->init(applicationName);
}
#endif
