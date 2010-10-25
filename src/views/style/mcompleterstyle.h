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

#ifndef MCOMPLETERSTYLE_H
#define MCOMPLETERSTYLE_H

#include <QColor>
#include <QFont>
#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MCompleterStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MCompleterStyle)

    M_STYLE_ATTRIBUTE(QColor, highlightColor, HighlightColor)
    M_STYLE_ATTRIBUTE(int, height, Height)
    M_STYLE_ATTRIBUTE(int, displayBorder, DisplayBorder)
    M_STYLE_ATTRIBUTE(int, labelMargin, LabelMargin)
    M_STYLE_ATTRIBUTE(int, buttonMargin, ButtonMargin)
    M_STYLE_ATTRIBUTE(int, buttonWidth, ButtonWidth)
    M_STYLE_ATTRIBUTE(int, yPositionOffset, YPositionOffset)
};

class M_VIEWS_EXPORT MCompleterStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MCompleterStyle)
};

#endif
