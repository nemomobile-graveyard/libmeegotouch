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

#ifndef MLISTINDEXSTYLE_H
#define MLISTINDEXSTYLE_H

#include "mwidgetstyle.h"

class MListIndexStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListIndexStyle)

    M_STYLE_ATTRIBUTE(QString, shortcutObjectName, ShortcutObjectName)

    M_STYLE_ATTRIBUTE(qreal, appearDelay, AppearDelay)
    M_STYLE_ATTRIBUTE(qreal, appearDuration, AppearDuration)
    M_STYLE_ATTRIBUTE(qreal, fadeOutDelay, FadeOutDelay)

    M_STYLE_ATTRIBUTE(int, floatingIndexOffset, FloatingIndexOffset)
    M_STYLE_ATTRIBUTE(int, floatingIndexCount, FloatingIndexCount)
    M_STYLE_ATTRIBUTE(qreal, floatingSnapDistance, FloatingSnapDistance)
    M_STYLE_ATTRIBUTE(QString, floatingStyleName, FloatingStyleName)
};

class MListIndexStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListIndexStyle)
};

#endif
