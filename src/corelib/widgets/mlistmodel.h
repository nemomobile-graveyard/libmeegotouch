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

#ifndef MLISTMODEL_H
#define MLISTMODEL_H

#include <QHash>
#include <QAbstractItemModel>
#include <QRectF>
#include <QModelIndexList>
#include <QItemSelectionModel>

#include <mwidgetmodel.h>
#include "mabstractcellcreator.h"

/*!
    \class MListModel
    \brief Data model class for MList.

    \ingroup models
    \sa MList
*/
class M_CORE_EXPORT MListModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MListModel)

private:
    /*!
        \property MListModel::itemModel
        \brief Pointer to an item model.
    */
    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)

    /*!
        \property MListModel::cellCreator
        \brief Pointer to a cell creator.
     */
    M_MODEL_PTR_PROPERTY(MCellCreator *, cellCreator, CellCreator, true, 0)

    /*!
        \property MListModel::headerCreator
        \brief Pointer to a header creator.
     */
    M_MODEL_PTR_PROPERTY(MCellCreator *, headerCreator, HeaderCreator, true, 0)

    /*!
        \property MListModel::firstVisibleItem
        \brief Index of a first visible on screen item.
     */
    M_MODEL_PROPERTY(QModelIndex, firstVisibleItem, FirstVisibleItem, false, QModelIndex())

    /*!
        \property MListModel::lastVisibleItem
        \brief Index of last visible on screen item.
     */
    M_MODEL_PROPERTY(QModelIndex, lastVisibleItem, LastVisibleItem, false, QModelIndex())

    /*!
        \property MListModel::showGroups
        \brief Boolean indicating if list should show items in groups or not.
     */
    M_MODEL_PROPERTY(bool, showGroups, ShowGroups, true, false)

    /*!
        \property MListModel::columns
        \brief Indicates how many columns list should have.
     */
    M_MODEL_PROPERTY(int, columns, Columns, true, 1)

    /*!
        \property MListModel::selectionModel
        \brief Pointer to a selection model, can be shared across different lists.
     */
    M_MODEL_PTR_PROPERTY(QItemSelectionModel *, selectionModel, SelectionModel, true, NULL)

    /*!
        \property MListModel::listIsMoving
        \brief Boolean indicating if list is moving (panning) right now.
     */
    M_MODEL_PROPERTY(bool, listIsMoving, ListIsMoving, true, false)

    /*!
        \property MListModel::listIsMoving
        \brief Index of an item to which list should scroll.
     */
    M_MODEL_PROPERTY(QModelIndex, scrollToIndex, ScrollToIndex, false, QModelIndex())

    /*!
        \property MListModel::scrollHint
        \brief Indicates if item to which list should scroll should be on top, bottom etc.
        \sa MList::ScrollHint
     */
    M_MODEL_PROPERTY(int, scrollHint, ScrollHint, true, 0)

    /*!
        \property MListModel::animationMode
        \brief Indicates if a method executed on list should be animated or non-animated.
        \sa MList::AnimationMode
     */
    M_MODEL_PROPERTY(int, animationMode, AnimationMode, true, 0)

    /*!
        \property MListModel::longTap
        \brief Coordinates of long tap.
     */
    M_MODEL_PROPERTY(QPointF, longTap, LongTap, true, QPointF())

    /*!
        \property MListModel::longTapEnabled
        \brief Boolean indicating if long tap is enabled or not.
     */
    M_MODEL_PROPERTY(bool, longTapEnabled, LongTapEnabled, true, false)

    /*!
      \deprecated
    */
    M_MODEL_PROPERTY(bool, listIndexVisible, ListIndexVisible, true, false)

    /*!
        \property MListModel::listIndexDisplayMode
        \brief List index display mode flag.
        \sa MList::DisplayMode
    */
    M_MODEL_PROPERTY(int, listIndexDisplayMode, ListIndexDisplayMode, true, 0)
};

#endif

