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

#include "duifeedback.h"
#include "duifeedback_p.h"

DuiFeedback::DuiFeedback(const DuiFeedback &feedback)
    : QObject(0), d_ptr(new DuiFeedbackPrivate)
{
    Q_D(DuiFeedback);
    d->name = feedback.name();
}

DuiFeedback::DuiFeedback(QObject *parent)
    : QObject(parent), d_ptr(new DuiFeedbackPrivate)
{
}

DuiFeedback::DuiFeedback(const QString &name, QObject *parent)
    : QObject(parent), d_ptr(new DuiFeedbackPrivate)
{
    Q_D(DuiFeedback);
    d->name = name;
}

DuiFeedback::~DuiFeedback()
{
    delete d_ptr;
}

DuiFeedback &DuiFeedback::operator=(const DuiFeedback &other)
{
    Q_D(DuiFeedback);
    d->name = other.name();

    return *this;
}

void DuiFeedback::setName(const QString &name)
{
    Q_D(DuiFeedback);
    d->name = name;
}

QString DuiFeedback::name() const
{
    Q_D(const DuiFeedback);
    return d->name;
}

void DuiFeedback::play() const
{
    Q_D(const DuiFeedback);
    d->play();
}
