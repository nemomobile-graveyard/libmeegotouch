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

#ifndef MCONTENTITEMMODEL_H__
#define MCONTENTITEMMODEL_H__

#include <mwidgetmodel.h>

class QPixmap;
class QImage;

class M_CORE_EXPORT MContentItemModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MContentItemModel)

public:
    /*!
        \property MContentItemModel::title
        \brief Title of item.
    */
    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property MContentItemModel::subtitle
        \brief Subtitle of item.
    */
    M_MODEL_PROPERTY(QString, subtitle, Subtitle, true, QString())

    /*!
      \property MContentItemModel::itemStyle
      \brief Item style.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(int, itemStyle, ItemStyle, true, 0)

    /*!
      \property MContentItemModel::itemMode
      \brief Item mode.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(int, itemMode, ItemMode, true, 0)

    /*!
      \property MContentItemModel::itemPixmap
      \brief Item thumbnail.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QPixmap, itemPixmap, ItemPixmap, false, NULL)

    /*!
      \property MContentItemModel::itemImage
      \brief Item thumbnail.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QImage, itemImage, ItemImage, false, QImage())

    /*!
      \property MContentItemModel::itemImageID
      \brief Item thumbnail theme ID.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QString, itemImageID, ItemImageID, true, QString())

    /*!
      \property MContentItemModel::itemImageSize
      \brief Contains size of the thumbnail (set thumbnail from theme using itemImageID).

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QSize, itemImageSize, ItemImageSize, true, QSize())

    M_MODEL_PROPERTY(QImage, itemQImage, ItemQImage, false, QImage())

    /*!
      \property MContentItemModel::selected
      \brief Contains selection status of item.
      */
    M_MODEL_PROPERTY(bool, selected, Selected, true, false)

    /*!
     \property MContentItemModel::optionalItemPixmap
     \brief Contains optional icon on the right-hand side
     */
    M_MODEL_PROPERTY(QPixmap, optionalPixmap, OptionalPixmap, false, NULL)

    /*!
     \property MContentItemModel::optionalItemImage
     \brief Contains optional icon on the right-hand side
     */
    M_MODEL_PROPERTY(QImage, optionalImage, OptionalImage, false, QImage()) 

    /*!
      \property MContentItemModel::optionalImageID
      \brief Contains theme ID of optional icon on the right-hand side.

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QString, optionalImageID, OptionalImageID, true, QString())

    /*!
      \property MContentItemModel::optionalImageSize
      \brief Contains size of the optional icon (set optional icon from theme with optionalImageID).

      \sa MContentItemView
      */
    M_MODEL_PROPERTY(QSize, optionalImageSize, OptionalImageSize, true, QSize())

    /*!
        \property MContentItemModel::additionalItem
        \brief MWidget as e.g. progress bar, visible below the title.
         In case there is already a subtitle it will be replaced.
    */
    M_MODEL_PTR_PROPERTY(MWidget*, additionalItem, AdditionalItem, true, NULL)

    /*!
        \property MContentItemModel::smallItem
        \brief MWidget as e.g. progress bar, visible on the lower right corner
    */
    M_MODEL_PTR_PROPERTY(MWidget*, smallItem, SmallItem, true, NULL)
};

#endif
