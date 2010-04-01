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

#ifndef DUICONTENTITEMMODEL_H__
#define DUICONTENTITEMMODEL_H__

#include <duiwidgetmodel.h>

class QPixmap;
class QImage;

class DUI_EXPORT DuiContentItemModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiContentItemModel)

public:
    /*!
        \property DuiContentItemModel::title
        \brief Title of item.
    */
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property DuiContentItemModel::subtitle
        \brief Subtitle of item.
    */
    DUI_MODEL_PROPERTY(QString, subtitle, Subtitle, true, QString())

    /*!
      \property DuiContentItemModel::itemStyle
      \brief Item style.

      \sa DuiContentItemView
      */
    DUI_MODEL_PROPERTY(int, itemStyle, ItemStyle, true, 0)

    /*!
      \property DuiContentItemModel::itemMode
      \brief Item mode.

      \sa DuiContentItemView
      */
    DUI_MODEL_PROPERTY(int, itemMode, ItemMode, true, 0)

    /*!
      \property DuiContentItemModel::itemPixmap
      \brief Item thumbnail.

      \sa DuiContentItemView
      */
    DUI_MODEL_PROPERTY(QPixmap, itemPixmap, ItemPixmap, false, NULL)

    /*!
      \property DuiContentItemModel::itemImage
      \brief Item thumbnail.

      \sa DuiContentItemView
      */
    DUI_MODEL_PROPERTY(QImage, itemImage, ItemImage, false, QImage())

    DUI_MODEL_PROPERTY(QImage, itemQImage, ItemQImage, false, QImage())

    /*!
      \property DuiContentItemModel::selected
      \brief Contains selection status of item.
      */
    DUI_MODEL_PROPERTY(bool, selected, Selected, true, false)

    /*!
     \property DuiContentItemModel::optionalItemPixmap
     \brief Contains optional icon on the right-hand side
     */
    DUI_MODEL_PROPERTY(QPixmap, optionalPixmap, OptionalPixmap, false, NULL)

    /*!
     \property DuiContentItemModel::optionalItemImage
     \brief Contains optional icon on the right-hand side
     */
    DUI_MODEL_PROPERTY(QImage, optionalImage, OptionalImage, false, QImage()) 
};

#endif
