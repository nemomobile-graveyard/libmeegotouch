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
#include "mstylesheetattribute.h"
#include "mstylesheetparser.h"

#include <algorithm>

namespace {
void fixAlignement(char** buffer, int size)
{
    size_t remainder = (size_t)*buffer % size;
    if (remainder != 0) {
        *buffer += size - remainder;
    }
}
}

MStyleSheetSelector::MStyleSheetSelector(
        const MStyleSheetAttribute *attributeList,
        const int attributeCount,
        const MUniqueStringCache::Index objectName,
        const MUniqueStringCache::Index className,
        const MUniqueStringCache::Index classType,
        const Orientation orientation,
        const MUniqueStringCache::Index mode,
        const MUniqueStringCache::Index parentName,
        const MUniqueStringCache::Index parentObjectName,
        const Flags flags,
        const MUniqueStringCache::Index filename) :
        fromMappedMemory(false),
        _attributeList(attributeList),
        _attributeCount(attributeCount),
        _objectName(objectName),
        _className(className),
        _classType(classType),
        _orientation(orientation),
        _mode(mode),
        _parentName(parentName),
        _parentObjectName(parentObjectName),
        _flags(flags),
        _filename(filename)
{
}

MStyleSheetSelector::~MStyleSheetSelector()
{
    if (!fromMappedMemory) {
        delete[] _attributeList;
    }
}

const MStyleSheetAttribute* MStyleSheetSelector::attributeList() const
{
    if (!fromMappedMemory) {
        return _attributeList;
    } else {
        // if this MStyleSheetSelector comes from mappes memory we know the relative
        // position if its MStyleSheetAttributes in memory
        char * mappedAttributeList = (char*)this + sizeof(MStyleSheetSelector);
        fixAlignement(&mappedAttributeList, 8);
        return reinterpret_cast<MStyleSheetAttribute*>(mappedAttributeList);
    }
}

const MStyleSheetAttribute* MStyleSheetSelector::attributeByName(MUniqueStringCache::Index name) const
{
    const MStyleSheetAttribute *attributes = attributeList();
    MStyleSheetAttributeComparator comp;
    const MStyleSheetAttribute *attribute = std::lower_bound(attributes, attributes + attributeCount(), name, comp);
    if (attribute == &attributes[attributeCount()] || attribute->getNameID() != name) {
        return 0;
    } else {
        return attribute;
    }
}

int MStyleSheetSelector::attributeCount() const
{
    return _attributeCount;
}

QLatin1String MStyleSheetSelector::parentName() const
{
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(_parentName);
}

QLatin1String MStyleSheetSelector::parentObjectName() const
{
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(_parentObjectName);
}

QLatin1String MStyleSheetSelector::objectName() const
{
    return MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(_objectName);
}

QLatin1String MStyleSheetSelector::className() const
{
    return MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(_className);
}

QLatin1String MStyleSheetSelector::classType() const
{
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(_classType);
}

MStyleSheetSelector::Orientation MStyleSheetSelector::orientation() const
{
    return _orientation;
}

QLatin1String MStyleSheetSelector::mode() const
{
    return MStyleSheetParser::stringCacheWithoutReverseLookup()->indexToString(_mode);
}

MStyleSheetSelector::Flags MStyleSheetSelector::flags() const
{
    return _flags;
}

QLatin1String MStyleSheetSelector::filename() const
{
    return MStyleSheetParser::stringCacheWithReverseLookup()->indexToString(_filename);
}

MUniqueStringCache::Index MStyleSheetSelector::filenameID() const
{
    return _filename;
}

MUniqueStringCache::Index MStyleSheetSelector::objectNameID() const
{
    return _objectName;
}

MUniqueStringCache::Index MStyleSheetSelector::classNameID() const
{
    return _className;
}

MUniqueStringCache::Index MStyleSheetSelector::classTypeID() const
{
    return _classType;
}

MUniqueStringCache::Index MStyleSheetSelector::modeID() const
{
    return _mode;
}

MUniqueStringCache::Index MStyleSheetSelector::parentNameID() const
{
    return _parentName;
}

MUniqueStringCache::Index MStyleSheetSelector::parentObjectNameID() const
{
    return _parentObjectName;
}


