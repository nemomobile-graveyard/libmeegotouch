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

#ifndef MFEEDBACKPLAYER_STUB
#define MFEEDBACKPLAYER_STUB

#include <mfeedbackplayer.h>
#include <stubbase.h>
#include "mfeedbackplayerprivate_stub.h"

class MFeedbackPlayerStub : public StubBase
{
public:
    virtual void MFeedbackPlayerConstructor(QObject *parent = 0);
    virtual void MFeedbackPlayerDestructor();
};

void MFeedbackPlayerStub::MFeedbackPlayerConstructor(QObject *parent)
{
    Q_UNUSED(parent);
}

void MFeedbackPlayerStub::MFeedbackPlayerDestructor()
{
}

MFeedbackPlayerStub gDefaultMFeedbackPlayerStub;
MFeedbackPlayerStub *gMFeedbackPlayerStub = &gDefaultMFeedbackPlayerStub;


MFeedbackPlayer::MFeedbackPlayer(QObject *parent)
    : QObject(parent), d_ptr(0)
{
    gMFeedbackPlayerStub->MFeedbackPlayerConstructor(parent);
}

MFeedbackPlayer::~MFeedbackPlayer()
{
    gMFeedbackPlayerStub->MFeedbackPlayerDestructor();
}

void MFeedbackPlayer::play(const QString &feedbackName)
{
    Q_UNUSED(feedbackName);
}

#endif
