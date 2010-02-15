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

#ifndef TESTOBJECTSTYLE_H
#define TESTOBJECTSTYLE_H

#include <duistyle.h>

#include <QFont>
#include <QPoint>
#include <QTextCharFormat>

#if QT_VERSION >= 0x040600
#include <QEasingCurve>
#endif

class QPixmap;
class DuiScalableImage;

class TestObjectStyle : public DuiStyle
{
    Q_OBJECT
    DUI_STYLE(TestObjectStyle)

    DUI_STYLE_ATTRIBUTE(int,        attributeInteger,       AttributeInteger)
    DUI_STYLE_ATTRIBUTE(qreal,      attributeReal,          AttributeReal)
    DUI_STYLE_ATTRIBUTE(QString,    attributeString,        AttributeString)
    DUI_STYLE_ATTRIBUTE(QChar,      attributeChar,          AttributeChar)
    DUI_STYLE_ATTRIBUTE(bool,       attributeBool,          AttributeBool)
    DUI_STYLE_ATTRIBUTE(QPoint,     attributePoint,         AttributePoint)
    DUI_STYLE_ATTRIBUTE(QPointF,    attributePointF,        AttributePointF)
    DUI_STYLE_ATTRIBUTE(QFont,      attributeFont,          AttributeFont)
    DUI_STYLE_ATTRIBUTE(QSize,      attributeSize,          AttributeSize)
    DUI_STYLE_ATTRIBUTE(QSizeF,     attributeSizeF,         AttributeSizeF)
    DUI_STYLE_ATTRIBUTE(QColor,     attributeColor,         AttributeColor)
    //DUI_STYLE_ATTRIBUTE(QEasingCurve, attributeEasingCurve, AttributeEasingCurve)
    DUI_STYLE_ATTRIBUTE(Qt::Alignment,                   attributeAlignment,        AttributeAlignment)
    DUI_STYLE_ATTRIBUTE(Qt::Orientation,                 attributeOrientation,      AttributeOrientation)
    DUI_STYLE_ATTRIBUTE(QTextCharFormat::UnderlineStyle, attributeUnderlineStyle,   AttributeUnderlineStyle)
    DUI_STYLE_ATTRIBUTE(Qt::PenStyle,                    attributePenStyle,         AttributePenStyle)
    DUI_STYLE_ATTRIBUTE(Qt::Axis,                        attributeAxis,             AttributeAxis)

    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap, AttributePixmap)
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap2, AttributePixmap2)
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap3, AttributePixmap3)
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap4, AttributePixmap4)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, attributeScalable, AttributeScalable)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, attributeScalable2, AttributeScalable2)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, attributeScalable3, AttributeScalable3)
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, attributeScalable4, AttributeScalable4)
};

class TestObjectStyleContainer : public DuiStyleContainer
{
    DUI_STYLE_CONTAINER(TestObjectStyle)
};

#endif

