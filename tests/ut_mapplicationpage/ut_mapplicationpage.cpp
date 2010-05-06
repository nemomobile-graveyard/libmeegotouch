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

#include "ut_mapplicationpage.h"

#include <MApplicationWindow>
#include <MApplication>
#include "mapplicationpage_p.h"
#include <MSceneManager>

#include <MPannableViewport>
#include <MAction>
#include <MButton>
#include <MLayout>
#include <MGridLayoutPolicy>

#include "mondisplaychangeevent.h"

void Ut_MApplicationPage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationpage" };
    app = new MApplication(argc, app_name);
    appWin = new MApplicationWindow;

    qRegisterMetaType<MApplicationPage *>();
    qRegisterMetaType<MEscapeButtonPanelModel::EscapeMode>();
}

void Ut_MApplicationPage::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_MApplicationPage::init()
{
    m_subject = new MApplicationPage;
}

void Ut_MApplicationPage::cleanup()
{
    // important: testCentralWidget deletes m_subject on its own
    if (m_subject) {
        delete m_subject;
        m_subject = 0;
    }
}

void Ut_MApplicationPage::testInitialValues()
{
    QCOMPARE(m_subject->title(), QString());
    QCOMPARE(m_subject->isContentCreated(), false);
    QVERIFY(m_subject->centralWidget());

    QCOMPARE(m_subject->isPannable(), true);
    QCOMPARE(m_subject->escapeMode(), MApplicationPageModel::EscapeAuto);
    QCOMPARE(m_subject->rememberPosition(), true);
}

void Ut_MApplicationPage::testProperties()
{
    QString title("Title of the page");
    bool pannable = true;
    Qt::Orientations panningDirection = Qt::Horizontal | Qt::Vertical;
    bool autoMarginsForComponents = true;
    MApplicationPageModel::PageEscapeMode escapeMode = MApplicationPageModel::EscapeManualBack;
    bool rememberPosition = false;

    m_subject->setTitle(title);
    QCOMPARE(m_subject->title(), title);
    m_subject->setPannable(pannable);
    QCOMPARE(m_subject->isPannable(), pannable);
    m_subject->setPanningDirection(panningDirection);
    QCOMPARE(m_subject->panningDirection(), panningDirection);
    m_subject->setAutoMarginsForComponentsEnabled(autoMarginsForComponents);
    QCOMPARE(m_subject->autoMarginsForComponentsEnabled(), autoMarginsForComponents);
    m_subject->setEscapeMode(escapeMode);
    QCOMPARE(m_subject->escapeMode(), escapeMode);
    m_subject->setRememberPosition(rememberPosition);
    QCOMPARE(m_subject->rememberPosition(), rememberPosition);
}

void Ut_MApplicationPage::testCentralWidget()
{
    QPointer<MWidget> widget = new MWidget;
    m_subject->setCentralWidget(widget);
    QCOMPARE(m_subject->centralWidget(), widget.data());

    // remove the current central widget and verify that it has been deleted
    m_subject->setCentralWidget(0);
    QCOMPARE(m_subject->centralWidget(), (MWidget *) 0);
    QVERIFY(widget.isNull());

    widget = new MWidget;
    m_subject->setCentralWidget(widget);
    QCOMPARE(m_subject->centralWidget(), widget.data());

    // delete the page to see if the central widget is deleted
    delete m_subject;
    m_subject = 0;
    QVERIFY(widget.isNull());
}

void Ut_MApplicationPage::testCreateContent()
{
    QVERIFY(!m_subject->isContentCreated());
    m_subject->createContent();
    QVERIFY(m_subject->isContentCreated());
}

void Ut_MApplicationPage::testPageTitleChanged()
{
    qRegisterMetaType< QList<const char *> >("QList<const char *>");

    QSignalSpy spy(m_subject->model(), SIGNAL(modified(QList<const char *>)));
    QString title("Title!");

    m_subject->setTitle(m_subject->title());
    QCOMPARE(spy.count(), 0);
    m_subject->setTitle(title);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(m_subject->model()->title(), title);
    m_subject->setTitle(title);
    QCOMPARE(spy.count(), 1);
    m_subject->setTitle(QString());
    QCOMPARE(spy.count(), 2);
    QCOMPARE(m_subject->model()->title(), QString());
}

void Ut_MApplicationPage::testRememberPosition()
{
    m_subject->setRememberPosition(true);
    m_subject->d_func()->pannableViewPort->setPosition(QPointF(0, 10));
    appWin->sceneManager()->appearSceneWindowNow(m_subject);
    QCOMPARE(m_subject->d_func()->pannableViewPort->position() + QPointF(10, 10), QPointF(10, 20));
    QCOMPARE(m_subject->d_func()->pannableViewPort->physics()->position() + QPointF(10, 10),  QPointF(10, 20));
}

void Ut_MApplicationPage::testForgetPosition()
{
    m_subject->setRememberPosition(false);
    m_subject->d_func()->pannableViewPort->setPosition(QPointF(0, 10));
    appWin->sceneManager()->appearSceneWindowNow(m_subject);
    QCOMPARE(m_subject->d_func()->pannableViewPort->position() + QPointF(10, 10), QPointF(10, 10));
    QCOMPARE(m_subject->d_func()->pannableViewPort->physics()->position() + QPointF(10, 10),  QPointF(10, 10));
}

void Ut_MApplicationPage::testActionUpdated()
{
    m_subject->clearActions();
    QSignalSpy spy(m_subject, SIGNAL(actionUpdated(QActionEvent *)));
    QCOMPARE(spy.count(), 0);

    m_subject->addAction(new MAction("test application page", m_subject));
    QCOMPARE(spy.count(), 1);

    m_subject->clearActions();
    QCOMPARE(spy.count(), 2);
}

void Ut_MApplicationPage::testDefaultComponentDisplayModes()
{
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::NavigationBar), MApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::HomeButton), MApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::EscapeButton), MApplicationPageModel::Show);
}

void Ut_MApplicationPage::testSettingComponentsDisplayModes()
{
    MApplicationPageModel *model = m_subject->model();

    m_subject->setComponentsDisplayMode(MApplicationPage::NavigationBar, MApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::NavigationBar), MApplicationPageModel::Hide);
    QCOMPARE(model->navigationBarDisplayMode(), MApplicationPageModel::Hide);

    m_subject->setComponentsDisplayMode(MApplicationPage::HomeButton, MApplicationPageModel::AutoHide);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::HomeButton), MApplicationPageModel::AutoHide);
    QCOMPARE(model->homeButtonDisplayMode(), MApplicationPageModel::AutoHide);
}

void Ut_MApplicationPage::testSettingMultipleComponentsDisplayModes()
{
    MApplicationPageModel *model = m_subject->model();

    m_subject->setComponentsDisplayMode(MApplicationPage::NavigationBar | MApplicationPage::HomeButton,
                                        MApplicationPageModel::Hide);

    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::NavigationBar), MApplicationPageModel::Hide);
    QCOMPARE(model->navigationBarDisplayMode(), MApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::HomeButton), MApplicationPageModel::Hide);
    QCOMPARE(model->homeButtonDisplayMode(), MApplicationPageModel::Hide);
}

void Ut_MApplicationPage::testSettingAllComponentsDisplayMode()
{
    m_subject->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Hide);

    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::NavigationBar), MApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::HomeButton), MApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::EscapeButton), MApplicationPageModel::Hide);

    m_subject->setComponentsDisplayMode(MApplicationPage::AllComponents, MApplicationPageModel::Show);

    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::NavigationBar), MApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::HomeButton), MApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(MApplicationPage::EscapeButton), MApplicationPageModel::Show);
}

void Ut_MApplicationPage::testUpdatingWindowTitleWithChangingPageTitle()
{
    QString title("Test title");
    QString title2("Another test title");
    QString title3("Multiple length variants title\0x9cMult. length var. title");
    QString title3_longest("Multiple length variants title");
    QString title4;

    m_subject->appear();

    m_subject->setTitle(title);
    QCOMPARE(appWin->windowTitle(), title);
    m_subject->setTitle(title2);
    QCOMPARE(appWin->windowTitle(), title2);
    m_subject->setTitle(title3);
    QCOMPARE(appWin->windowTitle(), title3_longest);
    m_subject->setTitle(title4);
    QCOMPARE(appWin->windowTitle(), title4);
}

QTEST_APPLESS_MAIN(Ut_MApplicationPage)
