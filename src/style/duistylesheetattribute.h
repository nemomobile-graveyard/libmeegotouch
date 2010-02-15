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

#ifndef DUISTYLESHEETATTRIBUTE_H
#define DUISTYLESHEETATTRIBUTE_H

#include <QMetaObject>
#include <QString>
#include <QMap>
#include <QTextCharFormat>
#include <QEasingCurve>

#include "duinamespace.h"
#include "duistyle.h"
#include "duishareddata.h"


//! \internal
/*! An attribute of a style sheet selector.
 *  This class includes helper functions such as attributeToInt.
 */
class DuiStyleSheetAttribute
{

// internal enum that defines the type of SIZE or SIZE_F
// attribute in attributeToInt and attributeToFloat conversion functions.
    enum SizeAttributeType {
        WidthAttribute,
        HeightAttribute
    };

public:
    QString name;
    QString value;
    QString constValue; //original parsed value containing the $CONSTANT definitions
    qint64 position;    //used for providing detailed css parse error output (linenumber)

    static int mmToInt(qreal mm);
    static qreal mmToReal(qreal mm);
    static int attributeToInt(const QString &attribute, bool *conversionOk);
    static int attributeToInt(const QString &attribute, bool *conversionOk, SizeAttributeType type, Dui::Orientation orientation);
    static qreal attributeToFloat(const QString &attribute, bool *conversionOk);
    static qreal attributeToFloat(const QString &attribute, bool *conversionOk, SizeAttributeType type, Dui::Orientation orientation);

    static bool booleanFromString(const QString &string, bool *conversionOk);
    static QColor colorFromString(const QString &string, bool *conversionOk);
    static QFont fontFromString(const QString string, bool *conversionOk);

    bool writeAttribute(const QString &filename,
                        DuiStyle *style,
                        const QMetaProperty &property,
                        Dui::Orientation orientation);

    void writeAttribute(const QMetaProperty &property,
                        DuiSharedData &shm,
                        QString filename);

    bool writeAttributeShm(DuiStyle *style,
                           QMetaProperty property,
                           DuiSharedData &ds,
                           Dui::Orientation orientation);
};

typedef QMap<QString, DuiStyleSheetAttribute *> DuiAttributeList;
//! \internal_end

#endif
