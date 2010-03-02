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

#include "qtmaemo6submenu.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6style_p.h"

#include <QMenu>

QtMaemo6SubMenu::QtMaemo6SubMenu(QMenu *m, QWidget *parent) : QListWidget(parent)
{
    if(m)
        fillListWidget(m);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(listItemClicked(QListWidgetItem *)));
}

QtMaemo6SubMenu::~QtMaemo6SubMenu()
{

}

void QtMaemo6SubMenu::fillListWidget(QMenu *m)
{
    m_actionItemList.clear();
    foreach(QAction * action, m->actions()) {
        if (!action->isSeparator() && action->isEnabled()) {
            QListWidgetItem *item = new QListWidgetItem(action->icon(), action->text(), this);
            m_actionItemList.insert(item, action);
        }
    }
}

void QtMaemo6SubMenu::listItemClicked(QListWidgetItem *item)
{
    QAction *action = m_actionItemList.value(item);
    if (action) {
        if (action->menu()) {
            QtMaemo6SubMenu *subMenu = new QtMaemo6SubMenu(action->menu(), NULL);
            QtMaemo6WindowDecoration *decoration = new QtMaemo6WindowDecoration(subMenu, NULL);
            decoration->showFastMaximized();
            //these both must be done after the show, because the status- and
            // menubar is added on show event
            decoration->setStatusBar(NULL);
            decoration->setMenuBar(NULL);
            QtMaemo6StylePrivate::drawWindowBackground(decoration);
        } else {
            action->activate(QAction::Trigger);
        }
    }
    close();
}
