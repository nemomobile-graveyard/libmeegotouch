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

#ifndef DUITOOLBARTABSTYLE_H
#define DUITOOLBARTABSTYLE_H

#include <duitoolbarstyle.h>

/*!
    \class DuiToolbarTabStyle
    \brief Style class for dui toolbar with tab mode.

    \ingroup styles
    \sa DuiToolbarTabStyleContainer DuiWidgetStyle \ref styling DuiToolBar DuiToolbarTabView
*/

class DUI_EXPORT DuiToolbarTabStyle : public DuiToolbarStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiToolbarTabStyle)
};

/*!
    \class DuiToolbarTabStyleContainer
    \brief Style mode container class for DuiToolbarTabStyle.

    \ingroup styles
    \sa DuiToolbarTabStyle
*/
class DUI_EXPORT DuiToolbarTabStyleContainer : public DuiToolbarStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiToolbarTabStyle)
};

#endif

