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

#include "mstylesheetselector.h"
#include "mstylesheetselector_p.h"
#include "mstylesheetattribute.h"

void MStyleSheetSelectorPrivate::operator=(const MStyleSheetSelectorPrivate &other)
{
    objName = other.objName;
    clName = other.clName;
    clType = other.clType;
    screenOrientation = other.screenOrientation;
    objectMode = other.objectMode;
    filename = other.filename;
    parentName = other.parentName;
    flags = other.flags;

    // copy attributes
    MAttributeList::const_iterator otherDataEnd = other.data.constEnd();

    for (MAttributeList::const_iterator iterator = other.data.begin(); iterator != otherDataEnd; ++iterator) {
        // attribute to copy
        MStyleSheetAttribute *source = *iterator;

        // copy of attribute
        MStyleSheetAttribute *copy = new MStyleSheetAttribute();
        copy->name = source->name;
        copy->value = source->value;
        copy->constValue = source->constValue;
        copy->position = source->position;

        // add copy of attribute to data list
        data.insert(iterator.key(), copy);
    }
}


MStyleSheetSelector::MStyleSheetSelector(const QString &objectName,
        const QString &className,
        const QString &classType,
        const QString &orientation,
        const QString &mode,
        const QString &filename,
        const QString &parentName,
        Flags flags) :
    d_ptr(new MStyleSheetSelectorPrivate)
{
    Q_D(MStyleSheetSelector);
    d->objName = objectName;
    d->clName = className;
    d->clType = classType;
    d->screenOrientation = orientation;
    d->objectMode = mode;
    d->filename = filename;
    d->parentName = parentName;
    d->flags = flags;
}




MStyleSheetSelector::MStyleSheetSelector(const MStyleSheetSelector &other) :
    d_ptr(new MStyleSheetSelectorPrivate)
{
    *d_ptr = *other.d_ptr;
}

MStyleSheetSelector::~MStyleSheetSelector()
{
    Q_D(MStyleSheetSelector);

    qDeleteAll(d->data);
    d->data.clear();
    delete d_ptr;
}


MAttributeList *MStyleSheetSelector::attributes()
{
    Q_D(MStyleSheetSelector);
    return &d->data;
}

QString MStyleSheetSelector::parentName() const
{
    Q_D(const MStyleSheetSelector);
    return d->parentName;
}

QString MStyleSheetSelector::objectName() const
{
    Q_D(const MStyleSheetSelector);
    return d->objName;
}

QString MStyleSheetSelector::className() const
{
    Q_D(const MStyleSheetSelector);
    return d->clName;
}

QString MStyleSheetSelector::classType() const
{
    Q_D(const MStyleSheetSelector);
    return d->clType;
}

QString MStyleSheetSelector::orientation() const
{
    Q_D(const MStyleSheetSelector);
    return d->screenOrientation;
}

QString MStyleSheetSelector::mode() const
{
    Q_D(const MStyleSheetSelector);
    return d->objectMode;
}

MStyleSheetSelector::Flags MStyleSheetSelector::flags() const
{
    Q_D(const MStyleSheetSelector);
    return d->flags;
}


