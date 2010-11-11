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
    objectName = other.objectName;
    className = other.className;
    classType = other.classType;
    screenOrientation = other.screenOrientation;
    objectMode = other.objectMode;
    parentName = other.parentName;
    parentObjectName = other.parentObjectName;
    flags = other.flags;

    // copy attributes
    MAttributeList::const_iterator otherAttributesEnd = other.attributes.constEnd();

    for (MAttributeList::const_iterator iterator = other.attributes.begin(); iterator != otherAttributesEnd; ++iterator) {
        // attribute to copy
        MStyleSheetAttribute *source = *iterator;

        // copy of attribute
        MStyleSheetAttribute *copy = new MStyleSheetAttribute(*source);

        // add copy of attribute to data list
        attributes.insert(iterator.key(), copy);
    }
}


MStyleSheetSelector::MStyleSheetSelector(const MUniqueStringCache::Index objectName,
        const MUniqueStringCache::Index className,
        const MUniqueStringCache::Index classType,
        const Orientation orientation,
        const MUniqueStringCache::Index mode,
        const MUniqueStringCache::Index parentName,
        Flags flags) :
    d_ptr(new MStyleSheetSelectorPrivate)
{
    Q_D(MStyleSheetSelector);
    d->objectName = objectName;
    d->className = className;
    d->classType = classType;
    d->screenOrientation = orientation;
    d->objectMode = mode;
    d->parentName = parentName;
    d->parentObjectName = MUniqueStringCache::UndefinedIndex;
    d->flags = flags;
}

MStyleSheetSelector::MStyleSheetSelector(const MUniqueStringCache::Index objectName,
        const MUniqueStringCache::Index className,
        const MUniqueStringCache::Index classType,
        const Orientation orientation,
        const MUniqueStringCache::Index mode,
        const MUniqueStringCache::Index parentName,
        const MUniqueStringCache::Index parentObjectName,
        Flags flags) :
    d_ptr(new MStyleSheetSelectorPrivate)
{
    Q_D(MStyleSheetSelector);
    d->objectName = objectName;
    d->className = className;
    d->classType = classType;
    d->screenOrientation = orientation;
    d->objectMode = mode;
    d->parentName = parentName;
    d->parentObjectName = parentObjectName;
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

    qDeleteAll(d->attributes);
    d->attributes.clear();
    delete d_ptr;
}


MAttributeList *MStyleSheetSelector::attributes()
{
    Q_D(MStyleSheetSelector);
    return &d->attributes;
}

QByteArray MStyleSheetSelector::parentName() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->parentName);
}

QByteArray MStyleSheetSelector::parentObjectName() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->parentObjectName);
}

QByteArray MStyleSheetSelector::objectName() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->objectName);
}

QByteArray MStyleSheetSelector::className() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->className);
}

QByteArray MStyleSheetSelector::classType() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->classType);
}

MStyleSheetSelector::Orientation MStyleSheetSelector::orientation() const
{
    Q_D(const MStyleSheetSelector);
    return d->screenOrientation;
}

QByteArray MStyleSheetSelector::mode() const
{
    Q_D(const MStyleSheetSelector);
    return MUniqueStringCache::indexToString(d->objectMode);
}

MStyleSheetSelector::Flags MStyleSheetSelector::flags() const
{
    Q_D(const MStyleSheetSelector);
    return d->flags;
}

MUniqueStringCache::Index MStyleSheetSelector::objectNameID() const
{
    Q_D(const MStyleSheetSelector);
    return d->objectName;
}

MUniqueStringCache::Index MStyleSheetSelector::classNameID() const
{
    Q_D(const MStyleSheetSelector);
    return d->className;
}

MUniqueStringCache::Index MStyleSheetSelector::classTypeID() const
{
    Q_D(const MStyleSheetSelector);
    return d->classType;
}

MUniqueStringCache::Index MStyleSheetSelector::modeID() const
{
    Q_D(const MStyleSheetSelector);
    return d->objectMode;
}

MUniqueStringCache::Index MStyleSheetSelector::parentNameID() const
{
    Q_D(const MStyleSheetSelector);
    return d->parentName;
}

MUniqueStringCache::Index MStyleSheetSelector::parentObjectNameID() const
{
    Q_D(const MStyleSheetSelector);
    return d->parentObjectName;
}


