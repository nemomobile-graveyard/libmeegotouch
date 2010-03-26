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
#include <DuiSceneManager>
#include <DuiEscapeButtonPanel>

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
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), true);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_DuiApplicationWindow::testPrestartTerminateOnClose()
{
    DuiApplication::setPrestartMode(Dui::TerminateOnClose);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), false);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
}

void Ut_DuiApplicationWindow::testPrestartLazyShutdown()
{
    DuiApplication::setPrestartMode(Dui::LazyShutdown);
    m_subject->show();
    QCOMPARE(m_subject->isVisible(), false);
    m_subject->close();
    QCOMPARE(m_subject->isVisible(), false);
    QCOMPARE(m_subject->isHidden(), true);
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
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_DuiApplicationWindow::testDisplayExitedOnCloseLazyShutdownApp()
{
    DuiApplication::setPrestartMode(Dui::LazyShutdown);
    m_subject->show();
    QSignalSpy spy(m_subject, SIGNAL(displayExited()));
    m_subject->close();
    QCOMPARE(spy.count(), 1);
}

void Ut_DuiApplicationWindow::testPageEscapeAuto()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());

    QVERIFY(escapeButtonPanel != 0);

    firstPage->appearNow(m_subject);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::CloseMode);

    secondPage->appearNow(m_subject);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::BackMode);

    secondPage->dismissNow();

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::CloseMode);
}

void Ut_DuiApplicationWindow::testPageEscapeAutoWhenAddingPageHistory()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());
    QList<DuiSceneWindow *> pageHistory;

    QVERIFY(escapeButtonPanel != 0);

    secondPage->appearNow(m_subject);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::CloseMode);

    pageHistory.append(firstPage);
    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::BackMode);
}

void Ut_DuiApplicationWindow::testPageEscapeAutoWhenClearingPageHistory()
{
    DuiApplicationPage *firstPage = new DuiApplicationPage;
    DuiApplicationPage *secondPage = new DuiApplicationPage;
    DuiEscapeButtonPanel *escapeButtonPanel = fetchEscapeButtonPanel(m_subject->scene()->items());
    QList<DuiSceneWindow *> pageHistory;

    QVERIFY(escapeButtonPanel != 0);

    firstPage->appearNow(m_subject);
    secondPage->appearNow(m_subject);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::BackMode);

    m_subject->sceneManager()->setPageHistory(pageHistory);

    QCOMPARE(escapeButtonPanel->escapeMode(), DuiEscapeButtonPanelModel::CloseMode);
}

DuiEscapeButtonPanel *Ut_DuiApplicationWindow::fetchEscapeButtonPanel(
        const QList<QGraphicsItem *> &itemsList) const
{
    int i = 0;
    int itemsCount = itemsList.count();
    QGraphicsItem *item;
    QGraphicsWidget *widget;
    DuiEscapeButtonPanel *escapeButtonPanel = 0;

    while (escapeButtonPanel == 0 && i < itemsCount) {
        item = itemsList.at(i);
        if (item->isWidget()) {
            widget = static_cast<QGraphicsWidget *>(item);
            escapeButtonPanel = qobject_cast<DuiEscapeButtonPanel*>(widget);
        }

        ++i;
    }

    return escapeButtonPanel;
}

QTEST_MAIN(Ut_DuiApplicationWindow)
