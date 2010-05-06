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

#include <QString>
#include <QStringList>
#include <QVariant>

#include "mgconfitem.h"

void MGConfItem::update_value(bool emit_signal)
{
    Q_UNUSED(emit_signal);
}

QString MGConfItem::key() const
{
    return QString();
}

QVariant MGConfItem::value() const
{
    return QVariant();
}

QVariant MGConfItem::value(const QVariant &def) const
{
    Q_UNUSED(def);
    return QVariant();
}

void MGConfItem::set(const QVariant &val)
{
    Q_UNUSED(val);
}

void MGConfItem::unset()
{
}

QList<QString> MGConfItem::listDirs() const
{
    return QList<QString>();
}

QList<QString> MGConfItem::listEntries() const
{
    return QList<QString>();
}

MGConfItem::MGConfItem(const QString &key, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(key);
}

MGConfItem::~MGConfItem()
{
}
