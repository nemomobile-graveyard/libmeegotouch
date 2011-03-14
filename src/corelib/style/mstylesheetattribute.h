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

#ifndef MSTYLESHEETATTRIBUTE_H
#define MSTYLESHEETATTRIBUTE_H

#include <QMetaObject>
#include <QString>
#include <QMap>
#include <QTextCharFormat>
#include <QEasingCurve>

#include "mnamespace.h"
#include "mstyle.h"
#include "mshareddata.h"
#include "muniquestringcache.h"

//! \internal
/*! An attribute of a style sheet selector.
 *  This class includes helper functions such as attributeToInt.
 */
class MStyleSheetAttribute
{

// internal enum that defines the type of SIZE or SIZE_F
// attribute in attributeToInt and attributeToFloat conversion functions.
    enum SizeAttributeType {
        WidthAttribute,
        HeightAttribute
    };

public:
    MStyleSheetAttribute(MUniqueStringCache::Index name, MUniqueStringCache::Index value, qint64 position);

    QLatin1String getName() const;
    MUniqueStringCache::Index getNameID() const;
    QLatin1String getValue() const;
    MUniqueStringCache::Index getValueID() const;
    qint64 getPosition() const;

    enum CacheOrientationFlags {
        PortraitFlag = 1,
        LandscapeFlag = 2
    };

    static int maxValueForDevice(SizeAttributeType type, M::Orientation orientation);
    static int attributeToInt(const QByteArray &attribute, bool *conversionOk);
    static int attributeToInt(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation, CacheOrientationFlags *cacheOrientation);
    static qreal attributeToFloat(const QByteArray &attribute, bool *conversionOk);
    static qreal attributeToFloat(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation, CacheOrientationFlags *cacheOrientation);

    static bool booleanFromString(const QByteArray &string, bool *conversionOk);
    static QColor colorFromString(const QByteArray &string, bool *conversionOk);
    static QFont fontFromString(const QByteArray string, bool *conversionOk);
    static QByteArray attributeNameToPropertyName(const QByteArray &attributeName);

    bool writeAttribute(MUniqueStringCache::Index filename,
                        MStyle *style,
                        const QMetaProperty &property,
                        M::Orientation orientation) const;

private:
    // needed by MStyleSheetParserPrivate
    MStyleSheetAttribute() {}

    // Fills a property with a variant and caches the variant if possible.
    // Pointer types cannot be cached as the ownership is transfered to the property.
    // Caching must be disabled for them.
    bool fillProperty(const QMetaProperty &property, MStyle *style, CacheOrientationFlags cacheOrientation,
        const QVariant &variant, bool cache = true) const;

    MUniqueStringCache::Index name;
    MUniqueStringCache::Index value;
    qint64 position;    //used for providing detailed css parse error output (linenumber)

    friend class MStyleSheetParserPrivate;
};

struct MStyleSheetAttributeComparator
{
    bool operator()(const MStyleSheetAttribute& a1, MUniqueStringCache::Index name)
    {
        return a1.getNameID() < name;
    }
};

//! \internal_end

#endif
