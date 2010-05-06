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

#ifndef MTOOLBARSTYLE_H
#define MTOOLBARSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MToolbarStyle
    \brief Style class for m toolbar.

    \ingroup styles
    \sa MToolbarStyleContainer MWidgetStyle \ref styling MToolBar MToolBarView
*/

class M_EXPORT MToolbarStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MToolbarStyle)
};

/*!
    \class MToolbarStyleContainer
    \brief Style mode container class for MToolbarStyle.

    \ingroup styles
    \sa MToolbarStyle
*/
class M_EXPORT MToolbarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MToolbarStyle)
};

#endif

