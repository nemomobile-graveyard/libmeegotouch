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

#include <QEvent>
#include <QGraphicsSceneContextMenuEvent>

#include "mobjectmenu.h"
#include "mscenewindow_p.h"
#include "maction.h"
#include "mscenemanager.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MObjectMenu)

MObjectMenu::MObjectMenu(MWidget *target)
    : MSceneWindow(new MSceneWindowPrivate, new MObjectMenuModel, MSceneWindow::ObjectMenu)
{
    if (target) {
        // go trough all actions in the target widget and add the the actions
        // which are associated to object menu to model.
        QList<QAction *> actionList = target->actions();
        MActionList list;
        const int actionListCount = actionList.count();
        for (int i = 0; i < actionListCount; ++i) {
            MAction *action = qobject_cast<MAction *>(actionList.at(i));
            if (action && (action->location() & MAction::ObjectMenuLocation)) {
                list.append(action);
            }
        }

        model()->setActions(list);

        QVariant v = target->property("contentURI");
        if(v.isValid())
            model()->setContentURI(v.toString());

        v = target->property("objectMenuTitle");
        if(v.isValid())
            model()->setTitle(v.toString());

        v = target->property("objectMenuIconId");
        if(v.isValid())
            model()->setIconId(v.toString());

        // install event filter to the target widget and object menu itself,
        // so we get notified when actions are added,removed or changed.
        installEventFilter(this);
        target->installEventFilter(this);
    }
    else {
        // When there's no target widget, install event filter into itself to
        // get notifications of added, removed or changed actions.
        installEventFilter(this);
    }
}

MObjectMenu::MObjectMenu(const QList<MAction *> &actionList)
    : MSceneWindow(new MSceneWindowPrivate, new MObjectMenuModel, MSceneWindow::ObjectMenu)
{
    installEventFilter(this);
    for (int i = 0; i < actionList.count(); ++i)
        addAction(actionList[i]);
}

MObjectMenu::~MObjectMenu()
{
}

bool MObjectMenu::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj);
    MAction *action = NULL;

    // if the event was related to object menu actions, we'll modify the model accordingly.
    switch (event->type()) {
    case QEvent::ActionAdded:
        if ((action = qobject_cast<MAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & MAction::ObjectMenuLocation)
                model()->addAction(action);
        }
        break;
    case QEvent::ActionChanged:
        if ((action = qobject_cast<MAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & MAction::ObjectMenuLocation)
                model()->modifyAction(action);
        }
        break;
    case QEvent::ActionRemoved:
        if ((action = qobject_cast<MAction *>(((QActionEvent *)event)->action()))) {
            if (action->location() & MAction::ObjectMenuLocation)
                model()->removeAction(action);
        }
        break;
    default:
        break;
    }
    return false;
}

void MObjectMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}

QPointF MObjectMenu::cursorPosition() const
{
    return model()->cursorPosition();
}

void MObjectMenu::setCursorPosition(const QPointF &pos)
{
    model()->setCursorPosition(pos);
}

QString MObjectMenu::title() const
{
    return model()->title();
}

void MObjectMenu::setTitle(const QString& title)
{
    model()->setTitle(title);
}

QString MObjectMenu::iconId() const
{
    return model()->iconId();
}

void MObjectMenu::setIconId(const QString& iconId)
{
    model()->setIconId(iconId);
}

