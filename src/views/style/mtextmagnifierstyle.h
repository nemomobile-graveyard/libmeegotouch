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

#ifndef MTEXTMAGNIFIERSTYLE_H
#define MTEXTMAGNIFIERSTYLE_H

#include "mwidgetstyle.h"

//! \internal
class MTextMagnifierStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MTextMagnifierStyle)

    //! Magnifier's border image
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, magnifierFrame, MagnifierFrame)

    //! Magnifier's clipping mask used to clip text contents into shape
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, magnifierMask, MagnifierMask)

    //! Amount of text magnification. Magnification value of 0.25 would mean 25 % increase in size.
    M_STYLE_ATTRIBUTE(qreal, magnification, Magnification)

    //! Defines offset from magnified point to actual drawn magnifier.
    M_STYLE_ATTRIBUTE(QPointF, visualOffset, VisualOffset)
};

class MTextMagnifierStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MTextMagnifierStyle)
};
//! \internal_end

#endif
