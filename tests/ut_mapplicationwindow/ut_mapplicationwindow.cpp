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

#include "ut_mapplicationwindow.h"

#include <mapplication.h>
#include <mapplication_p.h>
#include <mapplicationservice.h>
#include <mapplicationwindow.h>
#include <mapplicationpage.h>
#include <MComponentData>
#include <MScene>
#include <MSceneManager>
#include <MEscapeButtonPanel>

#include <QSignalSpy>
#include <QEvent>

// Test class implementation

void Ut_MApplicationWindow::initTestCase()
{
    if(MComponentData::instance() == 0) {
        static int argc = 1;
        static char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mscenewindow";
        m_componentData = new MComponentData(argc, argv);
    }
    qRegisterMetaType<MApplicationPage *>("MApplicationPage*");
}

void Ut_MApplicationWindow::cleanupTestCase()
{
}

void Ut_MApplicationWindow::init()
{
    m_subject = new MApplicationWindow;
}

void Ut_MApplicationWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MApplicationWindow::testConstructorWithoutScene()
{
    QVERIFY(m_subject->scene());
}

void Ut_MApplicationWindow::testConstructorWithScene()
{
    MScene *scene = new MScene;
    MApplicationWindow *window = new MApplicationWindow(scene);
    QCOMPARE((quintptr) window->scene(), (quintptr) scene);
    delete window;
}

void Ut_MApplicationWindow::testIsOnDisplay()
{
    m_subject = MApplication::activeApplicationWindow();

    QPointer<MApplicationPage> page1 = new MApplicationPage;
    QPointer<MApplicationPage> page2 = new MApplicationPage;

    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // Window is not visible => page should not become visible
    m_subject->sceneManager()->appearSceneWindowNow(page2);
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // To be revisited: how these could be unit-tested
    // (event->viewRect().intersects(sceneBoundingRect())
    // does not return meaningful values in this.

    page1->disappear();
    page1->disappear();
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);
}

void Ut_MApplicationWindow::testPrestartNoPrestart()
{
    MApplication::setPrestartMode(M::NoPrestart);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_MApplicationWindow::testPrestartTerminateOnClose()
{
    MApplication::setPrestartMode(M::TerminateOnClose);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_MApplicationWindow::testPrestartLazyShutdown()
{
    MApplication::setPrestartMode(M::LazyShutdown);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
    QCOMPARE(m_subject->isHidden(), true);
}

void Ut_MApplicationWindow::testWindowActivate()
{
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);

    MApplicationWindow *appWin = new MApplicationWindow;
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);

    QEvent activate(QEvent::WindowActivate);

    qApp->sendEvent(appWin, &activate);
    QVERIFY(MApplication::activeApplicationWindow() == appWin);

    delete appWin;
    QVERIFY(MApplication::activeApplicationWindow() == m_subject);
}

void Ut_MApplicationWindow::testPageChanged()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(MApplicationPage *)));
    MApplicationPage *page = new MApplicationPage;
    MApplicationPage *page2 = new MApplicationPage;
    m_subject->sceneManager()->appearSceneWindowNow(page);
    m_subject->sceneManager()->appearSceneWindowNow(page2);

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<MApplicationPage *>(), page);
    QCOMPARE(spy.at(1).at(0).value<MApplicationPage *>(), page2);

    delete page;
    delete page2;
}

void Ut_MApplicationWindow::testDeleteOnClose()
{
    qDebug() << "if the test crashes here, "
             << "Qt::WA_DeleteOnClose is broken for MApplicationWindow";
    MApplicationWindow *win = new MApplicationWindow;
    win->setAttribute(Qt::WA_DeleteOnClose, true);
    win->show();
    win->close();
}

void Ut_MApplicationWindow::testDisplayExitedOnClose()
{
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MApplicationWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    MApplication::setPrestartMode(M::LazyShutdown);
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_MApplicationWindow::testPageEscapeAuto()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());

    QVERIFY(escapeButtonPanel != 0);

    m_subject->sceneManager()->appearSceneWindowNow(firstPage);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::CloseMode);

    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::BackMode);

    m_subject->sceneManager()->dismissSceneWindowNow(secondPage);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::CloseMode);
}

void Ut_MApplicationWindow::testPageEscapeAutoWhenAddingPageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());
    QList<MSceneWindow *> pageHistory;

    QVERIFY(escapeButtonPanel != 0);

    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::CloseMode);

    pageHistory.append(firstPage);
    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::BackMode);
}

void Ut_MApplicationWindow::testPageEscapeAutoWhenClearingPageHistory()
{
    MApplicationPage *firstPage = new MApplicationPage;
    MApplicationPage *secondPage = new MApplicationPage;
    MEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());
    QList<MSceneWindow *> pageHistory;

    QVERIFY(escapeButtonPanel != 0);

    m_subject->sceneManager()->appearSceneWindowNow(firstPage);
    m_subject->sceneManager()->appearSceneWindowNow(secondPage);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::BackMode);

    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(escapeButtonPanel->escapeMode(), MEscapeButtonPanelModel::CloseMode);
}

MEscapeButtonPanel *Ut_MApplicationWindow::fetchEscapeButtonPanel(
        const QList<QGraphicsItem *> &itemsList) const
{
    int i = 0;
    int itemsCount = itemsList.count();
    QGraphicsItem *item;
    QGraphicsWidget *widget;
    MEscapeButtonPanel *escapeButtonPanel = 0;

    while (escapeButtonPanel == 0 && i < itemsCount) {
        item = itemsList.at(i);
        if (item->isWidget()) {
            widget = static_cast<QGraphicsWidget *>(item);
            escapeButtonPanel = qobject_cast<MEscapeButtonPanel*>(widget);
        }

        ++i;
    }

    return escapeButtonPanel;
}

QTEST_MAIN(Ut_MApplicationWindow)
