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

#include <QString>
#include <QStringList>
#include <QVariant>

#include "duigconfitem.h"

void DuiGConfItem::update_value(bool emit_signal)
{
    Q_UNUSED(emit_signal);
}

QString DuiGConfItem::key() const
{
    return QString();
}

QVariant DuiGConfItem::value() const
{
    return QVariant();
}

QVariant DuiGConfItem::value(const QVariant &def) const
{
    Q_UNUSED(def);
    return QVariant();
}

void DuiGConfItem::set(const QVariant &val)
{
    Q_UNUSED(val);
}

void DuiGConfItem::unset()
{
}

QList<QString> DuiGConfItem::listDirs() const
{
    return QList<QString>();
}

QList<QString> DuiGConfItem::listEntries() const
{
    return QList<QString>();
}

DuiGConfItem::DuiGConfItem(const QString &key, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(key);
}

DuiGConfItem::~DuiGConfItem()
{
}
