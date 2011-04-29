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

#ifndef MLISTINDEXSTYLE_H
#define MLISTINDEXSTYLE_H

#include "mwidgetstyle.h"

class MListIndexStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListIndexStyle)

    M_STYLE_ATTRIBUTE(QString, shortcutStyleName, ShortcutStyleName)
    M_STYLE_ATTRIBUTE(QString, tooltipStyleName, TooltipStyleName)

    M_STYLE_ATTRIBUTE(int, appearDelay, AppearDelay)
    M_STYLE_ATTRIBUTE(int, fadeDuration, FadeDuration)

    M_STYLE_PTR_ATTRIBUTE(QPixmap*, positionIndicator, PositionIndicator)
};

class MListIndexStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListIndexStyle)
};

#endif
