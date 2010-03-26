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

#ifndef DUIPOPUPLISTMODEL_H
#define DUIPOPUPLISTMODEL_H

#include "duidialogmodel.h"
#include <QPointer>
#include <QItemSelectionModel>

class QAbstractItemModel;
/*!
    \class DuiPopupListModel
    \brief Data model class for DuiPopupList.

    \ingroup models
    \sa DuiPopupListModel
*/

class DUI_EXPORT DuiPopupListModel : public DuiDialogModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiPopupListModel)

    /*!
        \property DuiPopupListModel::itemModel
        \brief QAbstractItemModel which used by DuiPopupList
    */
    DUI_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)

    /*!
        \property DuiPopupListModel::selectionModel
        \brief selectionModel keeps track of itemModel's selected items
    */
    DUI_MODEL_PROPERTY(QPointer<QItemSelectionModel>, selectionModel, SelectionModel, true, 0)
};

#endif
