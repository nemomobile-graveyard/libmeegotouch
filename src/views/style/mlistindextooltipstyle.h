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
 
#ifndef MLISTINDEXTOOLTIPSTYLE_H
#define MLISTINDEXTOOLTIPSTYLE_H

#include <mstylablewidgetstyle.h>

class MListIndexTooltipStyle : public MStylableWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListIndexTooltipStyle)

    M_STYLE_PTR_ATTRIBUTE(QPixmap *, arrowPixmap, ArrowPixmap)
};

class MListIndexTooltipStyleContainer : public MStylableWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListIndexTooltipStyle)
};

#endif // MLISTINDEXTOOLTIPSTYLE_H
