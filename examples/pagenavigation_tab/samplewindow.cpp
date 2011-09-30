/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "samplewindow.h"

#include <MLabel>
#include <MToolBar>

SampleWindow::SampleWindow(QWidget *parent) : MApplicationWindow(parent)
{

    // We want the toolbar to show actions as tabs. I.e., be a tab bar.
    setToolbarViewType(MToolBar::tabType);

    currentAction = 0;

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    QAction *alphaAction = createAction("Alpha", true);
    createAction("Beta");
    createAction("Gamma");
    createAction("Delta");

    connect(actionGroup, SIGNAL(triggered(QAction*)), SLOT(showPageForAction(QAction*)));

    showPageForAction(alphaAction);
}

void SampleWindow::showPageForAction(QAction *action)
{
    if (currentAction == action)
        return;

    MApplicationPage *page = createPage(action->text());
    page->appear(this, MSceneWindow::DestroyWhenDone);
    currentAction = action;
}

MApplicationPage *SampleWindow::createPage(const QString &name)
{
    MApplicationPage *page = new MApplicationPage;

    page->setTitle(name);

    QString contentText = QString("%1 Content").arg(name);
    page->setCentralWidget(new MLabel(contentText));

    page->setEscapeMode(MApplicationPageModel::EscapeCloseWindow);

    return page;
}

QAction *SampleWindow::createAction(const QString &name, bool checked)
{
    MAction *action = new MAction(name, this);
    action->setLocation(MAction::ToolBarLocation);
    action->setCheckable(true);
    action->setChecked(checked);
    action->setActionGroup(actionGroup);
    addAction(action);

    return action;
}
