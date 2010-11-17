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
    MStyleSheetAttribute(const QByteArray& name, const QByteArray& value, qint64 position);
    MStyleSheetAttribute(const MStyleSheetAttribute &other);

    QByteArray getName();
    QByteArray getValue();
    qint64 getPosition();

    static int attributeToInt(const QByteArray &attribute, bool *conversionOk);
    static int attributeToInt(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation);
    static qreal attributeToFloat(const QByteArray &attribute, bool *conversionOk);
    static qreal attributeToFloat(const QByteArray &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation);

    static bool booleanFromString(const QByteArray &string, bool *conversionOk);
    static QColor colorFromString(const QByteArray &string, bool *conversionOk);
    static QFont fontFromString(const QByteArray string, bool *conversionOk);
    static QByteArray attributeNameToPropertyName(const QByteArray &attributeName);

    bool writeAttribute(const QString &filename,
                        MStyle *style,
                        const QMetaProperty &property,
                        M::Orientation orientation);

private:
    // Fills a property with a variant and caches the variant if possible.
    // Pointer types cannot be cached as the ownership is transfered to the property.
    // Caching must be disabled for them.
    bool fillProperty(const QMetaProperty &property, MStyle *style, M::Orientation cacheOrientation,
        const QVariant &variant, bool cache = true);

    QByteArray name;
    QByteArray value;
    qint64 position;    //used for providing detailed css parse error output (linenumber)

    friend class MStyleSheetParserPrivate;
};

typedef QMap<QByteArray, MStyleSheetAttribute *> MAttributeList;
//! \internal_end

#endif
