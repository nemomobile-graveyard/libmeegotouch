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

#ifndef DUIFEEDBACKPLAYER_STUB
#define DUIFEEDBACKPLAYER_STUB

#include <duifeedbackplayer.h>
#include <stubbase.h>
#include "duifeedbackplayerprivate_stub.h"

class DuiFeedbackPlayerStub : public StubBase
{
public:
    virtual void DuiFeedbackPlayerConstructor(QObject *parent = 0);
    virtual void DuiFeedbackPlayerDestructor();
};

void DuiFeedbackPlayerStub::DuiFeedbackPlayerConstructor(QObject *parent)
{
    Q_UNUSED(parent);
}

void DuiFeedbackPlayerStub::DuiFeedbackPlayerDestructor()
{
}

DuiFeedbackPlayerStub gDefaultDuiFeedbackPlayerStub;
DuiFeedbackPlayerStub *gDuiFeedbackPlayerStub = &gDefaultDuiFeedbackPlayerStub;


DuiFeedbackPlayer::DuiFeedbackPlayer(QObject *parent)
    : QObject(parent), d_ptr(0)
{
    gDuiFeedbackPlayerStub->DuiFeedbackPlayerConstructor(parent);
}

DuiFeedbackPlayer::~DuiFeedbackPlayer()
{
    gDuiFeedbackPlayerStub->DuiFeedbackPlayerDestructor();
}

void DuiFeedbackPlayer::play(const QString &feedbackName)
{
    Q_UNUSED(feedbackName);
}

#endif
