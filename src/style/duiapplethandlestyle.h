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

#ifndef DUIAPPLETHANDLESTYLE_H
#define DUIAPPLETHANDLESTYLE_H

#include <duiwidgetstyle.h>
#include <QPoint>
#include <QSizeF>

class DUI_EXPORT DuiAppletHandleStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(DuiAppletHandleStyle)

    //! The image to be drawn on top of the applet when it is in a broken state
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, brokenAppletImage, BrokenAppletImage)

    //! Offset of the broken applet image relative to the top right corner of the applet
    DUI_STYLE_ATTRIBUTE(QPoint, brokenAppletImageOffset, BrokenAppletImageOffset)

    //! The radius of the blur to be applied in a broken applet
    DUI_STYLE_ATTRIBUTE(int, brokenAppletBlurRadius, BrokenAppletBlurRadius)

    //! The opacity of the applet when it is in a broken state
    DUI_STYLE_ATTRIBUTE(qreal, brokenAppletOpacity, BrokenAppletOpacity)

    //! The minimum size of the applet even when the applet states it could be smaller
    DUI_STYLE_ATTRIBUTE(QSizeF, minimumAppletSize, MinimumAppletSize)

    //! The maximum size of the applet even when the applet states it could be bigger
    DUI_STYLE_ATTRIBUTE(QSizeF, maximumAppletSize, MaximumAppletSize)
};

class DUI_EXPORT DuiAppletHandleStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(DuiAppletHandleStyle)
};

#endif // DUIAPPLETHANDLESTYLE_H
