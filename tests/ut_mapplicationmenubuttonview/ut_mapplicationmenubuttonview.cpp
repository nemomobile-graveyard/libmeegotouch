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
#include "ut_mapplicationmenubuttonview.h"
#include <QSignalSpy>
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mapplicationmenubutton.h"
#include "mapplicationmenubuttonview.h"
#include "mapplicationmenubuttonview_p.h"
#include "mprogressindicator.h"
#include "mimagewidget.h"
#include "mapplicationpage.h"

MApplication *app;
MApplicationWindow *win;
MApplicationPage *page;

void Ut_MApplicationMenuButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mapplicationmenubuttonview" };
    app = new MApplication(argc, app_name);
    win = new MApplicationWindow();
    page = new MApplicationPage();
    page->appear(win);
    win->show();
}

void Ut_MApplicationMenuButtonView::cleanupTestCase()
{
    delete win;
    delete app;
}

void Ut_MApplicationMenuButtonView::init()
{
    m_button = new MApplicationMenuButton();
    m_subject = new MApplicationMenuButtonView(m_button);
    m_button->setView(m_subject);
    page->setCentralWidget(m_button);
}

void Ut_MApplicationMenuButtonView::cleanup()
{
    page->setCentralWidget(0);
    m_button = 0;
    m_subject = 0;
}

void Ut_MApplicationMenuButtonView::testProgressIndicator()
{
    m_button->setProgressIndicatorVisible(true);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->spinner->isVisible(), true);

    m_button->setProgressIndicatorVisible(false);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->spinner->isVisible(), false);
}

void Ut_MApplicationMenuButtonView::testArrowIcon()
{
    m_button->setArrowIconVisible(true);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->arrowIconImage->isVisible(), true);

    m_button->setArrowIconVisible(false);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->arrowIconImage->isVisible(), false);

    // if both progress indicator and arrow icon are set as visible
    // only the progress indicator will be shown
    m_button->setProgressIndicatorVisible(true);
    m_button->setArrowIconVisible(true);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->spinner->isVisible(), true);
    QCOMPARE(m_subject->d_func()->arrowIconImage->isVisible(), false);
}

void Ut_MApplicationMenuButtonView::testIcon()
{
    // when the icon id is empty, setting it visible or invisible
    // doesn nothing
    m_button->setIconID("");

    m_button->setIconVisible(false);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->iconImage->isVisible(), false);

    m_button->setIconVisible(true);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->iconImage->isVisible(), false);

    // when an icon id is set, then setting the image visible or invisible
    // works
    m_button->setIconID("icon-l-music");

    m_button->setIconVisible(false);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->iconImage->isVisible(), false);

    m_button->setIconVisible(true);
    QCoreApplication::processEvents();
    QCOMPARE(m_subject->d_func()->iconImage->isVisible(), true);
}
QTEST_APPLESS_MAIN(Ut_MApplicationMenuButtonView)
