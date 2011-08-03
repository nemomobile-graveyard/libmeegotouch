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

#include <QTest>
#include <mondisplaychangeevent.h>
#include <mpositionindicatorview.h>
#include <mpositionindicatorview_p.h>
#include <mpositionindicator.h>
#include <mscene.h>
#include <mtheme.h>
#include <mapplication.h>
#include <mwindow.h>

#include "ut_mpositionindicatorview.h"

Q_DECLARE_METATYPE(QAbstractAnimation::State);

MApplication *app;

void Ut_MPositionIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mpositionindicatorview" };
    app = new MApplication(argc, app_name);
}


void Ut_MPositionIndicatorView::cleanupTestCase()
{
    delete app;
}

void Ut_MPositionIndicatorView::init()
{
    m_controller = new MPositionIndicator(0);
    m_subject = new MPositionIndicatorView(m_controller);
    m_controller->setView(m_subject);
}

void Ut_MPositionIndicatorView::cleanup()
{
    delete m_controller; // ~mwidgetcontroller calls mwidgetview.destroy() which deletes view (ie m_subject)
    m_controller = 0;
    m_subject = 0;
}

void Ut_MPositionIndicatorView::testIndicatorDisappearsWhenItGetsOffDisplay_data()
{
    QTest::addColumn<QRectF>("range");
    QTest::newRow("Content fully visible") << QRectF();
    QTest::newRow("Content partly visible") << QRectF(0, 0, 100, 100);
}

void Ut_MPositionIndicatorView::testIndicatorDisappearsWhenItGetsOffDisplay()
{
    QFETCH(QRectF, range);
    m_controller->setRange(range);

    MOnDisplayChangeEvent displayExited(false, QRectF());
    qApp->sendEvent(m_controller, &displayExited);

    QVERIFY(!m_subject->d_func()->hideTimer->isActive());
    QCOMPARE(m_subject->d_func()->fadeAnimation->state(), QAbstractAnimation::Stopped);
    QCOMPARE(m_subject->contentOpacity(), qreal(0.0));
}

void Ut_MPositionIndicatorView::testIndicatorAppearsWhenItGetsOnDisplay_data()
{
    QTest::addColumn<QRectF>("range");
    QTest::addColumn<QAbstractAnimation::State>("expectedAnimationState");
    QTest::addColumn<bool>("isHideTimerActive");

    QTest::newRow("Content fully visible") << QRectF() << QAbstractAnimation::Stopped << false;
    QTest::newRow("Content partly visible") << QRectF(0, 0, 100, 100) << QAbstractAnimation::Running << true;
}

void Ut_MPositionIndicatorView::testIndicatorAppearsWhenItGetsOnDisplay()
{
    QFETCH(QRectF, range);
    QFETCH(QAbstractAnimation::State, expectedAnimationState);
    QFETCH(bool, isHideTimerActive);

    m_controller->setRange(range);

    MOnDisplayChangeEvent displayEntered(true, QRectF());
    qApp->sendEvent(m_controller, &displayEntered);

    const QPropertyAnimation *fadeAnimation = m_subject->d_func()->fadeAnimation;
    QCOMPARE(fadeAnimation->state(), expectedAnimationState);
    if (fadeAnimation->state() == QAbstractAnimation::Running) {
        QCOMPARE(fadeAnimation->endValue().toReal(), qreal(1.0));
    }

    QCOMPARE(m_subject->d_func()->hideTimer->isActive(), isHideTimerActive);
    QCOMPARE(m_subject->contentOpacity(), qreal(0.0));
}

void Ut_MPositionIndicatorView::testNoActivityInSwitcher_data()
{
    QTest::addColumn<QRectF>("range");
    QTest::newRow("Content fully visible") << QRectF();
    QTest::newRow("Content partly visible") << QRectF(0, 0, 100, 100);
}

void Ut_MPositionIndicatorView::testNoActivityInSwitcher()
{
    QFETCH(QRectF, range);
    m_controller->setRange(range);

    MWindow window;
    MScene *scene = new MScene;
    window.setScene(scene);
    window.show();

    scene->addItem(m_controller);
    window.showMinimized();
    QVERIFY(window.isInSwitcher());
    QVERIFY(m_controller->isOnDisplay());

    MOnDisplayChangeEvent displayExited(false, QRectF());
    qApp->sendEvent(m_controller, &displayExited);
    QVERIFY(!m_subject->d_func()->hideTimer->isActive());
    QCOMPARE(m_subject->d_func()->fadeAnimation->state(), QAbstractAnimation::Stopped);
    QCOMPARE(m_subject->contentOpacity(), qreal(0.0));

    MOnDisplayChangeEvent displayEntered(true, QRectF());
    qApp->sendEvent(m_controller, &displayEntered);
    QVERIFY(!m_subject->d_func()->hideTimer->isActive());
    QCOMPARE(m_subject->d_func()->fadeAnimation->state(), QAbstractAnimation::Stopped);
    QCOMPARE(m_subject->contentOpacity(), qreal(0.0));

    scene->removeItem(m_controller);

    delete scene;
}

QTEST_APPLESS_MAIN(Ut_MPositionIndicatorView)

