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

#ifndef MPOPUPLISTMODEL_H
#define MPOPUPLISTMODEL_H

#include "mdialogmodel.h"
#include <QPointer>
#include <QItemSelectionModel>

class QAbstractItemModel;
/*!
    \class MPopupListModel
    \brief Data model class for MPopupList.

    \ingroup models
    \sa MPopupListModel
*/

class M_CORE_EXPORT MPopupListModel : public MDialogModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MPopupListModel)

    /*!
        \property MPopupListModel::itemModel
        \brief QAbstractItemModel which used by MPopupList
    */
    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)

    /*!
        \property MPopupListModel::selectionModel
        \brief selectionModel keeps track of itemModel's selected items
    */
    M_MODEL_PROPERTY(QPointer<QItemSelectionModel>, selectionModel, SelectionModel, true, 0)

    /*!
        \property MPopupListModel::scrollToIndex
        \brief this property is used to tell the view to scroll to the given item index
    */
    M_MODEL_PROPERTY(QModelIndex, scrollToIndex, ScrollToIndex, false, QModelIndex())
};

#endif
