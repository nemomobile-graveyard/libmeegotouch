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

#ifndef DUICOMBOBOXSTYLE_H
#define DUICOMBOBOXSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiComboBoxStyle
    \brief Style class for DuiComoBox.

    \ingroup styles
    \sa DuiComboBoxStyleContainer DuiWidgetStyle \ref styling DuiComboBox DuiComboBoxView
*/
class DUI_EXPORT DuiComboBoxStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiComboBoxStyle)

    /*!
        \property DuiComboBoxStyle::itemMode
        \brief itemMode of internal DuiContentItem.
        \see DuiContentItem::itemMode
    */
    DUI_STYLE_ATTRIBUTE(int, itemMode, ItemMode)

    /*!
      \property DuiComboBoxStyle::contentItemObjectName
      \brief Object name of internal DuiContentItem.
    */
    DUI_STYLE_ATTRIBUTE(QString, contentItemObjectName, ContentItemObjectName)
};

/*!
    \class DuiComboBoxStyleContainer
    \brief Style mode container class for DuiComboBoxStyle.

    \ingroup styles
    \sa DuiComboBoxStyle
*/
class DUI_EXPORT DuiComboBoxStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiComboBoxStyle)

    /*!
        \brief Style mode for a pressed comboBox.

        Mode is activated when a comboBox is pressed down and deactivated when
        comboBox is released.
     */
    DUI_STYLE_MODE(Pressed)
};

#endif

