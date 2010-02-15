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

#ifndef DUIDIALOGMODEL_H
#define DUIDIALOGMODEL_H

#include "duiscenewindowmodel.h"
#include <duinamespace.h>

#include <QList>

class DuiButtonModel;
typedef QList<DuiButtonModel *> DuiDialogButtonsList;

class DUI_EXPORT DuiDialogModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiDialogModel)

private:

    DUI_MODEL_PROPERTY(int, resultCode, ResultCode, true, 0)

    DUI_MODEL_PROPERTY(bool, closeButtonVisible, CloseButtonVisible, true, true)
    DUI_MODEL_PROPERTY(bool, buttonBoxVisible, ButtonBoxVisible, true, true)
    DUI_MODEL_PROPERTY(bool, titleBarVisible, TitleBarVisible, true, true)
    DUI_MODEL_PROPERTY(QString, title, Title, true, QString())
    DUI_MODEL_PROPERTY(bool, windowModal, WindowModal, true, false)

    /*!
        \property DuiDialogModel::progressIndicatorVisible
        \brief Whether the progress indicator on the title bar is visible.
    */
    DUI_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property DuiDialogModel::centralWidget
        \brief Central widget for the dialog.
    */
    DUI_MODEL_PTR_PROPERTY(DuiWidget *, centralWidget, CentralWidget, true, new DuiWidget)

    /*!
        \property DuiDialogModel::buttons
        \brief List of buttons int the button box.
    */
    DUI_MODEL_PROPERTY(DuiDialogButtonsList, buttons, Buttons, true, DuiDialogButtonsList())

public:

    /*!
     * Non-const getter for centralWidget property.
     * Used by view classes to add the central widget to their internal layouts.
     */
    DuiWidget *centralWidget();

    /*!
     * \brief Adds a given \a button to the button box.
     *
     * Nothing happens if the button was already added, otherwise
     * its reference count is increased.
     *
     * \param button A button
     */
    void addButton(DuiButtonModel *button);

    /*!
     * \brief Creates a standard button specified by the \a button value.
     *
     * The standard button has a predefined caption.
     *
     * Returns a pointer to the newly created button. If \a button was already
     * added, his model is returned instead and no new button is created.
     *
     * DuiDialogModel holds ownership over the created button.
     *
     * \sa Dui::StandardButton, button(Dui::StandardButton which)
     */
    DuiButtonModel *addButton(Dui::StandardButton button);

    /*!
     * \brief Removes \a button from the dialog without deleting it.
     *
     * The ownership of \a button is passed on to the caller.
     *
     * \sa addButton()
     */
    void removeButton(DuiButtonModel *button);

    /*!
     * \brief Returns a pointer to the standard button specified by \a which.
     *
     * Returns a null pointer if there is no given standard button in the
     * button box.
     *
     * Changing the properties of the returned button can make it not
     * be considered as a standard button anymore by the dialog.
     */
    DuiButtonModel *button(Dui::StandardButton which);

    /*!
     * \brief Returns a non-const pointer to the button at the given \a index.
     *
     * \a index is relative to buttons property.
     */
    DuiButtonModel *button(int index);

    /*!
     * \brief Returns the standard button enum value corresponding to the given button,
     *        or NoButton if the given button isn't a standard button.
     *
     * \sa button(Dui::StandardButton)
     */
    Dui::StandardButton standardButton(const DuiButtonModel *button) const;
};

#endif

