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

#ifndef MMASHUPCANVASSTYLE_H_
#define MMASHUPCANVASSTYLE_H_

#include <MWidgetStyle>

//! \internal
/*!
 * MMashupCanvasStyle is the style class for MMashupCanvas.
 */
class M_EXTENSIONS_EXPORT MMashupCanvasStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MMashupCanvasStyle)

    //! Whether the applets on this mashup canvas should be inside containers or not
    M_STYLE_ATTRIBUTE(bool, containerMode, ContainerMode)
};

/*!
 * MMashupCanvasStyleContainer is the style container class for MMashupCanvas.
 */
class M_EXTENSIONS_EXPORT MMashupCanvasStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MMashupCanvasStyle)
};
//! \internal_end

#endif /* MMASHUPCANVASSTYLE_H_ */
