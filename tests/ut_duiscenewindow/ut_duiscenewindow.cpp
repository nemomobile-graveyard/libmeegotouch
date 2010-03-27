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

#include "ut_duiscenewindow.h"

#include <DuiDismissEvent>
#include <DuiScene>
#include <DuiSceneManager>
#include <DuiApplicationWindow>
#include <DuiWindow>
#include <DuiOverlay>
#include <DuiComponentData>

class MyDuiDismissEventFilter : public QObject
{
public:
    MyDuiDismissEventFilter() : QObject(0) {
        count = 0;
    }

    bool eventFilter(QObject *watched, QEvent *event) {
        Q_UNUSED(watched)
        if (event->type() == DuiDismissEvent::eventType()) {
            count++;
        }
        return false;
    }

    int count;
};

// DuiWidgetController stubs
const DuiWidgetView *DuiWidgetController::view() const
{
    return 0;
}

// DuiDeviceProfile stubs
class DuiDeviceProfile
{
public:
    static DuiDeviceProfile *instance();
    QSize resolution() const;
};

DuiDeviceProfile *DuiDeviceProfile::instance()
{
    static DuiDeviceProfile p;
    return &p;
}

QSize DuiDeviceProfile::resolution() const
{
    return QSize(1000, 500);
}

// DuiComponentData stubs
DuiWindow *gActiveWindow = 0;
bool DuiComponentData::softwareRendering()
{
    return true;
}

bool DuiComponentData::fullScreen()
{
    return false;
}

void DuiComponentData::setActiveWindow(DuiWindow *window)
{
    gActiveWindow = window;
}

QList<DuiWindow *> DuiComponentData::windows()
{
    QList<DuiWindow *> windowList;
    if (gActiveWindow)
       windowList.append(gActiveWindow);
    return windowList;
}

DuiWindow *DuiComponentData::activeWindow()
{
    return gActiveWindow;
}

DuiApplicationWindow *DuiComponentData::activeApplicationWindow()
{
    return qobject_cast<DuiApplicationWindow *>(gActiveWindow);
}

void DuiComponentData::registerWindow(DuiWindow *window)
{
    if (gActiveWindow == 0)
        setActiveWindow(window);
}

void DuiComponentData::unregisterWindow(DuiWindow *window)
{
    if (gActiveWindow == window)
        setActiveWindow(0);
}

bool DuiComponentData::emulateTwoFingerGestures()
{
    return false;
}

// Test class implementation

void Ut_DuiSceneWindow::init()
{
    m_subject = new DuiOverlay;
}

void Ut_DuiSceneWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_DuiSceneWindow::initTestCase()
{
    window = new DuiWindow(new DuiSceneManager);
}

void Ut_DuiSceneWindow::cleanupTestCase()
{
    delete window;
    window = 0;
}

void Ut_DuiSceneWindow::testAccessors()
{
    m_subject->setManagedManually(true);

    QCOMPARE(m_subject->windowType(), DuiSceneWindow::Overlay);
    QCOMPARE(m_subject->isManagedManually(), true);
}

void Ut_DuiSceneWindow::testAppear()
{
    m_subject->appear(window);

    QCOMPARE(m_subject->sceneManager(), window->sceneManager());
    QVERIFY(m_subject->scene() == static_cast<QGraphicsScene *>(window->scene()));
}

/*
 * Appearing on a DuiWindow without a scene manager should just work.
 * Scene manager should be created on the fly.
 */
void Ut_DuiSceneWindow::testAppearWithoutSceneManager()
{
    window->setSceneManager(0);

    m_subject->appear(window);

    QVERIFY(m_subject->sceneManager() == window->sceneManager());
    QVERIFY(m_subject->scene() == static_cast<QGraphicsScene *>(window->scene()));
}

void Ut_DuiSceneWindow::testAppearedDisappearedSignals()
{
    QSignalSpy spyAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyDisappeared(m_subject, SIGNAL(disappeared()));

    m_subject->appearNow();

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(spyDisappeared.count(), 0);

    m_subject->disappearNow();

    QCOMPARE(spyAppeared.count(), 1);
    QCOMPARE(spyDisappeared.count(), 1);
}

void Ut_DuiSceneWindow::opacityAfterDisappearNow()
{
    m_subject->setOpacity(0.0);
    m_subject->appearNow();
    m_subject->disappearNow();
    QCOMPARE(m_subject->opacity(), 1.0);
}

void Ut_DuiSceneWindow::opacityAfterDisappear()
{
    m_subject->setOpacity(0.0);
    m_subject->appearNow();
    m_subject->disappear();
    QCOMPARE(m_subject->opacity(), 1.0);
}

void Ut_DuiSceneWindow::testDismiss()
{
    MyDuiDismissEventFilter dismissEventFilter;
    m_subject->installEventFilter(&dismissEventFilter);

    m_subject->appearNow();

    QSignalSpy spyWindowAppeared(m_subject, SIGNAL(appeared()));
    QSignalSpy spyWindowDisappeared(m_subject, SIGNAL(disappeared()));

    m_subject->dismissNow();

    QCOMPARE(spyWindowAppeared.count(), 0);
    QCOMPARE(spyWindowDisappeared.count(), 1);
    // QCOMPARE(dismissEventFilter.count, 1); FIXME: Dismiss events are disabled in 0.18 due to ABI freeze
}

void Ut_DuiSceneWindow::testDestroyWhenDoneCallingDisappear()
{
    m_subject->appearNow(DuiSceneWindow::DestroyWhenDone);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    m_subject->disappearNow();

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_DuiSceneWindow::testDestroyWhenDoneCallingDismiss()
{
    m_subject->appearNow(DuiSceneWindow::DestroyWhenDone);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    m_subject->dismissNow();

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_DuiSceneWindow::testDestroyWhenDismissedCallingDisappear()
{
    m_subject->appearNow(DuiSceneWindow::DestroyWhenDismissed);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    m_subject->disappearNow();

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 0);
}

void Ut_DuiSceneWindow::testDestroyWhenDismissedCallingDismiss()
{
    m_subject->appearNow(DuiSceneWindow::DestroyWhenDismissed);

    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));

    m_subject->dismissNow();

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 1);

    m_subject = 0;
}

void Ut_DuiSceneWindow::testDismissedStateReset()
{
    m_subject->appearNow();
    m_subject->dismissNow();

    // internal "dismissed" state should be reset to false on next reappearance
    // Therefore the disappearance below should not cause self-destruction

    m_subject->appearNow(DuiSceneWindow::DestroyWhenDismissed);
    QSignalSpy spyDestroyed(m_subject, SIGNAL(destroyed()));
    m_subject->disappearNow();

    processPendingEvents();

    QCOMPARE(spyDestroyed.count(), 0);
}

void Ut_DuiSceneWindow::processPendingEvents()
{
    // Send the posted QEvent::DeferredDelete from deleteLater().
    QCoreApplication::sendPostedEvents();

    // Quit when there are no more pending events (like DeferredDelete)
    QTimer::singleShot(0, QCoreApplication::instance(), SLOT(quit()));

    QCoreApplication::instance()->exec();
}

QTEST_MAIN(Ut_DuiSceneWindow)
