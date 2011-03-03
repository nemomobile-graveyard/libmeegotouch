/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBARSTYLE_H
#define MSTATUSBARSTYLE_H
//! \internal

#include <mscenewindowstyle.h>

class M_VIEWS_EXPORT MBorderDecorationRoundedCornersStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE(MBorderDecorationRoundedCornersStyle)

    /*!
      \brief Style name for the widget in the left corner.
    */
    M_STYLE_ATTRIBUTE(QString, leftCornerWidgetStyleName, LeftCornerWidgetStyleName)

    /*!
      \brief Style name for the widget in the right corner.
    */
    M_STYLE_ATTRIBUTE(QString, rightCornerWidgetStyleName, RightCornerWidgetStyleName)

};

class M_VIEWS_EXPORT MBorderDecorationRoundedCornersStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER(MBorderDecorationRoundedCornersStyle)
};

//! \internal_end
#endif // MSTATUSBARSTYLE_H
