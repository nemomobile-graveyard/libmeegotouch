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

#ifndef MDIALOGMODEL_H
#define MDIALOGMODEL_H

#include "mscenewindowmodel.h"
#include "mpannablewidget.h"
#include <mnamespace.h>

#include <QList>

class MButtonModel;
typedef QList<MButtonModel *> MDialogButtonsList;

class M_CORE_EXPORT MDialogModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MDialogModel)
    M_MODEL_CUSTOM_DESTRUCTOR

private:

    M_MODEL_PROPERTY(int, resultCode, ResultCode, true, 0)

    /*!
        \property MDialogModel::closeButtonVisible
        \deprecated Use MDialogStyle::hasCloseButton instead. OBS: Style properties
                    are set in CSS files and consumed by the assigned view.

     */
    M_MODEL_PROPERTY(bool, closeButtonVisible, CloseButtonVisible, true, true)
    M_MODEL_PROPERTY(bool, buttonBoxVisible, ButtonBoxVisible, true, true)

    /*!
        \property MDialogModel::titleBarVisible
        \deprecated Use MDialogStyle::hasTitleBar instead. OBS: Style properties
                    are set in CSS files and consumed by the assigned view.

     */
    M_MODEL_PROPERTY(bool, titleBarVisible, TitleBarVisible, true, true)
    M_MODEL_PROPERTY(QString, title, Title, true, QString())
    M_MODEL_PROPERTY(bool, system, System, true, false)
    M_MODEL_PROPERTY(bool, modal, Modal, true, true)
    M_MODEL_PROPERTY(bool, systemModal, SystemModal, true, false)
    M_MODEL_PROPERTY(bool, alwaysPannable, AlwaysPannable, true, true)

    /*!
        \property MDialogModel::contentsVerticalPanningPolicy
        \brief Vertical panning policy for MDialog's contents
        \sa MPannableWidget::PanningPolicy

     */
    M_MODEL_PROPERTY(MPannableWidget::PanningPolicy, contentsVerticalPanningPolicy, ContentsVerticalPanningPolicy, true, MPannableWidget::PanningAsNeeded)


    /*!
        \property MDialogModel::progressIndicatorVisible
        \brief Whether the progress indicator on the title bar is visible.
    */
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)

    /*!
        \property MDialogModel::centralWidget
        \brief Central widget for the dialog.
    */
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, centralWidget, CentralWidget, true, new MWidget)

    /*!
        \property MDialogModel::buttons
        \brief List of buttons int the button box.
    */
    M_MODEL_PROPERTY(MDialogButtonsList, buttons, Buttons, true, MDialogButtonsList())
    M_MODEL_PROPERTY(QString, titleBarIconId, TitleBarIconId, true, QString())

public:

    /*!
     * Non-const getter for centralWidget property.
     * Used by view classes to add the central widget to their internal layouts.
     */
    QGraphicsWidget *centralWidget();

    /*!
     * \brief Adds a given \a button to the button box.
     *
     * Nothing happens if the button was already added, otherwise
     * its reference count is increased.
     *
     * \param button A button
     */
    void addButton(MButtonModel *button);

    /*!
     * \brief Creates a standard button specified by the \a button value.
     *
     * The standard button has a predefined caption.
     *
     * Returns a pointer to the newly created button. If \a button was already
     * added, his model is returned instead and no new button is created.
     *
     * MDialogModel holds ownership over the created button.
     *
     * \sa M::StandardButton, button(M::StandardButton which)
     */
    MButtonModel *addButton(M::StandardButton button);

    /*!
     * \brief Removes \a button from the dialog without deleting it.
     *
     * The ownership of \a button is passed on to the caller.
     *
     * \sa addButton()
     */
    void removeButton(MButtonModel *button);

    /*!
     * \brief Returns a pointer to the standard button specified by \a which.
     *
     * Returns a null pointer if there is no given standard button in the
     * button box.
     *
     * Changing the properties of the returned button can make it not
     * be considered as a standard button anymore by the dialog.
     */
    MButtonModel *button(M::StandardButton which);

    /*!
     * \brief Returns a non-const pointer to the button at the given \a index.
     *
     * \a index is relative to buttons property.
     */
    MButtonModel *button(int index);

    /*!
     * \brief Returns the standard button enum value corresponding to the given button,
     *        or NoButton if the given button isn't a standard button.
     *
     * \sa button(M::StandardButton)
     */
    M::StandardButton standardButton(const MButtonModel *button) const;

    /*!
     * \brief Maps M::StandardButton enum values to corresponding M::ButtonRole.
     *
     * \sa button(M::StandardButton)
     */
    static M::ButtonRole roleFor(M::StandardButton button);
};

#endif

