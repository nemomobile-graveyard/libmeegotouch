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

#include <views/mspinnerview.h>
#include <views/mspinnerview_p.h>

#include <mprogressindicator.h>
#include "ut_mprogressindicatorspinnerview.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mapplicationpage.h"

MApplication *app;
MApplicationWindow *win;
MApplicationPage *page;

void Ut_MProgressIndicatorSpinnerView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mbuttonview" };
    app = new MApplication(argc, app_name);
    win = new MApplicationWindow();
    page = new MApplicationPage();

    m_progressIndicator = new MProgressIndicator();
    m_subject = new MSpinnerView(m_progressIndicator);
    m_progressIndicator->setView(m_subject);

    m_progressIndicator->resize(20, 20);

    page->setCentralWidget(m_progressIndicator);
    page->appear(win);
}

void Ut_MProgressIndicatorSpinnerView::cleanupTestCase()
{
    delete win;
    delete app;
}

void Ut_MProgressIndicatorSpinnerView::testSpinnerZeroRange()
{
    m_progressIndicator->setRange(0, 0);
    captureImage(m_progressIndicator);
}

QImage Ut_MProgressIndicatorSpinnerView::captureImage(MProgressIndicator *progressIndicator)
{
    QPixmap pixmap(progressIndicator->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    progressIndicator->paint(&painter, NULL, NULL);

    return pixmap.toImage();
}

void Ut_MProgressIndicatorSpinnerView::testThrottleAnimationWhenRenderedInSwitcher()
{
    // get animation duration
    int normalDuration = m_subject->d_func()->positionAnimation->duration();

    // emulate entering the switcher
    m_subject->d_func()->_q_switcherEntered();

    // get animation duration
    int slowDuration = m_subject->d_func()->positionAnimation->duration();

    QVERIFY(normalDuration < slowDuration);
}

QTEST_APPLESS_MAIN(Ut_MProgressIndicatorSpinnerView)
