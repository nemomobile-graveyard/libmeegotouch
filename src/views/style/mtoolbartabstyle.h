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

#ifndef MTOOLBARTABSTYLE_H
#define MTOOLBARTABSTYLE_H

#include <mtoolbarstyle.h>

/*!
    \class MToolbarTabStyle
    \brief Style class for m toolbar with tab mode.

    \ingroup styles
    \sa MToolbarTabStyleContainer MWidgetStyle \ref styling MToolBar MToolbarTabView
*/

class M_VIEWS_EXPORT MToolbarTabStyle : public MToolbarStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MToolbarTabStyle)
};

/*!
    \class MToolbarTabStyleContainer
    \brief Style mode container class for MToolbarTabStyle.

    \ingroup styles
    \sa MToolbarTabStyle
*/
class M_VIEWS_EXPORT MToolbarTabStyleContainer : public MToolbarStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MToolbarTabStyle)
};

#endif

