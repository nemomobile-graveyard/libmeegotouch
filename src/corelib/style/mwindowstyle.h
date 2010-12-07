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

#ifndef MWINDOWSTYLE_H
#define MWINDOWSTYLE_H

#include <mstyle.h>
#include <QColor>

class QPixmap;

//! \internal

/** \brief Defines a style for a MWindow class.
 */
class MWindowStyle : public MStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MWindowStyle)

    /*!
     Sets the default background color when opening a window. If
     x11StartupPixmap is defined the color will be ignored.
    */
    M_STYLE_ATTRIBUTE(QColor, startupFillColor, StartupFillColor)

    /*!
     The pixmap used to initially fill the window before it is shown.
     The pixmap needs to be an X11 pixmap, to ensure this make sure it has
     forcex11 in its filename.
     */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, x11StartupPixmap, X11StartupPixmap)


};

class MWindowStyleContainer : public MStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MWindowStyle)
};

//! \internal_end

#endif // MWINDOWSTYLE_H
