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

#ifndef DUIOBJECTMENUMODEL_H
#define DUIOBJECTMENUMODEL_H

#include <duiscenewindowmodel.h>

class DuiAction;

typedef QList<DuiAction *> DuiActionList;

/*!
    \class DuiObjectMenuModel
    \brief DuiObjectMenuModel contains a number of object menu actions related to one widget.

    \ingroup models

    \sa DuiObjectMenu
*/
class DUI_EXPORT DuiObjectMenuModel : public DuiSceneWindowModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiObjectMenuModel)

    /*!
        \property DuiObjectMenuModel::actions
        \brief A list of object menu actions.

        This property contains all object menu actions.
        This list is modified by DuiObjectMenu via addAction(), removeAction() and modifyAction() and is always up to date.
    */
    DUI_MODEL_PROPERTY(DuiActionList, actions, Actions, true, DuiActionList())

public:
    /*!
        \brief Adds the \a action to the actions list.

        This method should be used instead of modifying the actions list directly.
        When the action has been added, this method will emit a actionAdded() signal.
     */
    void addAction(DuiAction *action);

    /*!
        \brief Removes the \a action from the actions list.

        This method should be used instead of modifying the  actions list directly.
        When the action has been removed, this method will emit a actionRemoved() signal.
     */
    void removeAction(DuiAction *action);

    /*!
        \brief Emits a actionModified() signal.

        When modifying an action, this method should be used to inform all participants of the modification.
        This method will emit a actionModified() signal.
     */
    void modifyAction(DuiAction *action);

Q_SIGNALS:
    /*!
        \brief This signal is emitted when an action has been added to actions list.
     */
    void actionAdded(DuiAction *action);

    /*!
        \brief This signal is emitted when an action has been removed from actions list.
     */
    void actionRemoved(DuiAction *action);

    /*!
        \brief This signal is emitted when an action has been modified in actions list.
     */
    void actionModified(DuiAction *action);
};

#endif

