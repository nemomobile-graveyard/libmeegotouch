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

#include "duistylesheetselector.h"
#include "duistylesheetselector_p.h"
#include "duistylesheetattribute.h"

void DuiStyleSheetSelectorPrivate::operator=(const DuiStyleSheetSelectorPrivate &other)
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
    DuiAttributeList::const_iterator otherDataEnd = other.data.constEnd();

    for (DuiAttributeList::const_iterator iterator = other.data.begin(); iterator != otherDataEnd; ++iterator) {
        // attribute to copy
        DuiStyleSheetAttribute *source = *iterator;

        // copy of attribute
        DuiStyleSheetAttribute *copy = new DuiStyleSheetAttribute();
        copy->name = source->name;
        copy->value = source->value;
        copy->constValue = source->constValue;
        copy->position = source->position;

        // add copy of attribute to data list
        data.insert(iterator.key(), copy);
    }
}


DuiStyleSheetSelector::DuiStyleSheetSelector(const QString &objectName,
        const QString &className,
        const QString &classType,
        const QString &orientation,
        const QString &mode,
        const QString &filename,
        const QString &parentName,
        Flags flags) :
    d_ptr(new DuiStyleSheetSelectorPrivate)
{
    Q_D(DuiStyleSheetSelector);
    d->objName = objectName;
    d->clName = className;
    d->clType = classType;
    d->screenOrientation = orientation;
    d->objectMode = mode;
    d->filename = filename;
    d->parentName = parentName;
    d->flags = flags;
}




DuiStyleSheetSelector::DuiStyleSheetSelector(const DuiStyleSheetSelector &other) :
    d_ptr(new DuiStyleSheetSelectorPrivate)
{
    *d_ptr = *other.d_ptr;
}

DuiStyleSheetSelector::~DuiStyleSheetSelector()
{
    Q_D(DuiStyleSheetSelector);

    qDeleteAll(d->data);
    d->data.clear();
    delete d_ptr;
}


DuiAttributeList *DuiStyleSheetSelector::attributes()
{
    Q_D(DuiStyleSheetSelector);
    return &d->data;
}

QString DuiStyleSheetSelector::parentName() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->parentName;
}

QString DuiStyleSheetSelector::objectName() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->objName;
}

QString DuiStyleSheetSelector::className() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->clName;
}

QString DuiStyleSheetSelector::classType() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->clType;
}

QString DuiStyleSheetSelector::orientation() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->screenOrientation;
}

QString DuiStyleSheetSelector::mode() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->objectMode;
}

DuiStyleSheetSelector::Flags DuiStyleSheetSelector::flags() const
{
    Q_D(const DuiStyleSheetSelector);
    return d->flags;
}


