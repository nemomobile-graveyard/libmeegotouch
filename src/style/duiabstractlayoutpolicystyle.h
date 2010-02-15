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

#ifndef DUIABSTRACTLAYOUTPOLICYSTYLE_H
#define DUIABSTRACTLAYOUTPOLICYSTYLE_H

#include <duistyle.h>

/** \brief Defines a style for a DuiAbstractLayoutPolicy class and the policies that inherit from this
 *  This defines the attributes that are common to all DuiLayout policies
 */
class DUI_EXPORT DuiAbstractLayoutPolicyStyle : public DuiStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiAbstractLayoutPolicyStyle)

    ///Sets the horizontal distances between items, where applicable.
    DUI_STYLE_ATTRIBUTE(qreal, horizontalSpacing, HorizontalSpacing)
    ///Sets the vertical distances between items, where applicable.
    DUI_STYLE_ATTRIBUTE(qreal, verticalSpacing, VerticalSpacing)
    ///FIXME Convert these to int or, even better, QMargin
    ///The left contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the DuiLayout margin
    DUI_STYLE_ATTRIBUTE(qreal, marginLeft, MarginLeft)
    ///The top contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the DuiLayout margin
    DUI_STYLE_ATTRIBUTE(qreal, marginTop, MarginTop)
    ///The right contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the DuiLayout margin
    DUI_STYLE_ATTRIBUTE(qreal, marginRight, MarginRight)
    ///The bottom contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the DuiLayout margin
    DUI_STYLE_ATTRIBUTE(qreal, marginBottom, MarginBottom)

};

class DUI_EXPORT DuiAbstractLayoutPolicyStyleContainer : public DuiStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiAbstractLayoutPolicyStyle)
};

#endif

