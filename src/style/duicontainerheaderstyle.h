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

#ifndef DUICONTAINERHEADERSTYLE_H
#define DUICONTAINERHEADERSTYLE_H

#include <duiwidgetstyle.h>

#include <QSize>

/*!
 * \internal
 */

class DUI_EXPORT DuiContainerHeaderStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiContainerHeaderStyle)
};

/*!
 * \internal
 */

class DUI_EXPORT DuiContainerHeaderStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiContainerHeaderStyle)
    DUI_STYLE_MODE(Pressed)
};


#endif // DUICONTAINERHEADERSTYLE_H
