/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 
#ifndef MLISTITEMSTYLE_H
#define MLISTITEMSTYLE_H

#include <mwidgetstyle.h>

class M_VIEWS_EXPORT MListItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListItemStyle)

    M_STYLE_ATTRIBUTE(QString, downStateEffect, DownStateEffect)

    /*!
       \property MListItemStyle::maxDistanceForClick

       If the pointer gets further away from the press point than this value
       a cancel event is sent. Thus no click can take place (for this
       press-move-releae cycle) even if the pointer is released over the
       item.
     */
    M_STYLE_ATTRIBUTE(int, maxDistanceForClick, MaxDistanceForClick)
};

class M_VIEWS_EXPORT MListItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListItemStyle)
};

#endif // MLISTITEMSTYLE_H
