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

#ifndef MABSTRACTLAYOUTPOLICYSTYLE_H
#define MABSTRACTLAYOUTPOLICYSTYLE_H

#include <mstyle.h>

/** \brief Defines a style for a MAbstractLayoutPolicy class and the policies that inherit from this
 *  This defines the attributes that are common to all MLayout policies
 */
class M_CORE_EXPORT MAbstractLayoutPolicyStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MAbstractLayoutPolicyStyle)

    ///Sets the horizontal distances between items, where applicable.
    M_STYLE_ATTRIBUTE(qreal, horizontalSpacing, HorizontalSpacing)
    ///Sets the vertical distances between items, where applicable.
    M_STYLE_ATTRIBUTE(qreal, verticalSpacing, VerticalSpacing)
    ///FIXME Convert these to int or, even better, QMargin
    ///The left contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the MLayout margin
    M_STYLE_ATTRIBUTE(qreal, marginLeft, MarginLeft)
    ///The top contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the MLayout margin
    M_STYLE_ATTRIBUTE(qreal, marginTop, MarginTop)
    ///The right contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the MLayout margin
    M_STYLE_ATTRIBUTE(qreal, marginRight, MarginRight)
    ///The bottom contents margin of the layout for this policy.  If this is not set explicitly (in code or css) it returns -1, indicating to use the MLayout margin
    M_STYLE_ATTRIBUTE(qreal, marginBottom, MarginBottom)

};

class M_CORE_EXPORT MAbstractLayoutPolicyStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MAbstractLayoutPolicyStyle)
};

#endif

