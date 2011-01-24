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

#ifndef MLISTSTYLE_H
#define MLISTSTYLE_H

#include <QSize>
#include <mwidgetstyle.h>

/*!
    \class MListStyle
    \brief Style class for MList.

    Example:
    \code
        MListStyle {
            group-header-object-name : "myOwnGroupHeaderStyle";
        }

        #myOwnGroupHeaderStyle
        {
            color : #ff0000;
        }
    \endcode

    \ingroup styles
    \sa MLabelStyleContainer MWidgetStyle MLabelStyle \ref styling MList
*/
class M_VIEWS_EXPORT MListStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MListStyle)

    /*!
        \property MListStyle::groupHeaderObjectName
        \brief Sets object name for a group header widget

        Sets object name for a group header widget. Group header supports all styles
        which MLabel supports.

        \sa MLabelStyle
    */
    M_STYLE_ATTRIBUTE(QString,         groupHeaderObjectName,       GroupHeaderObjectName)

    /*!
        \property MListStyle::horizontalSeparatorObjectName
        \brief Sets object name for a horizontal separator widget

        Sets object name for a horizontal separator widget.
    */
    M_STYLE_ATTRIBUTE(QString, horizontalSeparatorObjectName, HorizontalSeparatorObjectName)

    /*!
        \property MListStyle::verticalSeparatorObjectName
        \brief Sets object name for a vertical separator widget

        Sets object name for a vertical separator widget.
    */
    M_STYLE_ATTRIBUTE(QString, verticalSeparatorObjectName, VerticalSeparatorObjectName)

    /*!
        \property MListStyle::groupSeparatorObjectName
        \brief Sets object name for a group separator widget

        Sets object name for a group separator widget.
    */
    M_STYLE_ATTRIBUTE(QString, groupSeparatorObjectName, GroupSeparatorObjectName)

    /*!
        \property MListStyle::listDeleteItemAnimation
        \brief Sets object name for delete item animation

        Sets object name for delete item animation.
    */
    M_STYLE_ATTRIBUTE(QString, deleteItemAnimation, DeleteItemAnimation)

    /*
        \property MListStyle::listIndexStyleName
        \brief Sets style name for a list index a.k.a. fast scroll widget

        Sets style name for a list index a.k.a. fast scroll widget
    */
    M_STYLE_ATTRIBUTE(QString, listIndexStyleName, ListIndexStyleName)
};

class M_VIEWS_EXPORT MListStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MListStyle)
};

#endif

