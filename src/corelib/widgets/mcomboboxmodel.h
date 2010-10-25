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

#ifndef MCOMBOBOXMODEL_H
#define MCOMBOBOXMODEL_H

#include <mwidgetmodel.h>
#include <QPointer>
#include <QItemSelectionModel>

class QAbstractItemModel;

/*!
    \class MComboBoxModel
    \brief Data model class for MComboBox.

    \ingroup models
    \sa MComboBox
*/
class M_CORE_EXPORT MComboBoxModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MComboBoxModel)

    /*!
       \property MComboBoxModel::iconID
       \brief Id of the icon that is displayed on the ComboBox
    */
    M_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
       \property MComboBoxModel::iconVisible
       \brief Boolean value that defines whether icon is displayed or not.
    */
    M_MODEL_PROPERTY(bool, iconVisible, IconVisible, true, true)

    /*!
       \property MComboBoxModel::title
       \brief Title of the comboBox.
    */
    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property MComboBoxModel::progressIndicatorVisible
        \brief Boolean value that defines whether a progress indicator is visible or not.
        If the progress indicator is visible, a subtitle is hidden.
    */
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property MComboBoxModel::itemModel
        \brief QAbstractItemModel which used by MComboBox
    */
    M_MODEL_PTR_PROPERTY(QAbstractItemModel *, itemModel, ItemModel, true, 0)

    /*!
        \property MComboBoxModel::selectionModel
        \brief selectionModel keeps track of itemModel's selected items
    */
    M_MODEL_PROPERTY(QPointer<QItemSelectionModel>, selectionModel, SelectionModel, true, 0)

};

#endif
