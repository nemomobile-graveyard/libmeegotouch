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

#ifndef MFEEDBACKPLAYERPRIVATE_STUB
#define MFEEDBACKPLAYERPRIVATE_STUB

#include <feedback/mfeedbackplayer_p.h>
#include <stubbase.h>

class MFeedbackPlayerPrivateStub : public StubBase
{
public:
    virtual void MFeedbackPlayerPrivateConstructor(QObject *parent = 0);
    virtual void MFeedbackPlayerPrivateDestructor();
    virtual bool init(const QString &applicationName);
};

void MFeedbackPlayerPrivateStub::MFeedbackPlayerPrivateConstructor(QObject *parent)
{
    Q_UNUSED(parent);
}

void MFeedbackPlayerPrivateStub::MFeedbackPlayerPrivateDestructor()
{
}

bool MFeedbackPlayerPrivateStub::init(const QString &applicationName)
{
    Q_UNUSED(applicationName);
    stubMethodEntered("init");
    return stubReturnValue<bool>("init");
}

MFeedbackPlayerPrivateStub gDefaultMFeedbackPlayerPrivateStub;
MFeedbackPlayerPrivateStub *gMFeedbackPlayerPrivateStub = &gDefaultMFeedbackPlayerPrivateStub;


MFeedbackPlayerPrivate::MFeedbackPlayerPrivate(QObject *parent)
    : QObject(parent)
{
    gMFeedbackPlayerPrivateStub->MFeedbackPlayerPrivateConstructor(parent);
}

MFeedbackPlayerPrivate::~MFeedbackPlayerPrivate()
{
    gMFeedbackPlayerPrivateStub->MFeedbackPlayerPrivateDestructor();
}

bool MFeedbackPlayerPrivate::init(const QString &applicationName)
{
    return gMFeedbackPlayerPrivateStub->init(applicationName);
}
#endif
