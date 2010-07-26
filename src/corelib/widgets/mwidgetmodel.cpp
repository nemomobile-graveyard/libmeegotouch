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

#include "mwidgetmodel.h"
#include "mwidgetmodel_p.h"

#include <QMetaProperty>
#include <MDebug>

///////////////////
// PRIVATE CLASS //
///////////////////
MWidgetModelPrivate::MWidgetModelPrivate() :
    transactionInProgress(false),
    referenceCount(0)
{
}

MWidgetModelPrivate::~MWidgetModelPrivate()
{
}

//////////////////
// PUBLIC CLASS //
//////////////////

void MWidgetModel::memberModified(const char *const member)
{
    if (d_ptr->transactionInProgress) {
        d_ptr->modifiedMembers.enqueue(member);
    } else {
        QList<const char *> list;
        list.append(member);
        emit modified(list);
    }
}

void MWidgetModel::beginTransaction()
{
    if (d_ptr->transactionInProgress) {
        mWarning("MWidgetModel") << "beginTransaction() - transaction already started!";
        return;
    }

    d_ptr->transactionInProgress = true;
}

void MWidgetModel::commitTransaction()
{
    if (d_ptr->transactionInProgress == false) {
        mWarning("MWidgetModel") << "commitTransaction() - no transaction ongoing!";
        return;
    }

    d_ptr->transactionInProgress = false;
    emit modified(d_ptr->modifiedMembers);
    d_ptr->modifiedMembers.clear();
}

void MWidgetModel::increaseReferenceCount()
{
    ++d_ptr->referenceCount;
}

void MWidgetModel::decreaseReferenceCount()
{
    --d_ptr->referenceCount;
    if (d_ptr->referenceCount == 0)
        delete this;
}

QDataStream &operator<<(QDataStream &stream, const MWidgetModel &model)
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

QDataStream &operator>>(QDataStream &stream, MWidgetModel &model)
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

