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

#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include "mbutton.h"
#include "mescapebuttonpanelview.h"
#include "mescapebuttonpanelview_p.h"
#include "mescapebuttonpanel.h"
#include "ut_mescapebuttonpanelview.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mtheme.h"

MApplication *app;

void Ut_MEscapeButtonPanelView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mescapebuttonpanelview" };
    app = new MApplication(argc, app_name);
    MTheme::loadCSS(qApp->applicationDirPath() + "/ut_mescapebuttonpanelview.css");
}

void Ut_MEscapeButtonPanelView::cleanupTestCase()
{
    delete app;
}

void Ut_MEscapeButtonPanelView::init()
{
    m_buttonPanel = new MEscapeButtonPanel();
    m_subject = new MEscapeButtonPanelView(m_buttonPanel);
    m_buttonPanel->setView(m_subject);
}

void Ut_MEscapeButtonPanelView::cleanup()
{
    delete m_buttonPanel;
}

void Ut_MEscapeButtonPanelView::testHiddenPanelWithCloseButton()
{
    QSignalSpy spy(m_buttonPanel, SIGNAL(buttonClicked()));

    // set the style that uses a close button
    m_buttonPanel->setStyleName("closeButton");

    // set back mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    QVERIFY(m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->backButton->click();
    QCOMPARE(spy.count(), 1);

    // set close mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    QVERIFY(!m_subject->d_func()->backButton->isVisible());
    QVERIFY(m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->closeButton->click();
    QCOMPARE(spy.count(), 2);
}

void Ut_MEscapeButtonPanelView::testHiddenPanelWithoutCloseButton()
{
    QSignalSpy spy(m_buttonPanel, SIGNAL(buttonClicked()));

    // set the style that doesn't use a close button
    m_buttonPanel->setStyleName("noCloseButton");

    // set back mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    QVERIFY(m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->backButton->click();
    QCOMPARE(spy.count(), 1);

    // set close mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    QVERIFY(!m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());
}

void Ut_MEscapeButtonPanelView::testVisiblePanelWithCloseButton()
{
    QSignalSpy spy(m_buttonPanel, SIGNAL(buttonClicked()));

    // set the style that uses a close button
    m_buttonPanel->setStyleName("closeButton");

    MApplicationWindow win;
    win.show();
    m_buttonPanel->appear(&win, MSceneWindow::DestroyWhenDone);

    // set back mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    //wait for the animations to finish
    QTest::qWait(1000);
    // at the end of the animation the buttons should be properly arranged
    QVERIFY(m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->backButton->click();
    QCOMPARE(spy.count(), 1);

    // set close mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    //wait for the animations to finish
    QTest::qWait(1000);
    // at the end of the animation the buttons should be properly arranged
    QVERIFY(!m_subject->d_func()->backButton->isVisible());
    QVERIFY(m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->closeButton->click();
    QCOMPARE(spy.count(), 2);

    m_buttonPanel = 0;
}

void Ut_MEscapeButtonPanelView::testVisiblePanelWithoutCloseButton()
{
    QSignalSpy spy(m_buttonPanel, SIGNAL(buttonClicked()));

    // set the style that uses a close button
    m_buttonPanel->setStyleName("noCloseButton");

    MApplicationWindow win;
    win.show();
    m_buttonPanel->appear(&win, MSceneWindow::KeepWhenDone);

    // set back mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::BackMode);
    //wait for the animations to finish
    QTest::qWait(1000);
    // at the end of the animation the buttons should be properly arranged
    QVERIFY(m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());
    // click on button and ensure that the signal in the controller gets emitted
    m_subject->d_func()->backButton->click();
    QCOMPARE(spy.count(), 1);

    // set close mode
    m_buttonPanel->setEscapeMode(MEscapeButtonPanelModel::CloseMode);
    //wait for the animations to finish
    QTest::qWait(1000);
    // at the end of the animation the buttons should be properly arranged
    QVERIFY(!m_subject->d_func()->backButton->isVisible());
    QVERIFY(!m_subject->d_func()->closeButton->isVisible());

    m_buttonPanel = 0;
}


QTEST_APPLESS_MAIN(Ut_MEscapeButtonPanelView)
