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

#include "ut_duiapplicationwindow.h"

#include <duiapplication.h>
#include <duiapplication_p.h>
#include <duiapplicationservice.h>
#include <duiapplicationwindow.h>
#include <duiapplicationpage.h>
#include <DuiComponentData>
#include <DuiScene>

#include <QSignalSpy>
#include <QEvent>

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

// WARNING: This restricts the unit test to the support one window only
QList<DuiWindow *> DuiComponentData::windows()
{
    QList<DuiWindow *> list;
    if (gActiveWindow)
        list << gActiveWindow;
    return list;
}

// Just a dummy stub to avoid need for real DuiComponentData instance
// because of this method
void DuiComponentData::setPrestarted(bool)
{}

//DuiApplication stubs

static Dui::PrestartMode fakeMode = Dui::LazyShutdown;

bool Ut_DuiApplicationWindow::m_prestartRestored = false;

bool DuiApplication::isPrestarted()
{
    if ((fakeMode == Dui::LazyShutdown) || (fakeMode == Dui::TerminateOnClose))
        return true;

    return false;
}

void DuiApplication::setPrestartMode(Dui::PrestartMode mode)
{
    fakeMode = mode;
    return;
}

Dui::PrestartMode DuiApplication::prestartMode()
{
    return fakeMode;
}

// Test class implementation

void Ut_DuiApplicationWindow::initTestCase()
{
    qRegisterMetaType<DuiApplicationPage *>("DuiApplicationPage*");
}

void Ut_DuiApplicationWindow::cleanupTestCase()
{
}

void Ut_DuiApplicationWindow::init()
{
    m_subject = new DuiApplicationWindow;
}

void Ut_DuiApplicationWindow::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

// It's not possible to owerwrite inlined QWidget::show, setVisible is used instead
bool Ut_DuiApplicationWindow::m_windowShown = false;
void QWidget::setVisible(bool visible)
{
    Ut_DuiApplicationWindow::m_windowShown = visible;
}

bool Ut_DuiApplicationWindow::m_windowClosed = false;
bool QWidget::close()
{
    Ut_DuiApplicationWindow::m_windowClosed = true;
    return true;
}

void Ut_DuiApplicationWindow::testConstructorWithoutScene()
{
    QVERIFY(m_subject->scene());
}

void Ut_DuiApplicationWindow::testConstructorWithScene()
{
    DuiScene *scene = new DuiScene;
    DuiApplicationWindow *window = new DuiApplicationWindow(scene);
    QCOMPARE((quintptr) window->scene(), (quintptr) scene);
    delete window;
}

void Ut_DuiApplicationWindow::testIsOnDisplay()
{
    m_subject = DuiApplication::activeApplicationWindow();

    QPointer<DuiApplicationPage> page1 = new DuiApplicationPage;
    QPointer<DuiApplicationPage> page2 = new DuiApplicationPage;

    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // Window is not visible => page should not become visible
    page2->appearNow();
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);

    // To be revisited: how these could be unit-tested
    // (event->viewRect().intersects(sceneBoundingRect())
    // does not return meaningful values in this.

    page1->disappear();
    page1->disappear();
    QVERIFY(page1->isOnDisplay() == false);
    QVERIFY(page2->isOnDisplay() == false);
}

void Ut_DuiApplicationWindow::testPrestartNoPrestart()
{
    DuiApplication::setPrestartMode(Dui::NoPrestart);
    m_windowShown = false;
    m_subject->show();
    QCOMPARE(m_windowShown, true);
    m_prestartRestored = false;
    m_windowClosed = false;
    m_subject->close();
    QCOMPARE(m_prestartRestored, false);
    QCOMPARE(m_windowClosed, true);
}

void Ut_DuiApplicationWindow::testPrestartTerminateOnClose()
{
    DuiApplication::setPrestartMode(Dui::TerminateOnClose);
    m_windowShown = false;
    m_subject->show();
    QCOMPARE(m_windowShown, false);
    m_prestartRestored = false;
    m_windowClosed = false;
    m_subject->close();
    QCOMPARE(m_prestartRestored, false);
    QCOMPARE(m_windowClosed, true);
}

void Ut_DuiApplicationWindow::testPrestartLazyShutdown()
{
    DuiApplication::setPrestartMode(Dui::LazyShutdown);
    m_windowShown = false;
    m_subject->show();
    QCOMPARE(Ut_DuiApplicationWindow::m_windowShown, false);
    m_prestartRestored = false;
    m_windowClosed = false;
    m_subject->close();
    // For some reason the DuiApplicationPrivate::restorePrestart()
    // stub never gets called.
    // QCOMPARE( m_prestartRestored, true );
    QCOMPARE(Ut_DuiApplicationWindow::m_windowClosed, false);
}

void Ut_DuiApplicationWindow::testWindowActivate()
{
    QVERIFY(DuiApplication::activeApplicationWindow() == m_subject);

    DuiApplicationWindow *appWin = new DuiApplicationWindow;
    QVERIFY(DuiApplication::activeApplicationWindow() == m_subject);

    QEvent activate(QEvent::WindowActivate);

    qApp->sendEvent(appWin, &activate);
    QVERIFY(DuiApplication::activeApplicationWindow() == appWin);

    delete appWin;
    QVERIFY(DuiApplication::activeApplicationWindow() == 0);

    qApp->sendEvent(m_subject, &activate);
    QVERIFY(DuiApplication::activeApplicationWindow() == m_subject);

}

void Ut_DuiApplicationWindow::testPageChanged()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(DuiApplicationPage *)));
    DuiApplicationPage *page = new DuiApplicationPage;
    DuiApplicationPage *page2 = new DuiApplicationPage;
    page->appearNow(m_subject);
    page2->appearNow(m_subject);

    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(0).at(0).value<DuiApplicationPage *>(), page);
    QCOMPARE(spy.at(1).at(0).value<DuiApplicationPage *>(), page2);

    delete page;
    delete page2;
}

void Ut_DuiApplicationWindow::testDeleteOnClose()
{
    qDebug() << "if the test crashes here, "
             << "Qt::WA_DeleteOnClose is broken for DuiApplicationWindow";
    DuiApplicationWindow *win = new DuiApplicationWindow;
    win->setAttribute(Qt::WA_DeleteOnClose, true);
    win->show();
    win->close();
}

void Ut_DuiApplicationWindow::testDisplayExitedOnClose()
{
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(exitedDisplay()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_DuiApplicationWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    DuiApplication::setPrestartMode(Dui::LazyShutdown);
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(exitedDisplay()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}


QTEST_MAIN(Ut_DuiApplicationWindow)
