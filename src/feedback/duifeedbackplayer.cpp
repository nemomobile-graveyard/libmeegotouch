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

#include "duifeedbackplayer.h"
#include "duifeedbackplayer_p.h"

const QString DuiFeedbackPlayer::Press = "press";
const QString DuiFeedbackPlayer::Release = "release";
const QString DuiFeedbackPlayer::Cancel = "cancel";
const QString DuiFeedbackPlayer::Error = "error";

DuiFeedbackPlayer::DuiFeedbackPlayer(QObject *parent)
    : QObject(parent), d_ptr(new DuiFeedbackPlayerPrivate(this))
{
}

DuiFeedbackPlayer::~DuiFeedbackPlayer()
{
    delete d_ptr;
}

void DuiFeedbackPlayer::play(const QString &feedbackName)
{
    Q_D(DuiFeedbackPlayer);

    d->sendPlaybackRequest(feedbackName);
}
