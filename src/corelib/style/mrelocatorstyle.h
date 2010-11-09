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

#ifndef MRELOCATORSTYLE_H
#define MRELOCATORSTYLE_H

#include "mstyle.h"

//! \internal
class MRelocatorStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MRelocatorStyle)

    //! Vertical anchor position measured from the top of visible screen area.
    M_STYLE_ATTRIBUTE(qreal, verticalAnchorPosition, VerticalAnchorPosition)

    //! Margin on the top of visible screen area above which cursor will be relocated to anchor position.
    M_STYLE_ATTRIBUTE(qreal, topNoGoMargin, TopNoGoMargin)

    //! Margin on the bottom of visible screen area under which cursor will be relocated to anchor position.
    M_STYLE_ATTRIBUTE(qreal, bottomNoGoMargin, BottomNoGoMargin)
};

class MRelocatorStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MRelocatorStyle)

    //! Style mode to apply when physical keyboard is opened. Default mode means onscreen keyboard.
    M_STYLE_MODE(PhysicalKeyboard)
};
//! \internal_end

#endif
