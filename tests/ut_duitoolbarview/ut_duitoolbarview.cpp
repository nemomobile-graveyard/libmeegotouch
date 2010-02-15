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

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiAction>
#include <DuiToolBar>
#include <DuiToolBarView>
#include <DuiTextEdit>
#include <DuiWidgetAction>
#include <QGraphicsLinearLayout>

#include "ut_duitoolbarview.h"

DuiApplication *app;
DuiApplicationWindow *appWin;

void Ut_DuiToolBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_DuiToolBarView" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;
}

void Ut_DuiToolBarView::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiToolBarView::init()
{
    m_toolbar = new DuiToolBar();
    QVERIFY(m_toolbar != 0);

    m_subject = new DuiToolBarView(m_toolbar);
    QVERIFY(m_subject != 0);

    m_toolbar->setView(m_subject);
    //m_subject->updateStyle();
}

void Ut_DuiToolBarView::cleanup()
{
    delete m_toolbar;
    m_toolbar = 0;
}

void Ut_DuiToolBarView::testDeleteAddAction()
{
    m_toolbar->clearActions();

    DuiWidget *parentWidget = new DuiWidget();
    m_toolbar->addAction(createTextEditAction(parentWidget));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);
    delete parentWidget;

    // toolbar should not crash as action has been deleted
    m_toolbar->addAction(new DuiAction("TEXTONLY", m_toolbar));
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents, 10);

    QVERIFY(m_toolbar->actions().count() == 1);
}

DuiWidgetAction *Ut_DuiToolBarView::createTextEditAction(DuiWidget *parentWidget)
{
    DuiTextEdit *textEntry = new DuiTextEdit();
    DuiWidgetAction *actionTextEdit = new DuiWidgetAction(parentWidget);
    actionTextEdit->setWidget(textEntry);
    return actionTextEdit;
}

QTEST_APPLESS_MAIN(Ut_DuiToolBarView)
