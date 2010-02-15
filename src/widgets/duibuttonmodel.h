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

#ifndef DUIBUTTONMODEL_H
#define DUIBUTTONMODEL_H

#include <duiwidgetmodel.h>

/*!
    \class DuiButtonModel
    \brief Data model class for DuiButton.

    \ingroup models
    \sa DuiButton
*/
class DUI_EXPORT DuiButtonModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiButtonModel)

    /*!
        \property DuiButtonModel::iconID
        \brief Id of the icon that is displayed whe a non-toggled button.
    */
    DUI_MODEL_PROPERTY(QString, iconID, IconID, true, QString())

    /*!
        \property DuiButtonModel::toggledIconID
        \brief Id of the icon that is displayed when button is in toggled/down state.
    */
    DUI_MODEL_PROPERTY(QString, toggledIconID, ToggledIconID, true, QString::null)

    /*!
        \property DuiButtonModel::text
        \brief Text of the button.
    */
    DUI_MODEL_PROPERTY(QString, text, Text, true, QString())

    /*!
        \property DuiButtonModel::textVisible
        \brief Boolean value that defines whether text is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, textVisible, TextVisible, true, true)

    /*!
        \property DuiButtonModel::iconVisible
        \brief Boolean value that defines whether icon is displayed or not.
    */
    DUI_MODEL_PROPERTY(bool, iconVisible, IconVisible, true, true)

    /*!
        \property DuiButtonModel::checkable
        \brief Boolean value that defines whether button is checkable or not.
    */
    DUI_MODEL_PROPERTY(bool, checkable, Checkable, true, false)

    /*!
        \property DuiButtonModel::checked
        \brief Boolean value that defines whether button is currently checked or not.
    */
    DUI_MODEL_PROPERTY(bool, checked, Checked, true, false)

    /*!
        \property DuiButtonModel::down
        \brief Boolean value that defines whether button is currently pressed down or not.
    */
    DUI_MODEL_PROPERTY(bool, down, Down, true, false)

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

