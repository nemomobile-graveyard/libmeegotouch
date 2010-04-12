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

#ifndef MFEEDBACKSTUB_H
#define MFEEDBACKSTUB_H

#include "mfeedback.h"
#include <stubbase.h>

#include <QObject>
#include <QString>

class MFeedbackStub : public StubBase
{
public:
    virtual void MFeedbackConstructor(QObject *parent);
    virtual void MFeedbackDestructor();
    virtual QString name() const;
    virtual void play();
};

void MFeedbackStub::MFeedbackConstructor(QObject *)
{
}

void MFeedbackStub::MFeedbackDestructor()
{
}

QString MFeedbackStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

void MFeedbackStub::play()
{
    stubMethodEntered("play");
}

MFeedbackStub gDefaultMFeedbackStub;
MFeedbackStub *gMFeedbackStub = &gDefaultMFeedbackStub;

MFeedback::MFeedback(const QString &, QObject *) : d_ptr(0)
{
}

MFeedback::~MFeedback()
{
}

QString MFeedback::name() const
{
    return gMFeedbackStub->name();
}

void MFeedback::play() const
{
    gMFeedbackStub->play();
}

#endif

