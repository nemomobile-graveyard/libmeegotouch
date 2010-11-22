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

#ifndef MOBJECTMENUMODEL_H
#define MOBJECTMENUMODEL_H

#include <mscenewindowmodel.h>

class MAction;

typedef QList<MAction *> MActionList;

/*!
    \class MObjectMenuModel
    \brief MObjectMenuModel contains a number of object menu actions related to one widget.

    \ingroup models

    \sa MObjectMenu
*/
class M_CORE_EXPORT MObjectMenuModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MObjectMenuModel)

    /*!
        \property MObjectMenuModel::actions
        \brief A list of object menu actions.

        This property contains all object menu actions.
        This list is modified by MObjectMenu via addAction(), removeAction() and modifyAction() and is always up to date.
    */
    M_MODEL_PROPERTY(MActionList, actions, Actions, true, MActionList())

    /*!
        \property MObjectMenuModel::cursorPosition
        \brief Position of the cursor that invokes the object menu

        This property contains the position of the cursor that causes menu to appear.
    */
    M_MODEL_PROPERTY(QPointF, cursorPosition, CursorPosition, true, QPointF())

    /*!
        \property MObjectMenuModel::title
        \brief Title/header text for the object menu.
    */
    M_MODEL_PROPERTY(QString, title, Title, true, QString())

    /*!
        \property MObjectMenuModel::iconId
        \brief IconId for the icon for the object menu.
    */
    M_MODEL_PROPERTY(QString, iconId, IconId, true, QString())

public:
    /*!
        \brief Adds the \a action to the actions list.

        This method should be used instead of modifying the actions list directly.
        When the action has been added, this method will emit a actionAdded() signal.
     */
    void addAction(MAction *action);

    /*!
        \brief Removes the \a action from the actions list.

        This method should be used instead of modifying the  actions list directly.
        When the action has been removed, this method will emit a actionRemoved() signal.
     */
    void removeAction(MAction *action);

    /*!
        \brief Emits a actionModified() signal.

        When modifying an action, this method should be used to inform all participants of the modification.
        This method will emit a actionModified() signal.
     */
    void modifyAction(MAction *action);

Q_SIGNALS:
    /*!
        \brief This signal is emitted when an action has been added to actions list.
     */
    void actionAdded(MAction *action);

    /*!
        \brief This signal is emitted when an action has been removed from actions list.
     */
    void actionRemoved(MAction *action);

    /*!
        \brief This signal is emitted when an action has been modified in actions list.
     */
    void actionModified(MAction *action);

private:
    M_MODEL_PROPERTY(QString, contentURI, ContentURI, true, QString())
};

#endif
