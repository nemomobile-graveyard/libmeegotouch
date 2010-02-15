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

#include <QEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "duiobjectmenu.h"
#include "duiscenewindow_p.h"
#include "duiaction.h"
#include "duiscenemanager.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiObjectMenu)

DuiObjectMenu::DuiObjectMenu(DuiWidget *target)
    : DuiSceneWindow(new DuiSceneWindowPrivate, new DuiObjectMenuModel, DuiSceneWindow::ObjectMenu)
{
    QList<QAction *> actionList = target->actions();
    DuiActionList list;

    // go trough all actions in the target widget and add the the actions
    // which are associated to object menu to model.
    const int actionListCount = actionList.count();
    for (int i = 0; i < actionListCount; ++i) {
        DuiAction *action = qobject_cast<DuiAction *>(actionList.at(i));
        if (action && (action->location() & DuiAction::ObjectMenuLocation)) {
            list.append(action);
        }
    }

    model()->setActions(list);

    // install event filter to the target widget, so we get notified
    // when actions are added,removed or changed.
    target->installEventFilter(this);
}

DuiObjectMenu::~DuiObjectMenu()
{
}

bool DuiObjectMenu::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    DuiAction *action = NULL;

    // if the event was related to object menu actions, we'll modify the model accordingly.
    switch (event->type()) {
    case QEvent::ActionAdded:
        if ((action = qobject_cast<DuiAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & DuiAction::ObjectMenuLocation)
                model()->addAction(action);
        }
        break;
    case QEvent::ActionChanged:
        if ((action = qobject_cast<DuiAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & DuiAction::ObjectMenuLocation)
                model()->modifyAction(action);
        }
        break;
    case QEvent::ActionRemoved:
        if ((action = qobject_cast<DuiAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & DuiAction::ObjectMenuLocation)
                model()->removeAction(action);
        }
        break;
    default:
        break;
    }
    return false;
}

void DuiObjectMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}
