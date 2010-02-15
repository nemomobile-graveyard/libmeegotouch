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

#ifndef DUICOMPLETERSTYLE_H
#define DUICOMPLETERSTYLE_H

#include <QColor>
#include <QFont>
#include <duiscenewindowstyle.h>

class DUI_EXPORT DuiCompleterStyle : public DuiSceneWindowStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiCompleterStyle)

    DUI_STYLE_ATTRIBUTE(QColor, highlightColor, HighlightColor)
    DUI_STYLE_ATTRIBUTE(int, height, Height)
    DUI_STYLE_ATTRIBUTE(int, displayBorder, DisplayBorder)
    DUI_STYLE_ATTRIBUTE(int, labelMargin, LabelMargin)
    DUI_STYLE_ATTRIBUTE(int, buttonMargin, ButtonMargin)
    DUI_STYLE_ATTRIBUTE(int, buttonWidth, ButtonWidth)
    DUI_STYLE_ATTRIBUTE(int, yPositionOffset, YPositionOffset)
};

class DUI_EXPORT DuiCompleterStyleContainer : public DuiSceneWindowStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiCompleterStyle)
};

#endif
