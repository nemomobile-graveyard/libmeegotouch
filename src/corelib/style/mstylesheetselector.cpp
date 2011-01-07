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
#include "mstylesheetparser.h"

#include <algorithm>

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

    // the attributes are supposed to come from mapped memory. we can simply reuse the pointer
    attributeList = other.attributeList;
    attributeCount = other.attributeCount;
}

MStyleSheetSelector::MStyleSheetSelector(
        MStyleSheetAttribute *attributeList,
        int attributeCount,
        const MUniqueStringCache::Index objectName,
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
    d->attributeList = attributeList;
    d->attributeCount = attributeCount;
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
    delete d_ptr;
}

MStyleSheetAttribute* MStyleSheetSelector::attributeList() const
{
    Q_D(const MStyleSheetSelector);
    return d->attributeList;
}

MStyleSheetAttribute* MStyleSheetSelector::attributeByName(MUniqueStringCache::Index name) const
{
    MStyleSheetAttribute *attributes = attributeList();
    MStyleSheetAttributeComparator comp;
    MStyleSheetAttribute *attribute = std::lower_bound(attributes, attributes + attributeCount(), name, comp);
    if (attribute == &attributes[attributeCount()]) {
        return 0;
    } else {
        return attribute;
    }
}

int MStyleSheetSelector::attributeCount() const
{
    Q_D(const MStyleSheetSelector);
    return d->attributeCount;
}

QByteArray MStyleSheetSelector::parentName() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->parentName);
}

QByteArray MStyleSheetSelector::parentObjectName() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->parentObjectName);
}

QByteArray MStyleSheetSelector::objectName() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->objectName);
}

QByteArray MStyleSheetSelector::className() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->className);
}

QByteArray MStyleSheetSelector::classType() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->classType);
}

MStyleSheetSelector::Orientation MStyleSheetSelector::orientation() const
{
    Q_D(const MStyleSheetSelector);
    return d->screenOrientation;
}

QByteArray MStyleSheetSelector::mode() const
{
    Q_D(const MStyleSheetSelector);
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(d->objectMode);
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


