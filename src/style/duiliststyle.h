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

#ifndef DUILISTSTYLE_H
#define DUILISTSTYLE_H

#include <QSize>
#include <duiwidgetstyle.h>

/*!
    \class DuiListStyle
    \brief Style class for DuiList.

    Example:
    \code
        DuiListStyle {
            group-header-object-name : "myOwnGroupHeaderStyle";
        }

        #myOwnGroupHeaderStyle
        {
            color : #ff0000;
        }
    \endcode

    \ingroup styles
    \sa DuiLabelStyleContainer DuiWidgetStyle DuiLabelStyle \ref styling DuiList
*/
class DUI_EXPORT DuiListStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiListStyle)

    /*!
        \property DuiListStyle::groupHeaderObjectName
        \brief Sets object name for a group header widget

        Sets object name for a group header widget. Group header supports all styles
        which DuiLabel supports.

        \sa DuiLabelStyle
    */
    DUI_STYLE_ATTRIBUTE(QString,         groupHeaderObjectName,       GroupHeaderObjectName)
};

class DUI_EXPORT DuiListStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiListStyle)
};

#endif

