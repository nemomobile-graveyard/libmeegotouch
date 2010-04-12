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

#ifndef MEXTENSIONHANDLESTYLE_H
#define MEXTENSIONHANDLESTYLE_H

#include <mwidgetstyle.h>
#include <QPoint>
#include <QSizeF>

//! \internal
class MExtensionHandleStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MExtensionHandleStyle)

    //! The image to be drawn on top of the extension when it is in a broken state
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, brokenImage, BrokenImage)

    //! Offset of the "broken" image relative to the top right corner
    M_STYLE_ATTRIBUTE(QPoint, brokenImageOffset, BrokenImageOffset)

    //! The radius of the blur to be applied in a broken state
    M_STYLE_ATTRIBUTE(int, brokenBlurRadius, BrokenBlurRadius)

    //! Opacity in broken state
    M_STYLE_ATTRIBUTE(qreal, brokenOpacity, BrokenOpacity)

    //! The size used if the remote process requests a smaller size
    M_STYLE_ATTRIBUTE(QSizeF, forcedMinimumSize, ForcedMinimumSize)

    //! The size used if the remote process requests a larger size
    M_STYLE_ATTRIBUTE(QSizeF, forcedMaximumSize, ForcedMaximumSize)
};

class MExtensionHandleStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MExtensionHandleStyle)
};
//! \internal_end

#endif // MEXTENSIONHANDLESTYLE_H
