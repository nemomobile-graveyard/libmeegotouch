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

#ifndef TESTOBJECTSTYLE_H
#define TESTOBJECTSTYLE_H

#include <mstyle.h>

#include <mnamespace.h>
#include <QFont>
#include <QPoint>
#include <QTextCharFormat>

#if QT_VERSION >= 0x040600
#include <QEasingCurve>
#endif

class QPixmap;
class MScalableImage;

class TestObjectStyle : public MStyle
{
    Q_OBJECT
    M_STYLE(TestObjectStyle)

    M_STYLE_ATTRIBUTE(int,        attributeInteger,       AttributeInteger)
    M_STYLE_ATTRIBUTE(qreal,      attributeReal,          AttributeReal)
    M_STYLE_ATTRIBUTE(QString,    attributeString,        AttributeString)
    M_STYLE_ATTRIBUTE(QChar,      attributeChar,          AttributeChar)
    M_STYLE_ATTRIBUTE(bool,       attributeBool,          AttributeBool)
    M_STYLE_ATTRIBUTE(QPoint,     attributePoint,         AttributePoint)
    M_STYLE_ATTRIBUTE(QPointF,    attributePointF,        AttributePointF)
    M_STYLE_ATTRIBUTE(QFont,      attributeFont,          AttributeFont)
    M_STYLE_ATTRIBUTE(QSize,      attributeSize,          AttributeSize)
    M_STYLE_ATTRIBUTE(QSizeF,     attributeSizeF,         AttributeSizeF)
    M_STYLE_ATTRIBUTE(QColor,     attributeColor,         AttributeColor)
    //M_STYLE_ATTRIBUTE(QEasingCurve, attributeEasingCurve, AttributeEasingCurve)
    M_STYLE_ATTRIBUTE(Qt::Alignment,                   attributeAlignment,        AttributeAlignment)
    M_STYLE_ATTRIBUTE(Qt::Orientation,                 attributeOrientation,      AttributeOrientation)
    M_STYLE_ATTRIBUTE(QTextCharFormat::UnderlineStyle, attributeUnderlineStyle,   AttributeUnderlineStyle)
    M_STYLE_ATTRIBUTE(Qt::PenStyle,                    attributePenStyle,         AttributePenStyle)
    M_STYLE_ATTRIBUTE(Qt::Axis,                        attributeAxis,             AttributeAxis)
    M_STYLE_ATTRIBUTE(QTextOption::WrapMode,         attributeWrapMode,         AttributeWrapMode)

    M_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap, AttributePixmap)
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap2, AttributePixmap2)
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap3, AttributePixmap3)
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, attributePixmap4, AttributePixmap4)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, attributeScalable, AttributeScalable)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, attributeScalable2, AttributeScalable2)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, attributeScalable3, AttributeScalable3)
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, attributeScalable4, AttributeScalable4)
};

class TestObjectStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER(TestObjectStyle)
};

#endif

