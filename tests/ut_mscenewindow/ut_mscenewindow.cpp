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
    m_subject = new MSceneWindow;
}

void Ut_MSceneWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MSceneWindow::initTestCase()
{
    if(MComponentData::instance() == 0) {
        static int argc = 1;
        static char *argv[ 1 ];
        argv[ 0 ] = (char*)"./ut_mwindow";
        m_componentData = MComponentData::createInstance(argc, argv);
    }
    window = new MWindow(new MSceneManager);

    qRegisterMetaType<MSceneWindow::SceneWindowState>("MSceneWindow::SceneWindowState");
}

void Ut_MSceneWindow::cleanupTestCase()
{
    delete window;
    window = 0;
}

void Ut_MSceneWindow::testAccessors()
{
    m_subject->setManagedManually(true);

    QCOMPARE(m_subject->windowType(), MSceneWindow::PlainSceneWindow);
    QCOMPARE(m_subject->isManagedManually(), true);
}

void Ut_MSceneWindow::testAppearOnWindow()
{
    m_subject->appear(window);

    QCOMPARE(m_subject->sceneManager(), window->sceneManager());
    QVERIFY(m_subject->scene() == static_cast<QGraphicsScene *>(window->scene()));
}

void Ut_MSceneWindow::testAppearOnScene()
{
    m_subject->appear(window->scene());

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

void Ut_MSceneWindow::testSceneWindowStateChanges()
{
    QSignalSpy spyDisappeared(m_subject, SIGNAL(disappeared()));
    QSignalSpy spyAppearing(m_subject, SIGNAL(appearing()));
    QSignalSpy spyAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyDisappearing(m_subject, SIGNAL(disappearing()));
    QSignalSpy spySceneWindowStateChanged(m_subject,
            SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState,
                    MSceneWindow::SceneWindowState)));
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(m_subject);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Disappeared);

    testBridge.setSceneWindowState(MSceneWindow::Appearing);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Appearing);
    QCOMPARE(spyDisappeared.count(), 0);
    QCOMPARE(spyAppearing.count(), 1);
    QCOMPARE(spyAppeared.count(), 0);
    QCOMPARE(spyDisappearing.count(), 0);
    QVERIFY(verifySceneWindowStateChange(spySceneWindowStateChanged,
                                         MSceneWindow::Appearing,
                                         MSceneWindow::Disappeared));
    spyAppearing.clear();

    testBridge.setSceneWindowState(MSceneWindow::Appeared);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Appeared);
    QCOMPARE(spyDisappeared.count(), 0);
    QCOMPARE(spyAppearing.count(), 0);
    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(spyDisappearing.count(), 0);
    QVERIFY(verifySceneWindowStateChange(spySceneWindowStateChanged,
                                         MSceneWindow::Appeared,
                                         MSceneWindow::Appearing));
    spyAppeared.clear();

    testBridge.setSceneWindowState(MSceneWindow::Disappearing);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Disappearing);
    QCOMPARE(spyDisappeared.count(), 0);
    QCOMPARE(spyAppearing.count(), 0);
    QCOMPARE(spyAppeared.count(), 0);
    QCOMPARE(spyDisappearing.count(), 1);
    QVERIFY(verifySceneWindowStateChange(spySceneWindowStateChanged,
                                         MSceneWindow::Disappearing,
                                         MSceneWindow::Appeared));
    spyDisappearing.clear();

    testBridge.setSceneWindowState(MSceneWindow::Disappeared);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(spyDisappeared.count(), 1);
    QCOMPARE(spyAppearing.count(), 0);
    QCOMPARE(spyAppeared.count(), 0);
    QCOMPARE(spyDisappearing.count(), 0);
    QVERIFY(verifySceneWindowStateChange(spySceneWindowStateChanged,
                                         MSceneWindow::Disappeared,
                                         MSceneWindow::Disappearing));
    spyDisappeared.clear();
}

void Ut_MSceneWindow::testSettingSameSceneWindowState()
{
    QSignalSpy spyDisappeared(m_subject, SIGNAL(disappeared()));
    QSignalSpy spyAppearing(m_subject, SIGNAL(appearing()));
    QSignalSpy spyAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyDisappearing(m_subject, SIGNAL(disappearing()));
    QSignalSpy spySceneWindowStateChanged(m_subject,
            SIGNAL(sceneWindowStateChanged(MSceneWindow::SceneWindowState,
                    MSceneWindow::SceneWindowState)));
    TestBridge testBridge;

    testBridge.setObjectName("_m_testBridge");
    testBridge.setParent(m_subject);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Disappeared);

    testBridge.setSceneWindowState(MSceneWindow::Disappeared);

    QCOMPARE(m_subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(spyDisappeared.count(), 0);
    QCOMPARE(spyAppearing.count(), 0);
    QCOMPARE(spyAppeared.count(), 0);
    QCOMPARE(spyDisappearing.count(), 0);
    QCOMPARE(spySceneWindowStateChanged.count(), 0);

}

void Ut_MSceneWindow::testFocusHandlingOnWindowBlocking()
{
    window->show();
    QTest::qWaitForWindowShown(window);

    m_subject->appear(window->scene());
    QGraphicsWidget *widget = new QGraphicsWidget(m_subject);
    widget->setFlag(QGraphicsItem::ItemIsFocusable);
    widget->setFocus();

    if (!window->scene()->isActive())
        QSKIP("The scene is not active, focus will not be set...", SkipAll);

    QVERIFY(widget->hasFocus());

    qApp->sendEvent(m_subject, new QEvent(QEvent::WindowBlocked));

    QVERIFY(!widget->hasFocus());

    qApp->sendEvent(m_subject, new QEvent(QEvent::WindowUnblocked));

    QVERIFY(widget->hasFocus());

    window->hide();
    qApp->processEvents();
}

void Ut_MSceneWindow::processPendingEvents()
{
    // Send the posted QEvent::DeferredDelete from deleteLater().
    QCoreApplication::sendPostedEvents();

    // Quit when there are no more pending events (like DeferredDelete)
    QTimer::singleShot(0, QCoreApplication::instance(), SLOT(quit()));

    QCoreApplication::instance()->exec();
}

bool Ut_MSceneWindow::verifySceneWindowStateChange(QSignalSpy &spy,
        MSceneWindow::SceneWindowState expectedNewState,
        MSceneWindow::SceneWindowState expectedOldState)
{
    if (spy.count() != 1) {
        qWarning() << "sceneWindowStateChanged(). Signal emitted" << spy.count()
            << "times. Expected 1";
        return false;
    }

    QList<QVariant> arguments = spy.takeFirst();

    if (arguments.count() != 2) {
        qFatal("Did not connect to the proper signal");
        return false;
    }

    MSceneWindow::SceneWindowState actualNewState =
        arguments.at(0).value<MSceneWindow::SceneWindowState>();

    MSceneWindow::SceneWindowState actualOldState =
        arguments.at(1).value<MSceneWindow::SceneWindowState>();

    if (actualNewState != expectedNewState) {
        qWarning() << "sceneWindowStateChanged(). newState: Expected" << expectedNewState
            << "got" << actualNewState;
        return false;
    }

    if (actualOldState != expectedOldState) {
        qWarning() << "sceneWindowStateChanged(). oldState: Expected" << expectedOldState
            << "got" << actualNewState;
        return false;
    }

    return true;
}

TestBridge::TestBridge(QObject *parent)
    : QObject(parent)
{
}

void TestBridge::setSceneWindowState(MSceneWindow::SceneWindowState newState)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "setSceneWindowState",
            Qt::DirectConnection,
            Q_ARG(MSceneWindow::SceneWindowState, newState));
}

QTEST_MAIN(Ut_MSceneWindow)
