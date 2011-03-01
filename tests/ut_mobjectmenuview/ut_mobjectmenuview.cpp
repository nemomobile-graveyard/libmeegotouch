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

#include <MApplication>
#include <MApplicationWindow>
#include <mobjectmenu.h>

#include "mobjectmenuview_p.h"
#include <MAction>
#include <MButton>
#include <MLabel>
#include <MImageWidget>
#include <MTheme>
#include <QPixmap>

#include "ut_mobjectmenuview.h"

namespace {
    const char *IconId = "icon-l-browser";
};

MApplication *app;

void Ut_MObjectMenuView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./Ut_MObjectMenuView" };
    app = new MApplication(argc, app_name);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_theme = MTheme::instance();
}

void Ut_MObjectMenuView::cleanupTestCase()
{
    delete app;
}

void Ut_MObjectMenuView::init()
{
    m_menu = new MObjectMenu(0);
    m_subject = new MObjectMenuView(m_menu);
}

void Ut_MObjectMenuView::cleanup()
{
    delete m_menu;
    m_menu = 0;
}

void Ut_MObjectMenuView::testSetView()
{
    QVERIFY(m_menu != 0);
    QVERIFY(m_subject != 0);

    m_menu->setView(m_subject);
}

void Ut_MObjectMenuView::testTitleAndIcon()
{
    m_menu->setView(m_subject);

    m_menu->setTitle("title");
    QCOMPARE(m_menu->title(), m_subject->d_func()->titleLabel->text());

    m_menu->setIconId(IconId);
    const QPixmap *pixmap = m_theme->pixmap(IconId, QSize(64,64));

    // wait for pending theme requests
    if (MTheme::hasPendingRequests()) {
        QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

        QEventLoop eventLoop;
        connect(m_theme, SIGNAL(pixmapRequestsFinished()), &eventLoop, SLOT(quit()));
        QTimer::singleShot(10000, &eventLoop, SLOT(quit())); // fallback if pixmapRequestsFinished() is not send
        eventLoop.exec();

        QCOMPARE(spy.count(), 1);
    }

    QCOMPARE(pixmap, m_subject->d_func()->titleIcon->pixmap());
    m_theme->releasePixmap(pixmap);
}

void Ut_MObjectMenuView::testActions()
{
    m_menu->setView(m_subject);

    MAction* menuAction = new MAction("menuAction",m_menu);
    menuAction->setLocation(MAction::ObjectMenuLocation);

    MAction* toolbarAction = new MAction("toolbarAction",m_menu);
    toolbarAction->setLocation(MAction::ToolBarLocation);

    MAction* everyLocationAction = new MAction("everyLocationAction",m_menu);
    everyLocationAction->setLocation(MAction::EveryLocation);

    m_menu->addAction(menuAction);
    QCOMPARE(m_subject->d_func()->buttons.size(), 1);
    QVERIFY(m_subject->d_func()->buttons.contains(menuAction));
    QCOMPARE(m_subject->d_func()->buttons.value(menuAction)->text(), menuAction->text());


    m_menu->addAction(toolbarAction); // this action should be ignored by menu
    QCOMPARE(m_subject->d_func()->buttons.size(), 1);
    QVERIFY(!m_subject->d_func()->buttons.contains(toolbarAction));


    m_menu->addAction(everyLocationAction);
    QCOMPARE(m_subject->d_func()->buttons.size(), 2);
    QVERIFY(m_subject->d_func()->buttons.contains(everyLocationAction));
    QCOMPARE(m_subject->d_func()->buttons.value(everyLocationAction)->text(), everyLocationAction->text());

    menuAction->setText("menuAction2");
    QCOMPARE(m_subject->d_func()->buttons.value(menuAction)->text(), menuAction->text());

    m_menu->removeAction(menuAction);
    QCOMPARE(m_subject->d_func()->buttons.size(), 1);
    QVERIFY(!m_subject->d_func()->buttons.contains(menuAction));

    m_menu->removeAction(toolbarAction);
    QCOMPARE(m_subject->d_func()->buttons.size(), 1);
    QVERIFY(!m_subject->d_func()->buttons.contains(menuAction));

    m_menu->removeAction(everyLocationAction);
    QCOMPARE(m_subject->d_func()->buttons.size(), 0);
    QVERIFY(!m_subject->d_func()->buttons.contains(everyLocationAction));
}


QTEST_APPLESS_MAIN(Ut_MObjectMenuView)

