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

#include "mfeedbackplayer.h"
#include "mfeedbackplayer_p.h"

const QString MFeedbackPlayer::Press = "press";
const QString MFeedbackPlayer::Release = "release";
const QString MFeedbackPlayer::Cancel = "cancel";
const QString MFeedbackPlayer::Error = "error";

MFeedbackPlayer::MFeedbackPlayer(QObject *parent)
    : QObject(parent), d_ptr(new MFeedbackPlayerPrivate(this))
{
}

MFeedbackPlayer::~MFeedbackPlayer()
{
    delete d_ptr;
}

void MFeedbackPlayer::play(const QString &feedbackName)
{
    Q_D(MFeedbackPlayer);

    d->sendPlaybackRequest(feedbackName);
}
