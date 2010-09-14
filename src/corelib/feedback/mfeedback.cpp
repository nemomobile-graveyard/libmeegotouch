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

#include "mfeedback.h"
#include "mfeedback_p.h"

const QString MFeedback::Press = "press";
const QString MFeedback::Release = "release";
const QString MFeedback::Cancel = "cancel";

MFeedback::MFeedback(const MFeedback &feedback)
    : QObject(0), d_ptr(new MFeedbackPrivate)
{
    Q_D(MFeedback);
    d->name = feedback.name();
}

MFeedback::MFeedback(QObject *parent)
    : QObject(parent), d_ptr(new MFeedbackPrivate)
{
}

MFeedback::MFeedback(const QString &name, QObject *parent)
    : QObject(parent), d_ptr(new MFeedbackPrivate)
{
    Q_D(MFeedback);
    d->name = name;
}

MFeedback::~MFeedback()
{
    delete d_ptr;
}

MFeedback &MFeedback::operator=(const MFeedback &other)
{
    Q_D(MFeedback);
    d->name = other.name();

    return *this;
}

void MFeedback::setName(const QString &name)
{
    Q_D(MFeedback);
    d->name = name;
}

QString MFeedback::name() const
{
    Q_D(const MFeedback);
    return d->name;
}

void MFeedback::play() const
{
    Q_D(const MFeedback);
    d->play();
}

void MFeedback::play(const QString &feedbackName)
{
    MFeedbackPrivate::play(feedbackName);
}
