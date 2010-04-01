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

#ifndef DUIMASHUPCANVASSTYLE_H_
#define DUIMASHUPCANVASSTYLE_H_

#include <DuiWidgetStyle>

/*!
 * DuiMashupCanvasStyle is the style class for DuiMashupCanvas.
 */
class DUI_EXPORT DuiMashupCanvasStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiMashupCanvasStyle)

    //! Whether the applets on this mashup canvas should be inside containers or not
    DUI_STYLE_ATTRIBUTE(bool, containerMode, ContainerMode)
};

/*!
 * DuiMashupCanvasStyleContainer is the style container class for DuiMashupCanvas.
 */
class DUI_EXPORT DuiMashupCanvasStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiMashupCanvasStyle)
};

#endif /* DUIMASHUPCANVASSTYLE_H_ */
