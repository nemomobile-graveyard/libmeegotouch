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

#include "ut_duiapplicationpage.h"

#include <DuiApplicationWindow>
#include <DuiApplication>
#include "duiapplicationpage_p.h"

#include <DuiPannableViewport>
#include <DuiAction>
#include <DuiButton>
#include <DuiLayout>
#include <DuiGridLayoutPolicy>

#include "duiondisplaychangeevent.h"

void Ut_DuiApplicationPage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duiapplicationpage" };
    app = new DuiApplication(argc, app_name);
    appWin = new DuiApplicationWindow;

    qRegisterMetaType<DuiApplicationPage *>();
    qRegisterMetaType<DuiEscapeButtonPanelModel::EscapeMode>();
}

void Ut_DuiApplicationPage::cleanupTestCase()
{
    delete appWin;
    delete app;
}

void Ut_DuiApplicationPage::init()
{
    m_subject = new DuiApplicationPage;
}

void Ut_DuiApplicationPage::cleanup()
{
    // important: testCentralWidget deletes m_subject on its own
    if (m_subject) {
        delete m_subject;
        m_subject = 0;
    }
}

void Ut_DuiApplicationPage::testInitialValues()
{
    QCOMPARE(m_subject->title(), QString());
    QCOMPARE(m_subject->isContentCreated(), false);
    QVERIFY(m_subject->centralWidget());

    QCOMPARE(m_subject->isPannableAreaInteractive(), true);
    QCOMPARE(m_subject->escapeButtonMode(), DuiEscapeButtonPanelModel::CloseMode);
    QCOMPARE(m_subject->rememberPosition(), true);
}

void Ut_DuiApplicationPage::testProperties()
{
    QString title("Title of the page");
    bool pannableAreaInteractive = true;
    Qt::Orientations pannableAreaDirection = Qt::Horizontal | Qt::Vertical;
    bool autoMarginsForComponents = true;
    DuiEscapeButtonPanelModel::EscapeMode escapeMode = DuiEscapeButtonPanelModel::BackMode;
    bool rememberPosition = false;

    m_subject->setTitle(title);
    QCOMPARE(m_subject->title(), title);
    m_subject->setPannableAreaInteractive(pannableAreaInteractive);
    QCOMPARE(m_subject->isPannableAreaInteractive(), pannableAreaInteractive);
    m_subject->setPannableAreaDirection(pannableAreaDirection);
    QCOMPARE(m_subject->pannableAreaDirection(), pannableAreaDirection);
    m_subject->setAutoMarginsForComponentsEnabled(autoMarginsForComponents);
    QCOMPARE(m_subject->autoMarginsForComponentsEnabled(), autoMarginsForComponents);
    m_subject->setEscapeButtonMode(escapeMode);
    QCOMPARE(m_subject->escapeButtonMode(), escapeMode);
    m_subject->setRememberPosition(rememberPosition);
    QCOMPARE(m_subject->rememberPosition(), rememberPosition);
}

void Ut_DuiApplicationPage::testCentralWidget()
{
    QPointer<DuiWidget> widget = new DuiWidget;
    m_subject->setCentralWidget(widget);
    QCOMPARE(m_subject->centralWidget(), widget.data());

    // remove the current central widget and verify that it has been deleted
    m_subject->setCentralWidget(0);
    QCOMPARE(m_subject->centralWidget(), (DuiWidget *) 0);
    QVERIFY(widget.isNull());

    widget = new DuiWidget;
    m_subject->setCentralWidget(widget);
    QCOMPARE(m_subject->centralWidget(), widget.data());

    // delete the page to see if the central widget is deleted
    delete m_subject;
    m_subject = 0;
    QVERIFY(widget.isNull());
}

void Ut_DuiApplicationPage::testCreateContent()
{
    QVERIFY(!m_subject->isContentCreated());
    m_subject->createContent();
    QVERIFY(m_subject->isContentCreated());
}

void Ut_DuiApplicationPage::testPageTitleChanged()
{
    QSignalSpy spy(m_subject, SIGNAL(pageTitleChanged(DuiApplicationPage *, QString)));
    QString title("Title!");

    m_subject->setTitle(m_subject->title());
    QCOMPARE(spy.count(), 0);
    m_subject->setTitle(title);
    QCOMPARE(spy.count(), 1);
    m_subject->setTitle(title);
    QCOMPARE(spy.count(), 1);
    m_subject->setTitle(QString());
    QCOMPARE(spy.count(), 2);

    QCOMPARE(spy.at(0).at(0).value<DuiApplicationPage *>(), m_subject);
    QCOMPARE(spy.at(0).at(1).toString(), title);

    QCOMPARE(spy.at(1).at(0).value<DuiApplicationPage *>(), m_subject);
    QCOMPARE(spy.at(1).at(1).toString(), QString());
}

void Ut_DuiApplicationPage::testEscapeButtonModeChanged()
{
    QSignalSpy spy(m_subject, SIGNAL(escapeButtonModeChanged(DuiEscapeButtonPanelModel::EscapeMode)));
    DuiEscapeButtonPanelModel::EscapeMode backMode = DuiEscapeButtonPanelModel::BackMode;
    DuiEscapeButtonPanelModel::EscapeMode closeMode = DuiEscapeButtonPanelModel::CloseMode;

    m_subject->setEscapeButtonMode(m_subject->escapeButtonMode());
    QCOMPARE(spy.count(), 0);
    m_subject->setEscapeButtonMode(backMode);
    QCOMPARE(spy.count(), 1);
    m_subject->setEscapeButtonMode(backMode);
    QCOMPARE(spy.count(), 1);
    m_subject->setEscapeButtonMode(closeMode);
    QCOMPARE(spy.count(), 2);

    QCOMPARE(spy.at(0).at(0).value<DuiEscapeButtonPanelModel::EscapeMode>(), backMode);
    QCOMPARE(spy.at(1).at(0).value<DuiEscapeButtonPanelModel::EscapeMode>(), closeMode);
}

void Ut_DuiApplicationPage::testRememberPosition()
{
    m_subject->setRememberPosition(true);
    m_subject->d_func()->pannableViewPort->setPosition(QPointF(0, 10));
    m_subject->appearNow();
    QCOMPARE(m_subject->d_func()->pannableViewPort->position() + QPointF(10, 10), QPointF(10, 20));
    QCOMPARE(m_subject->d_func()->pannableViewPort->physics()->position() + QPointF(10, 10),  QPointF(10, 20));
}

void Ut_DuiApplicationPage::testForgetPosition()
{
    m_subject->setRememberPosition(false);
    m_subject->d_func()->pannableViewPort->setPosition(QPointF(0, 10));
    m_subject->appearNow();
    QCOMPARE(m_subject->d_func()->pannableViewPort->position() + QPointF(10, 10), QPointF(10, 10));
    QCOMPARE(m_subject->d_func()->pannableViewPort->physics()->position() + QPointF(10, 10),  QPointF(10, 10));
}

void Ut_DuiApplicationPage::testActionUpdated()
{
    m_subject->clearActions();
    QSignalSpy spy(m_subject, SIGNAL(actionUpdated(QActionEvent *)));
    QCOMPARE(spy.count(), 0);

    m_subject->addAction(new DuiAction("test application page", m_subject));
    QCOMPARE(spy.count(), 1);

    m_subject->clearActions();
    QCOMPARE(spy.count(), 2);
}

void Ut_DuiApplicationPage::testDefaultComponentDisplayModes()
{
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::NavigationBar), DuiApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::HomeButton), DuiApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::EscapeButton), DuiApplicationPageModel::Show);
}

void Ut_DuiApplicationPage::testSettingComponentsDisplayModes()
{
    DuiApplicationPageModel *model = m_subject->model();

    m_subject->setComponentsDisplayMode(DuiApplicationPage::NavigationBar, DuiApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::NavigationBar), DuiApplicationPageModel::Hide);
    QCOMPARE(model->navigationBarDisplayMode(), DuiApplicationPageModel::Hide);

    m_subject->setComponentsDisplayMode(DuiApplicationPage::HomeButton, DuiApplicationPageModel::AutoHide);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::HomeButton), DuiApplicationPageModel::AutoHide);
    QCOMPARE(model->homeButtonDisplayMode(), DuiApplicationPageModel::AutoHide);
}

void Ut_DuiApplicationPage::testSettingMultipleComponentsDisplayModes()
{
    DuiApplicationPageModel *model = m_subject->model();

    m_subject->setComponentsDisplayMode(DuiApplicationPage::NavigationBar | DuiApplicationPage::HomeButton,
                                        DuiApplicationPageModel::Hide);

    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::NavigationBar), DuiApplicationPageModel::Hide);
    QCOMPARE(model->navigationBarDisplayMode(), DuiApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::HomeButton), DuiApplicationPageModel::Hide);
    QCOMPARE(model->homeButtonDisplayMode(), DuiApplicationPageModel::Hide);
}

void Ut_DuiApplicationPage::testSettingAllComponentsDisplayMode()
{
    m_subject->setComponentsDisplayMode(DuiApplicationPage::AllComponents, DuiApplicationPageModel::Hide);

    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::NavigationBar), DuiApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::HomeButton), DuiApplicationPageModel::Hide);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::EscapeButton), DuiApplicationPageModel::Hide);

    m_subject->setComponentsDisplayMode(DuiApplicationPage::AllComponents, DuiApplicationPageModel::Show);

    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::NavigationBar), DuiApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::HomeButton), DuiApplicationPageModel::Show);
    QCOMPARE(m_subject->componentDisplayMode(DuiApplicationPage::EscapeButton), DuiApplicationPageModel::Show);
}

QTEST_APPLESS_MAIN(Ut_DuiApplicationPage)
