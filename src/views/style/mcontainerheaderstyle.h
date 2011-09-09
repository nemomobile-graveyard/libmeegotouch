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

#ifndef MCONTAINERHEADERSTYLE_H
#define MCONTAINERHEADERSTYLE_H

#include <mwidgetstyle.h>

#include <QSize>

/*!
 * \internal
 */

class M_VIEWS_EXPORT MContainerHeaderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MContainerHeaderStyle)

    /*!
        \property MContainerHeaderStyle::pressTimeout
        \brief timeout for button press.

    */
    M_STYLE_ATTRIBUTE(int, pressTimeout, PressTimeout)

    /*!
       \property MContainerHeaderStyle::maxDistanceForClick

       If the pointer gets further away from the press point than this value
       a cancel event is sent. Thus no click can take place (for this
       press-move-release cycle) even if the pointer is released over the
       button.
     */
    M_STYLE_ATTRIBUTE(int, maxDistanceForClick, MaxDistanceForClick)
};

class M_VIEWS_EXPORT MContainerHeaderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MContainerHeaderStyle)
    M_STYLE_MODE(Pressed)
};

/*! \internal_end */

#endif // MCONTAINERHEADERSTYLE_H
