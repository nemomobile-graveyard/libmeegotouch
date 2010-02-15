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

#ifndef DUIFEEDBACKSTUB_H
#define DUIFEEDBACKSTUB_H

#include "duifeedback.h"
#include <stubbase.h>

#include <QObject>
#include <QString>

class DuiFeedbackStub : public StubBase
{
public:
    virtual void DuiFeedbackConstructor(QObject *parent);
    virtual void DuiFeedbackDestructor();
    virtual QString name() const;
    virtual void play();
};

void DuiFeedbackStub::DuiFeedbackConstructor(QObject *)
{
}

void DuiFeedbackStub::DuiFeedbackDestructor()
{
}

QString DuiFeedbackStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

void DuiFeedbackStub::play()
{
    stubMethodEntered("play");
}

DuiFeedbackStub gDefaultDuiFeedbackStub;
DuiFeedbackStub *gDuiFeedbackStub = &gDefaultDuiFeedbackStub;

DuiFeedback::DuiFeedback(const QString &, QObject *) : d_ptr(0)
{
}

DuiFeedback::~DuiFeedback()
{
}

QString DuiFeedback::name() const
{
    return gDuiFeedbackStub->name();
}

void DuiFeedback::play() const
{
    gDuiFeedbackStub->play();
}

#endif

