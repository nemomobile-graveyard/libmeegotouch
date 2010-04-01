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

#ifndef DUICOMBOBOXMODEL_H
#define DUICOMBOBOXMODEL_H

#include <duiwidgetmodel.h>
#include <QPointer>
#include <QItemSelectionModel>

class QAbstractItemModel;

/*!
    \class DuiComboBoxModel
    \brief Data model class for DuiComboBox.

    \ingroup models
    \sa DuiComboBox
*/
class DUI_EXPORT DuiComboBoxModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiComboBoxModel)

    /*!
       \property DuiComboBoxModel::iconID
       \brief Id of the icon that is displayed on the ComboBox
    */
    DUI_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
       \property DuiComboBoxModel::iconVisible
       \brief Boolean value that defines whether icon is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, iconVisible, IconVisible, true, true)

    /*!
       \property DuiComboBoxModel::title
       \brief Title of the comboBox.
    */
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property DuiComboBoxModel::itemModel
        \brief QAbstractItemModel which used by DuiComboBox
    */
    DUI_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)

    /*!
        \property DuiComboBoxModel::selectionModel
        \brief selectionModel keeps track of itemModel's selected items
    */
    DUI_MODEL_PROPERTY(QPointer<QItemSelectionModel>, selectionModel, SelectionModel, true, 0)

};

#endif
