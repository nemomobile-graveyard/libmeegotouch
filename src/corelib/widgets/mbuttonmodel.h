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

#ifndef MBUTTONMODEL_H
#define MBUTTONMODEL_H

#include <mwidgetmodel.h>
#include <QIcon>

/*!
    \class MButtonModel
    \brief Data model class for MButton.

    \ingroup models
    \sa MButton
*/

//We need this to due to MOC generator limitations:
using M::ButtonRole;

class M_CORE_EXPORT MButtonModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MButtonModel)

    /*!
        \property MButtonModel::iconID
        \brief ID of the icon that is displayed for a non-toggled button.
    */
    M_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
        \property MButtonModel::toggledIconID
        \brief ID of the icon that is displayed when the button is in toggled/down state.
    */
    M_MODEL_PROPERTY(QString, toggledIconID, ToggledIconID, true, QString::null)

    /*!
        \property MButtonModel::icon
        \brief QIcon that is displayed for the button.
    */
    M_MODEL_PROPERTY(QIcon, icon, Icon, false, QIcon())

    /*!
        \property MButtonModel::text
        \brief Text of the button.
    */
    M_MODEL_PROPERTY(QString, text, Text, true, QString())

    /*!
        \property MButtonModel::textVisible
        \brief Boolean value that defines whether text is displayed or not.
    */
    M_MODEL_PROPERTY(bool, textVisible, TextVisible, true, true)

    /*!
        \property MButtonModel::iconVisible
        \brief Boolean value that defines whether icon is displayed or not.
    */
    M_MODEL_PROPERTY(bool, iconVisible, IconVisible, true, true)

    /*!
        \property MButtonModel::checkable
        \brief Boolean value that defines whether button is checkable or not.
    */
    M_MODEL_PROPERTY(bool, checkable, Checkable, true, false)

    /*!
        \property MButtonModel::checked
        \brief Boolean value that defines whether button is currently checked or not.
    */
    M_MODEL_PROPERTY(bool, checked, Checked, true, false)

    /*!
        \property MButtonModel::down
        \brief Boolean value that defines whether button is currently pressed down or not.
    */
    M_MODEL_PROPERTY(bool, down, Down, true, false)

     /*!
        \property MButtonModel::role
        \brief Enum property that defines button role.
    */
    M_MODEL_PROPERTY(ButtonRole, role, Role, true,  M::InvalidRole)

public:

    /*!
        \brief Notifies the model to emit clicked signal.
    */
    void click() {
        Q_EMIT clicked();
    }

Q_SIGNALS:

    /*!
        \brief Signals that the button was clicked.
    */
    void clicked();
};

#endif

