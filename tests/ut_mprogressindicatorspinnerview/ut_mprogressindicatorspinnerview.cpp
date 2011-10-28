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
#include <mondisplaychangeevent.h>

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
    m_progressIndicator->setUnknownDuration(true);

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

void Ut_MProgressIndicatorSpinnerView::testSpinnerInSwitcher()
{
    MOnDisplayChangeEvent onDisplayEvent(true,
                                         QRectF(QPointF(0,0),
                                         win->visibleSceneSize()));

    MOnDisplayChangeEvent offDisplayEvent(false,
                                         QRectF(QPointF(0,0),
                                         win->visibleSceneSize()));

    win->show();
    qApp->sendEvent(win, &onDisplayEvent);

    QVERIFY(m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Running);

    // emulate entering the switcher
    win->showMinimized();

    // the animation should be paused when we are in switcher
    QVERIFY(m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Paused ||
            m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Stopped);

    // emulate display exited when on switcher (should have no effect)
    qApp->sendEvent(win, &offDisplayEvent);
    QVERIFY(m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Paused ||
            m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Stopped);

    // emulate display entered when on switcher (should have no effect)
    qApp->sendEvent(win, &onDisplayEvent);
    QVERIFY(m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Paused ||
            m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Stopped);
    
    // emulate leaving the switcher
    win->showNormal();

    QVERIFY(m_subject->d_func()->positionAnimation->state() == QPropertyAnimation::Running);
}

void Ut_MProgressIndicatorSpinnerView::testAnimationState()
{
    MProgressIndicator* pi = new MProgressIndicator();
    MSpinnerView* view = new MSpinnerView(pi);
    pi->setUnknownDuration(true);
    pi->setVisible(false);
    pi->setView(view);
    // Regression test for NB#249480 - MProgressIndicator with unknown duration keeps spinning when hidden
    QVERIFY(view->d_func()->positionAnimation->state() == QPropertyAnimation::Paused ||
            view->d_func()->positionAnimation->state() == QPropertyAnimation::Stopped);

    // Emulate displayEntered (it resumes animation)
    view->d_func()->_q_resumeAnimation();
    QCOMPARE(view->d_func()->positionAnimation->state(), QPropertyAnimation::Running);
    // Emulate displayExited (it resumes animation)
    view->d_func()->_q_pauseAnimation();
    QCOMPARE(view->d_func()->positionAnimation->state(), QPropertyAnimation::Paused);
    delete pi;
}

QTEST_APPLESS_MAIN(Ut_MProgressIndicatorSpinnerView)
