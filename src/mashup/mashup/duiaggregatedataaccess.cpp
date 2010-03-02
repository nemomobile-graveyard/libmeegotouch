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

#include "duiaggregatedataaccess.h"
#include "duiaggregatedataaccess_p.h"

DuiAggregateDataAccessPrivate::DuiAggregateDataAccessPrivate(DuiDataAccess &primaryAccess, DuiDataAccess &secondaryAccess) :
    primaryAccess(primaryAccess), secondaryAccess(secondaryAccess), q_ptr(NULL)
{
}

DuiAggregateDataAccessPrivate::~DuiAggregateDataAccessPrivate()
{
}

DuiAggregateDataAccess::DuiAggregateDataAccess(DuiDataAccess &primaryAccess, DuiDataAccess &secondaryAccess) :
    d_ptr(new DuiAggregateDataAccessPrivate(primaryAccess, secondaryAccess))
{
    init();
}

DuiAggregateDataAccess::DuiAggregateDataAccess(DuiAggregateDataAccessPrivate &dd) :
    d_ptr(&dd)
{
    init();
}

void DuiAggregateDataAccess::init()
{
    Q_D(DuiAggregateDataAccess);
    d->q_ptr = this;
    connect(&d->primaryAccess, SIGNAL(valueChanged(QString, QVariant)), this, SIGNAL(valueChanged(QString, QVariant)));
    connect(&d->secondaryAccess, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(secondaryAccessValueChanged(QString, QVariant)));
}

DuiAggregateDataAccess::~DuiAggregateDataAccess()
{
    delete d_ptr;
}

bool DuiAggregateDataAccess::setValue(const QString &key, const QVariant &value)
{
    Q_D(DuiAggregateDataAccess);

    bool ret = false;
    ret = d->primaryAccess.setValue(key, value);
    if (!ret) {
        ret = d->secondaryAccess.setValue(key, value);
    }
    return ret;
}

QVariant DuiAggregateDataAccess::value(const QString &key) const
{
    Q_D(const DuiAggregateDataAccess);

    if (d->primaryAccess.contains(key)) {
        return d->primaryAccess.value(key);
    }
    return d->secondaryAccess.value(key);
}

QStringList DuiAggregateDataAccess::allKeys() const
{
    Q_D(const DuiAggregateDataAccess);

    QStringList ret = d->primaryAccess.allKeys() + d->secondaryAccess.allKeys();
    ret.removeDuplicates();
    return ret;
}

bool DuiAggregateDataAccess::contains(const QString &key) const
{
    Q_D(const DuiAggregateDataAccess);

    return d->primaryAccess.contains(key) || d->secondaryAccess.contains(key);
}

void DuiAggregateDataAccess::secondaryAccessValueChanged(const QString &key, const QVariant &value)
{
    Q_D(const DuiAggregateDataAccess);
    if (!d->primaryAccess.contains(key)) {
        emit valueChanged(key, value);
    }
}
