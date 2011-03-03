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

#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QString>
#include <QGraphicsSceneMouseEvent>
#include <QSignalSpy>

#include <views/mprogressindicatorbarview.h>
#include <views/mprogressindicatorbarview_p.h>

#include <mprogressindicator.h>
#include "ut_mprogressindicatorbarview.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mapplicationpage.h"

MApplication *app;
MApplicationWindow *win;
MApplicationPage *page;

void Ut_MProgressIndicatorBarView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttonview" };
    app = new MApplication(argc, app_name);
    win = new MApplicationWindow();
    page = new MApplicationPage();

    m_progressIndicator = new MProgressIndicator();
    m_subject = new MProgressIndicatorBarView(m_progressIndicator);
    m_progressIndicator->setView(m_subject);

    m_progressIndicator->resize(20, 20);
    page->setCentralWidget(m_progressIndicator);
    page->appear(win);
}

void Ut_MProgressIndicatorBarView::cleanupTestCase()
{
    delete win;
    delete app;
}

void Ut_MProgressIndicatorBarView::testSpinnerZeroRange()
{
    m_progressIndicator->setRange(0, 0);
    captureImage(m_progressIndicator);
}

QImage Ut_MProgressIndicatorBarView::captureImage(MProgressIndicator *progressIndicator)
{
    QPixmap pixmap(progressIndicator->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    progressIndicator->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

void Ut_MProgressIndicatorBarView::testThrottleAnimationWhenRenderedInSwitcher()
{
    m_progressIndicator->setUnknownDuration(true);

    // get animation duration
    int normalInterval = m_subject->d_func()->animationTimer->interval();

    // emulate the window entering the switcher
    m_subject->d_func()->switcherEntered();

    // get animation duration
    int slowInterval = m_subject->d_func()->animationTimer->interval();

    QVERIFY(normalInterval < slowInterval);
}

QTEST_APPLESS_MAIN(Ut_MProgressIndicatorBarView)
