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

#include "ut_mscenewindow.h"

#include <MDismissEvent>
#include <MScene>
#include <MSceneManager>
#include <MApplicationWindow>
#include <MWindow>
#include <MOverlay>
#include <MComponentData>

class MyMDismissEventFilter : public QObject
{
public:
    MyMDismissEventFilter() : QObject(0) {
        count = 0;
    }

    bool eventFilter(QObject *watched, QEvent *event) {
        Q_UNUSED(watched)
        if (event->type() == MDismissEvent::eventType()) {
            count++;
        }
        return false;
    }

    int count;
};

// Test class implementation

void Ut_MSceneWindow::init()
{
    m_subject = new MOverlay;
}

void Ut_MSceneWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSceneWindow::initTestCase()
{
    if(MComponentData::instance() == 0) {
        int argc = 1;
        char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = new MComponentData(argc, argv);
    }
    window = new MWindow(new MSceneManager);
}

void Ut_MSceneWindow::cleanupTestCase()
{
    delete window;
    window = 0;
}

void Ut_MSceneWindow::testAccessors()
{
    m_subject->setManagedManually(true);

    QCOMPARE(m_subject->windowType(), MSceneWindow::Overlay);
    QCOMPARE(m_subject->isManagedManually(), true);
}

void Ut_MSceneWindow::testAppear()
{
    m_subject->appear(window);

    QCOMPARE(m_subject->sceneManager(), window->sceneManager());
    QVERIFY(m_subject->scene() == static_cast<QGraphicsScene *>(window->scene()));
}

/*
 * Appearing on a MWindow without a scene manager should just work.
 * Scene manager should be created on the fly.
 */
void Ut_MSceneWindow::testAppearWithoutSceneManager()
{
    window->setSceneManager(0);

    m_subject->appear(window);

    QVERIFY(m_subject->sceneManager() == window->sceneManager());
    QVERIFY(m_subject->scene() == static_cast<QGraphicsScene *>(window->scene()));
}

void Ut_MSceneWindow::testAppearedDisappearedSignals()
{
    QSignalSpy spyAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyDisappeared(m_subject, SIGNAL(disappeared()));

    window->sceneManager()->appearSceneWindowNow(m_subject);

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(spyDisappeared.count(), 0);

    window->sceneManager()->disappearSceneWindowNow(m_subject);

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(spyDisappeared.count(), 1);
}

void Ut_MSceneWindow::testDismiss()
{
    MyMDismissEventFilter dismissEventFilter;
    m_subject->installEventFilter(&dismissEventFilter);

    window->sceneManager()->appearSceneWindowNow(m_subject);

    QSignalSpy spyWindowAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyWindowDisappeared(m_subject, SIGNAL(disappeared()));

    window->sceneManager()->dismissSceneWindowNow(m_subject);

    QCOMPARE(spyWindowAppeared.count(), 0);
    QCOMPARE(spyWindowDisappeared.count(), 1);
    // QCOMPARE(dismissEventFilter.count, 1); FIXME: Dismiss events are disabled in 0.18 due to ABI freeze
}

void Ut_MSceneWindow::testDestroyWhenDoneCallingDisappear()
{
    window->sceneManager()->appearSceneWindowNow(m_subject, MSceneWindow::DestroyWhenDone);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    window->sceneManager()->disappearSceneWindowNow(m_subject);

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_MSceneWindow::testDestroyWhenDoneCallingDismiss()
{
    window->sceneManager()->appearSceneWindowNow(m_subject, MSceneWindow::DestroyWhenDone);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    window->sceneManager()->dismissSceneWindowNow(m_subject);

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_MSceneWindow::testDestroyWhenDismissedCallingDisappear()
{
    window->sceneManager()->appearSceneWindowNow(m_subject, MSceneWindow::DestroyWhenDismissed);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    window->sceneManager()->disappearSceneWindowNow(m_subject);

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 0);
}

void Ut_MSceneWindow::testDestroyWhenDismissedCallingDismiss()
{
    window->sceneManager()->appearSceneWindowNow(m_subject, MSceneWindow::DestroyWhenDismissed);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    window->sceneManager()->dismissSceneWindowNow(m_subject);

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_MSceneWindow::testDismissedStateReset()
{
    window->sceneManager()->appearSceneWindowNow(m_subject);
    window->sceneManager()->dismissSceneWindowNow(m_subject);

    // internal "dismissed" state should be reset to false on next reappearance
    // Therefore the disappearance below should not cause self-destruction

    window->sceneManager()->appearSceneWindowNow(m_subject, MSceneWindow::DestroyWhenDismissed);
    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));
    window->sceneManager()->disappearSceneWindowNow(m_subject);

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 0);
}

void Ut_MSceneWindow::processPendingEvents()
{
    // Send the posted QEvent::DeferredDelete from deleteLater().
    QCoreApplication::sendPostedEvents();

    // Quit when there are no more pending events (like DeferredDelete)
    QTimer::singleShot(0, QCoreApplication::instance(), SLOT(quit()));

    QCoreApplication::instance()->exec();
}

QTEST_MAIN(Ut_MSceneWindow)
