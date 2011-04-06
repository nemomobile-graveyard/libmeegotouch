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
#include <MPannableViewport>
#include <MPositionIndicator>
#include <MScene>

#include <QWeakPointer>

#include "mondisplaychangeevent.h"

bool gMWindowIsOnDisplay = false;
bool MWindow::isOnDisplay() const
{
    return gMWindowIsOnDisplay;
}

DummyObject::DummyObject(QObject *parent)
    : QObject(parent)
{
    page = 0;

    onApplicationPageAppearingWasCalled = false;
    pageContentWasAlreadyCreatedInAppearing = false;

    onApplicationPageAppearedWasCalled = false;
    pageContentWasAlreadyCreatedInAppeared = false;
}

void DummyObject::setApplicationPage(MApplicationPage *page)
{
    Q_ASSERT(page != 0);
    Q_ASSERT(this->page == 0);

    this->page = page;
    connect(page, SIGNAL(appearing()), SLOT(onApplicationPageAppearing()));
    connect(page, SIGNAL(appeared()), SLOT(onApplicationPageAppeared()));
}

void DummyObject::onApplicationPageAppearing()
{
    Q_ASSERT(page);
    onApplicationPageAppearingWasCalled = true;
    pageContentWasAlreadyCreatedInAppearing = page->isContentCreated();
}

void DummyObject::onApplicationPageAppeared()
{
    Q_ASSERT(page);
    onApplicationPageAppearedWasCalled = true;
    pageContentWasAlreadyCreatedInAppeared = page->isContentCreated();
}

void Ut_MApplicationPage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationpage" };
    app = new MApplication(argc, app_name);

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
    appWin = new MApplicationWindow;
    m_subject = new MApplicationPage;
}

void Ut_MApplicationPage::cleanup()
{
    // important: testCentralWidget deletes m_subject on its own
    if (m_subject) {
        delete m_subject;
        m_subject = 0;
    }

    delete appWin;
    appWin = 0;
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
    bool isProgressIndicatorVisible = true;

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
    m_subject->setProgressIndicatorVisible(isProgressIndicatorVisible);
    QCOMPARE(m_subject->isProgressIndicatorVisible(), isProgressIndicatorVisible);
}

void Ut_MApplicationPage::testApplicationWindow()
{
    MApplicationWindow *appWin2 = new MApplicationWindow;

    QCOMPARE(m_subject->applicationWindow(), (MApplicationWindow*) 0);
    m_subject->appear();
    QCOMPARE(m_subject->applicationWindow(), appWin);
    m_subject->disappear();
    QCOMPARE(m_subject->applicationWindow(), appWin);
    m_subject->appear(appWin2);
    QCOMPARE(m_subject->applicationWindow(), appWin2);
    m_subject->disappear();

    appWin2->scene()->removeItem(m_subject);
    delete appWin2;
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

void Ut_MApplicationPage::testIsContentCreated()
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
    m_subject->view();
    m_subject->adjustSize();

    m_subject->pannableViewport()->setAutoRange(false);
    m_subject->pannableViewport()->setRange(QRectF(0,0,1000,1000));

    m_subject->setRememberPosition(true);

    m_subject->d_func()->pannableViewport->setPosition(QPointF(0, 10));
    emit m_subject->sceneWindowStateChanged(MSceneWindow::Disappeared, MSceneWindow::Appeared);
    emit m_subject->sceneWindowStateChanged(MSceneWindow::Appeared, MSceneWindow::Disappeared);

    QCOMPARE(m_subject->d_func()->pannableViewport->position() + QPointF(10, 10), QPointF(10, 20));
}

void Ut_MApplicationPage::testForgetPosition()
{
    m_subject->view();
    m_subject->adjustSize();

    m_subject->pannableViewport()->setAutoRange(false);
    m_subject->pannableViewport()->setRange(QRectF(0,0,1000,1000));

    m_subject->setRememberPosition(false);

    m_subject->d_func()->pannableViewport->setPosition(QPointF(0, 10));
    emit m_subject->sceneWindowStateChanged(MSceneWindow::Disappeared, MSceneWindow::Appeared);
    emit m_subject->sceneWindowStateChanged(MSceneWindow::Appeared, MSceneWindow::Disappeared);

    QCOMPARE(m_subject->d_func()->pannableViewport->position() + QPointF(10, 10), QPointF(10, 10));
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

    m_subject->appear(appWin);

    m_subject->setTitle(title);
    QCOMPARE(appWin->windowTitle(), title);
    m_subject->setTitle(title2);
    QCOMPARE(appWin->windowTitle(), title2);
    m_subject->setTitle(title3);
    QCOMPARE(appWin->windowTitle(), title3_longest);
    m_subject->setTitle(title4);
    QCOMPARE(appWin->windowTitle(), title4);
}

void Ut_MApplicationPage::testIfPositionIndicatorGeometryFollowsExposedRect()
{
    m_subject->appear(appWin);
    QCOMPARE(m_subject->pannableViewport()->positionIndicator()->geometry(), m_subject->exposedContentRect());
}

void Ut_MApplicationPage::testContentIsCreatedOnEnteringAppearingState()
{
    MApplicationPage *page = new MApplicationPage;

    bool original_gMWindowIsOnDisplay = gMWindowIsOnDisplay;
    gMWindowIsOnDisplay = true;

    DummyObject dummyObject;
    dummyObject.setApplicationPage(m_subject);

    appWin->show();

    // Simulate an animated appearance.

    QCOMPARE(m_subject->isContentCreated(), false);

    page->appear(appWin); // first page appears "now" at all times
    m_subject->appear(appWin);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(m_subject->isContentCreated(), true);
    QCOMPARE(dummyObject.onApplicationPageAppearingWasCalled, true);
    QCOMPARE(dummyObject.pageContentWasAlreadyCreatedInAppearing, true);

    // put it back to its original value
    gMWindowIsOnDisplay = original_gMWindowIsOnDisplay;
    delete page;
}

void Ut_MApplicationPage::testContentIsCreatedOnEnteringAppearedState()
{
    DummyObject dummyObject;
    dummyObject.setApplicationPage(m_subject);

    QCOMPARE(m_subject->isContentCreated(), false);

    appWin->sceneManager()->appearSceneWindowNow(m_subject);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(m_subject->isContentCreated(), true);
    QCOMPARE(dummyObject.onApplicationPageAppearedWasCalled, true);
    QCOMPARE(dummyObject.pageContentWasAlreadyCreatedInAppeared, true);
}

void Ut_MApplicationPage::testCustomNavigationBarContentSetterAndGetter()
{
    QGraphicsWidget *customNavigationBarContent = new QGraphicsWidget;
    QWeakPointer<QGraphicsWidget> customNavigationBarContentPointer = customNavigationBarContent;

    QVERIFY(m_subject->customNavigationBarContent() == 0);

    m_subject->setCustomNavigationBarContent(customNavigationBarContent);

    QVERIFY(m_subject->customNavigationBarContent() == customNavigationBarContent);

    m_subject->setCustomNavigationBarContent(0);

    QVERIFY(m_subject->customNavigationBarContent() == 0);

    // Page should have deleted the old widget when replacing it with the 0 (null) one.
    QVERIFY(customNavigationBarContentPointer.isNull());
}

void Ut_MApplicationPage::testCustomNavigationBarContentOwnershipOnPageDeletion() {
    QGraphicsWidget *customNavigationBarContent = new QGraphicsWidget;
    QWeakPointer<QGraphicsWidget> customNavigationBarContentPointer = customNavigationBarContent;

    m_subject->setCustomNavigationBarContent(customNavigationBarContent);

    delete m_subject;
    m_subject = 0;

    QVERIFY(customNavigationBarContentPointer.isNull());
}

void Ut_MApplicationPage::testCustomNavigationBarContentChangedSignal()
{
    QGraphicsWidget *customNavigationBarContent = new QGraphicsWidget;
    QGraphicsWidget *otherCustomNavigationBarContent = new QGraphicsWidget;
    QSignalSpy customNavigationBarContentChangedSignalSpy(m_subject, SIGNAL(customNavigationBarContentChanged()));

    m_subject->setCustomNavigationBarContent(customNavigationBarContent);

    QCOMPARE(customNavigationBarContentChangedSignalSpy.count(), 1);

    m_subject->setCustomNavigationBarContent(otherCustomNavigationBarContent);

    QCOMPARE(customNavigationBarContentChangedSignalSpy.count(), 2);
}

void Ut_MApplicationPage::testSurviveCentralWidgetDoubleDelete()
{
    QGraphicsWidget *newCentralWidget = new QGraphicsWidget(m_subject);
    QGraphicsWidget *anotherCentralWidget = new QGraphicsWidget(m_subject);
    m_subject->setCentralWidget(newCentralWidget);
    delete newCentralWidget;
    m_subject->setCentralWidget(anotherCentralWidget);
    QVERIFY(true);
}

QTEST_APPLESS_MAIN(Ut_MApplicationPage)
