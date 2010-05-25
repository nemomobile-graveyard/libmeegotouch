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

#include "qtmaemo6menu.h"
#include "qtmaemo6submenu.h"
#include "qtmaemo6windowdecoration.h"
#include "qtmaemo6style_p.h"

#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QGridLayout>
#include <QToolButton>

QtMaemo6Menu::QtMaemo6Menu(QMenuBar *mb, QWidget *parent) : QWidget(parent)
{
    setObjectName("Qt_Maemo6_Menu");
    QGridLayout *gridLayout = new QGridLayout(this);

    for (int i = 0; i < mb->actions().count(); ++i) {
        QAction *action = mb->actions().at(i);
        QToolButton *button = new QToolButton();
        button->setDefaultAction(action);
        if (action->menu())
            connect(button, SIGNAL(clicked()), this, SLOT(showSubMenu()));
        else
            connect(button, SIGNAL(clicked()), this, SLOT(executeAction()));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        gridLayout->addWidget(button, i / 2, i % 2);
    }
}

QtMaemo6Menu::~QtMaemo6Menu()
{

}

void QtMaemo6Menu::showSubMenu()
{
    if (QToolButton *button = qobject_cast<QToolButton *>(sender())) {
        if (QMenu *menu = button->defaultAction()->menu()) {
            //show menu
            QtMaemo6SubMenu *subMenu = new QtMaemo6SubMenu(menu, NULL);
            QtMaemo6WindowDecoration *decoration = new QtMaemo6WindowDecoration(subMenu, NULL);
            decoration->showFastMaximized();
            //these both must be done after the show, because the status- and
            // menubar is added on show event
            decoration->setStatusBar(NULL);
            decoration->setMenuBar(NULL);
            QtMaemo6StylePrivate::drawWindowBackground(decoration);
            close();
        }
    }
}

void QtMaemo6Menu::executeAction()
{
    if (QToolButton *button = qobject_cast<QToolButton *>(sender())) {
        button->defaultAction()->activate(QAction::Trigger);
        close();
    }
}

void QtMaemo6Menu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    QStyleOption option;
    option.initFrom(this);

    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
