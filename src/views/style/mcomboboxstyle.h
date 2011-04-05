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

#ifndef MCOMBOBOXSTYLE_H
#define MCOMBOBOXSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MComboBoxStyle
    \brief Style class for MComoBox.

    \ingroup styles
    \sa MComboBoxStyleContainer MWidgetStyle \ref styling MComboBox MComboBoxView
*/
class M_VIEWS_EXPORT MComboBoxStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MComboBoxStyle)

    /*!
      \deprecated do not use
    */
    M_STYLE_ATTRIBUTE(int, itemMode, ItemMode)

    /*!
      \deprecated do not use
    */
    M_STYLE_ATTRIBUTE(QString, contentItemObjectName, ContentItemObjectName)

    /*!
      \property MComboBoxStyle::indicatorImage
      \brief Image ID of combobox's indicator arrows.
    */
    M_STYLE_ATTRIBUTE(QString, indicatorImage, IndicatorImage)

    /*!
      \property MComboBoxStyle::buttonStyleName
      \brief Style name of the pressable widget.
    */
    M_STYLE_ATTRIBUTE(QString, buttonStyleName, ButtonStyleName)

    /*!
      \property MComboBoxStyle::titleObjectName
      \brief Object name of title label.
    */
    M_STYLE_ATTRIBUTE(QString, titleObjectName, TitleObjectName)

    /*!
      \property MComboBoxStyle::subTitleObjectName
      \brief Object name of subtitle label.
    */
    M_STYLE_ATTRIBUTE(QString, subTitleObjectName, SubTitleObjectName)

    /*!
      \property MComboBoxStyle::iconObjectName
      \brief Object name of icon.
    */
    M_STYLE_ATTRIBUTE(QString, iconObjectName, IconObjectName)

    /*!
      \property MComboBoxStyle::progressIndicatorObjectName
      \brief Object name of progress indicator.
    */
    M_STYLE_ATTRIBUTE(QString, progressIndicatorObjectName, progressIndicatorObjectName)

    /*!
      \property MComboBoxStyle::indicatorObjectName
      \brief Object name of combobox's indicator arrows.
    */
    M_STYLE_ATTRIBUTE(QString, indicatorObjectName, indicatorObjectName)

    /*!
      \property MComboBoxStyle::hasTitle
      \brief Controls whether a title label is displayed.
    */
    M_STYLE_ATTRIBUTE(bool, hasTitle, HasTitle)

    /*!
      \property MComboBoxStyle::separateTitle
      \brief If true, combobox's title will be above button.
    */
    M_STYLE_ATTRIBUTE(bool, separateTitle, SeparateTitle)

    /*!
      \property MComboBoxStyle::popupListType
      \brief Specifies the view type of the popup list to use.
    */
    M_STYLE_ATTRIBUTE(QString, popupListType, PopupListType)
};

/*!
    \class MComboBoxStyleContainer
    \brief Style mode container class for MComboBoxStyle.

    \ingroup styles
    \sa MComboBoxStyle
*/
class M_VIEWS_EXPORT MComboBoxStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MComboBoxStyle)

    /*!
        \brief Style mode for a pressed comboBox.

        Mode is activated when a comboBox is pressed down and deactivated when
        comboBox is released.
     */
    M_STYLE_MODE(Pressed)
};

#endif

