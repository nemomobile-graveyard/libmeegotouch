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

#include "duiwidgetmodel.h"
#include "duiwidgetmodel_p.h"

#include <QMetaProperty>
#include <DuiDebug>

///////////////////
// PRIVATE CLASS //
///////////////////
DuiWidgetModelPrivate::DuiWidgetModelPrivate() :
    transactionInProgress(false),
    referenceCount(0)
{
}

DuiWidgetModelPrivate::~DuiWidgetModelPrivate()
{
}

//////////////////
// PUBLIC CLASS //
//////////////////

void DuiWidgetModel::memberModified(const char *const member)
{
    if (d_ptr->transactionInProgress) {
        d_ptr->modifiedMembers.enqueue(member);
    } else {
        QList<const char *> list;
        list.append(member);
        emit modified(list);
    }
}

void DuiWidgetModel::beginTransaction()
{
    if (d_ptr->transactionInProgress) {
        duiWarning("DuiWidgetModel") << "beginTransaction() - transaction already started!";
        return;
    }

    d_ptr->transactionInProgress = true;
}

void DuiWidgetModel::commitTransaction()
{
    if (d_ptr->transactionInProgress == false) {
        duiWarning("DuiWidgetModel") << "commitTransaction() - no transaction ongoing!";
        return;
    }

    d_ptr->transactionInProgress = false;
    emit modified(d_ptr->modifiedMembers);
    d_ptr->modifiedMembers.clear();
}

void DuiWidgetModel::increaseReferenceCount()
{
    ++d_ptr->referenceCount;
}

void DuiWidgetModel::decreaseReferenceCount()
{
    --d_ptr->referenceCount;
    if (d_ptr->referenceCount == 0)
        deleteLater();
}

QDataStream &operator<<(QDataStream &stream, const DuiWidgetModel &model)
{
    const QMetaObject *metaObject = model.metaObject();
    QString hashString = metaObject->className();

    int propertyCount = metaObject->propertyCount();
    for (int i = 0; i != propertyCount; ++i) {
        hashString += metaObject->property(i).name();
        hashString += metaObject->property(i).typeName();
    }

    uint hash = qHash(hashString);
    stream << hash;
    for (int i = 0; i != propertyCount; ++i) {
        stream << metaObject->property(i).read(&model);
    }

    return stream;
}

QDataStream &operator>>(QDataStream &stream, DuiWidgetModel &model)
{
    const QMetaObject *metaObject = model.metaObject();
    QString hashString = metaObject->className();

    int propertyCount = metaObject->propertyCount();
    for (int i = 0; i != propertyCount; ++i) {
        hashString += metaObject->property(i).name();
        hashString += metaObject->property(i).typeName();
    }

    uint realHash = qHash(hashString);
    uint hash;
    stream >> hash;
    if (hash != realHash) {
        qFatal("Deserializing of %s failed from QDataStream, Q_PROPERTY table doesn't match!", metaObject->className());
    }

    for (int i = 0; i != propertyCount; ++i) {
        QVariant v;
        stream >> v;
        metaObject->property(i).write(&model, v);
    }

    return stream;
}

