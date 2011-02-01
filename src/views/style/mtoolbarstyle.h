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

class M_VIEWS_EXPORT MToolbarStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MToolbarStyle)

    M_STYLE_PTR_ATTRIBUTE(MScalableImage*, dropShadowImage, DropShadowImage)

    /*!
     * \brief Max number of slots in the bar (-1 = unlimited)
     */
    M_STYLE_ATTRIBUTE(int, capacity, Capacity)

    /*!
     * \brief Tool bar buttons have icon or not
     */
    M_STYLE_ATTRIBUTE(bool, hasIcons, HasIcons)

    /*!
     * \brief Tool bar buttons have text label or not
     */
    M_STYLE_ATTRIBUTE(bool, hasLabels, HasLabels)

    /*!
     * \brief Tool bar buttons have spacers between them to layout them equally in the available space
     */
    M_STYLE_ATTRIBUTE(bool, hasSpaces, HasSpaces)

    /*!
     * \brief Tool bar actions that have only text applied are handled with default button view instead of custom tool or tab bar views.
     */
    M_STYLE_ATTRIBUTE(bool, labelOnlyAsCommonButton, LabelOnlyAsCommonButton)

    /*!
     * \brief Enable tool bar widget centering to scene window parent of the tool bar
     */
    M_STYLE_ATTRIBUTE(bool, centerToParent, CenterToParent)

    /*!
     * \brief Enable just common (default view based) button centering to scene window parent of the tool bar
     */
    M_STYLE_ATTRIBUTE(bool, centerCommonButtonsToParent, CenterCommonButtonsToParent)

    /*!
     * \brief Margin that is applied according to widget alignment:
       Left side of the tool bar when widgets/buttons are aligned to the left.
       Right side of the tool bar when widgets/buttons are aligned to the right.
       Both sides when alignment is justify.
       Neither side on HCenter alignment.
     */
    M_STYLE_ATTRIBUTE(int, alignmentMargins, AlignmentMargins)

    /*!
     * \brief Style name for label only button that are based on common style
     */
    M_STYLE_ATTRIBUTE(QString, labelOnlyCommonButtonStyleName, LabelOnlyCommonButtonStyleName)

    /*!
     * \brief Style name for tool bar specific label only button
     */
    M_STYLE_ATTRIBUTE(QString, labelOnlyButtonStyleName, LabelOnlyButtonStyleName)

    /*!
     * \brief Style name for button containing just icon
     */
    M_STYLE_ATTRIBUTE(QString, iconButtonStyleName, IconButtonStyleName)

    /*!
     * \brief Style name for button containing both icon and label
     */
    M_STYLE_ATTRIBUTE(QString, iconLabelButtonStyleName, IconLabelButtonStyleName)
};

/*!
    \class MToolbarStyleContainer
    \brief Style mode container class for MToolbarStyle.

    \ingroup styles
    \sa MToolbarStyle
*/
class M_VIEWS_EXPORT MToolbarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MToolbarStyle)
};

#endif

