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

#include "maggregatedataaccess.h"
#include "maggregatedataaccess_p.h"

MAggregateDataAccessPrivate::MAggregateDataAccessPrivate(MDataAccess &primaryAccess, MDataAccess &secondaryAccess) :
    primaryAccess(primaryAccess), secondaryAccess(secondaryAccess), q_ptr(NULL)
{
}

MAggregateDataAccessPrivate::~MAggregateDataAccessPrivate()
{
}

MAggregateDataAccess::MAggregateDataAccess(MDataAccess &primaryAccess, MDataAccess &secondaryAccess) :
    d_ptr(new MAggregateDataAccessPrivate(primaryAccess, secondaryAccess))
{
    init();
}

MAggregateDataAccess::MAggregateDataAccess(MAggregateDataAccessPrivate &dd) :
    d_ptr(&dd)
{
    init();
}

void MAggregateDataAccess::init()
{
    Q_D(MAggregateDataAccess);
    d->q_ptr = this;
    connect(&d->primaryAccess, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(valueChanged(QString, QVariant)));
    connect(&d->secondaryAccess, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(secondaryAccessValueChanged(QString, QVariant)));
}

MAggregateDataAccess::~MAggregateDataAccess()
{
    delete d_ptr;
}

bool MAggregateDataAccess::setValue(const QString &key, const QVariant &value)
{
    Q_D(MAggregateDataAccess);

    bool ret = false;
    ret = d->primaryAccess.setValue(key, value);
    if (!ret) {
        ret = d->secondaryAccess.setValue(key, value);
    }
    return ret;
}

QVariant MAggregateDataAccess::value(const QString &key) const
{
    Q_D(const MAggregateDataAccess);

    if (d->primaryAccess.contains(key)) {
        return d->primaryAccess.value(key);
    }
    return d->secondaryAccess.value(key);
}

QStringList MAggregateDataAccess::allKeys() const
{
    Q_D(const MAggregateDataAccess);

    QStringList ret = d->primaryAccess.allKeys() + d->secondaryAccess.allKeys();
    ret.removeDuplicates();
    return ret;
}

bool MAggregateDataAccess::contains(const QString &key) const
{
    Q_D(const MAggregateDataAccess);

    return d->primaryAccess.contains(key) || d->secondaryAccess.contains(key);
}

void MAggregateDataAccess::secondaryAccessValueChanged(const QString &key, const QVariant &value)
{
    Q_D(const MAggregateDataAccess);
    if (!d->primaryAccess.contains(key)) {
        emit valueChanged(key, value);
    }
}
