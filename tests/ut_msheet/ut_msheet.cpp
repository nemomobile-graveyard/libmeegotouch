/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "ut_msheet.h"
#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty_stub.h"
#endif

#include <QObject>
#include <mdeviceprofile.h>
#include <MApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <MDismissEvent>
#include <mscenemanager.h>
#include <mwindow.h>

#include <msheet.h>

// QCOMPARE doesn't know MSceneWindow::SceneWindowSate enum. Thus it won't
// print "Expected" and "Actual" values in case of failure unless they're cast
// to a known type
#define STATE_COMPARE(s1, s2) QCOMPARE(static_cast<int>(s1), static_cast<int>(s2));


bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    if (isKeyboardOpen) {
        return angle == M::Angle0;
    } else {
        return true;
    }
}


Ut_MSheet::Ut_MSheet():
        subject(0),
        app(0)
{
}

void Ut_MSheet::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_msheet" };
    app = new MApplication(argc, app_name);
#ifdef HAVE_CONTEXTSUBSCRIBER
    gContextPropertyStubMap->createStub("/maemo/InternalKeyboard/Present")->stubSetReturnValue("value", QVariant("false"));
    gContextPropertyStubMap->createStub("com.nokia.policy.video_route")->stubSetReturnValue("value", QVariant(""));
    gContextPropertyStubMap->createStub("Screen.IsCovered")->stubSetReturnValue("value", QVariant("false"));
    gContextPropertyStubMap->findStub("/maemo/InternalKeyboard/Open")->stubSetReturnValue("value", QVariant("false"));

    if (MDeviceProfile::instance()->orientationFromAngle(M::Angle0) == M::Landscape) {
        gContextPropertyStubMap->findStub("Screen.TopEdge")->
                stubSetReturnValue("value", QVariant("top")); // landscape

        gContextPropertyStubMap->createStub("/Screen/Desktop/OrientationAngle")->
                stubSetReturnValue("value", QVariant(0)); // landscape

        gContextPropertyStubMap->createStub("/Screen/CurrentWindow/OrientationAngle")->
                stubSetReturnValue("value", QVariant(270)); // portrait
    } else {
        gContextPropertyStubMap->findStub("Screen.TopEdge")->
                stubSetReturnValue("value", QVariant("left")); // landscape

        gContextPropertyStubMap->createStub("/Screen/Desktop/OrientationAngle")->
                stubSetReturnValue("value", QVariant(270)); // landscape

        gContextPropertyStubMap->createStub("/Screen/CurrentWindow/OrientationAngle")->
                stubSetReturnValue("value", QVariant(0)); // portrait
    }
#endif
}

void Ut_MSheet::cleanupTestCase()
{
    delete app;
}

void Ut_MSheet::init()
{
    subject = new MSheet;
}

void Ut_MSheet::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_MSheet::testClosingStandAloneWindowDismissesSheet()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    EventSpy sheetDismissSpy;
    subject->installEventFilter(&sheetDismissSpy);

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Appeared);

    standAloneWindow->close();

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(sheetDismissSpy.typesOfEventsReceived.count(MDismissEvent::eventType()), 1);
}

void Ut_MSheet::testHidingStandAloneWindowMakesSheetDisappear()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    EventSpy sheetDismissSpy;
    subject->installEventFilter(&sheetDismissSpy);

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Appeared);

    standAloneWindow->hide();

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
    QCOMPARE(sheetDismissSpy.typesOfEventsReceived.count(MDismissEvent::eventType()), 0);
}

void Ut_MSheet::testDismissingSheetClosesStandAloneWindow()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    QVERIFY(standAloneWindow->isVisible());

    EventSpy windowCloseSpy;
    standAloneWindow->installEventFilter(&windowCloseSpy);

    subject->dismiss();

    QVERIFY(!standAloneWindow->isVisible());
    QCOMPARE(windowCloseSpy.typesOfEventsReceived.count(QEvent::Close), 1);

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    // Disappearance is immediate from scene manager's point of view
    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
}

void Ut_MSheet::testMakingSheetDisappearClosesStandAloneWindow()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    QGraphicsView *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    QVERIFY(standAloneWindow->isVisible());

    EventSpy windowCloseSpy;
    standAloneWindow->installEventFilter(&windowCloseSpy);

    subject->disappear();

    QVERIFY(!standAloneWindow->isVisible());
    QCOMPARE(windowCloseSpy.typesOfEventsReceived.count(QEvent::Close), 1);

    QApplication::processEvents(); // sheet disappearance comes on next event loop

    // Disappearance is immediate from scene manager's point of view
    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Disappeared);
}

/*
  If user wants its centralWidget to be bigger than the area of the
  central slot he has to add a MPannableViewport himself.
  This behavior also unsures that our internal pannable viewport
  doesn't messes up with some other pannable viewport that user might
  add as a child of his central widget
  */
void Ut_MSheet::testCentralWidgetDoesntGrowBeyondSlotLimits()
{
    QGraphicsWidget *bigWidget = new QGraphicsWidget;
    MSceneManager *sceneManager = new MSceneManager;
    MWindow *window = new MWindow(sceneManager);

    // Make bigWidget want to be bigger than the whole screen
    QSizeF bigSize = window->size();
    bigSize *= 2;
    bigWidget->setPreferredSize(bigSize);
    bigWidget->setMaximumSize(bigSize*4);

    subject->setCentralWidget(bigWidget);

    sceneManager->appearSceneWindowNow(subject);

    // Make sure all layouts set the geometries of their widgets
    app->processEvents();

    QGraphicsWidget *slot = bigWidget->parentWidget();

    QVERIFY(slot->size().width() <= window->size().width());
    QVERIFY(slot->size().height() <= window->size().height());

    // bigWidget should have exactly the same size of the slot
    // that contains it.
    QVERIFY(bigWidget->size().width() == slot->size().width());
    QVERIFY(bigWidget->size().height() == slot->size().height());
}

void Ut_MSheet::testSettingInitialSystemwideModeOrientation_data()
{
    QTest::addColumn<MSheet::SystemwideModeOrientation>("systemwideModeOrientation");
    QTest::addColumn<bool>("isOrientationLocked");
    QTest::addColumn<M::Orientation>("orientation");
    QTest::addColumn<QVariant>("followsCurrentApplicationWindowOrientation");

    QTest::newRow("FollowsDeviceOrientation")
            << MSheet::FollowsDeviceOrientation
            << false // not locked
            << M::Landscape // since device orientation is in landscape
            << QVariant(); // don't follow current app window

    QTest::newRow("FollowsCurrentAppWindowOrientation")
            << MSheet::FollowsCurrentAppWindowOrientation
            << false // not locked
            << M::Portrait // since current app. window is in portrait
            << QVariant(1); // do follow current app window

    QTest::newRow("LockedToPortraitOrientation")
            << MSheet::LockedToPortraitOrientation
            << true // locked
            << M::Portrait // since we're locked to portrait
            << QVariant(); // don't follow current app window

    QTest::newRow("LockedToLandscapeOrientation")
            << MSheet::LockedToLandscapeOrientation
            << true // locked
            << M::Landscape // since we're locked to landscape
            << QVariant(); // don't follow current app window
}

void Ut_MSheet::testSettingInitialSystemwideModeOrientation()
{
    QFETCH(MSheet::SystemwideModeOrientation, systemwideModeOrientation);
    QFETCH(bool, isOrientationLocked);
    QFETCH(M::Orientation, orientation);
    QFETCH(QVariant, followsCurrentApplicationWindowOrientation);

    subject->setSystemwideModeOrientation(systemwideModeOrientation);
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    MWindow *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    QCOMPARE(standAloneWindow->isOrientationLocked(), isOrientationLocked);
#ifdef HAVE_CONTEXTSUBSCRIBER
    QCOMPARE(standAloneWindow->orientation(), orientation);
#else
    Q_UNUSED(orientation);
#endif
    QCOMPARE(standAloneWindow->property("followsCurrentApplicationWindowOrientation"),
             followsCurrentApplicationWindowOrientation);
}

void Ut_MSheet::testChangingSystemwideModeOrientationAfterAppearance_data()
{
    QTest::addColumn<MSheet::SystemwideModeOrientation>("systemwideModeOrientation");
    QTest::addColumn<bool>("isOrientationLocked");
    QTest::addColumn<M::Orientation>("orientation");
    QTest::addColumn<QVariant>("followsCurrentApplicationWindowOrientation");

    QTest::newRow("FollowsDeviceOrientation")
            << MSheet::FollowsDeviceOrientation
            << false // not locked
            << M::Landscape // since device orientation is in landscape
            << QVariant(); // don't follow current app window

    QTest::newRow("FollowsCurrentAppWindowOrientation")
            << MSheet::FollowsCurrentAppWindowOrientation
            << false // not locked
            << M::Portrait // since current app. window is in portrait
            << QVariant(1); // do follow current app window

    QTest::newRow("LockedToPortraitOrientation")
            << MSheet::LockedToPortraitOrientation
            << true // locked
            << M::Portrait // since we're locked to portrait
            << QVariant(); // don't follow current app window

    QTest::newRow("LockedToLandscapeOrientation")
            << MSheet::LockedToLandscapeOrientation
            << true // locked
            << M::Landscape // since we're locked to landscape
            << QVariant(); // don't follow current app window
}

void Ut_MSheet::testChangingSystemwideModeOrientationAfterAppearance()
{
    QFETCH(MSheet::SystemwideModeOrientation, systemwideModeOrientation);
    QFETCH(bool, isOrientationLocked);
    QFETCH(M::Orientation, orientation);
    QFETCH(QVariant, followsCurrentApplicationWindowOrientation);

    // show it with its default systemwide orientation: FollowsDeviceOrientation
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);

    MWindow *standAloneWindow = fetchStandAloneWindowOfSubject();
    QVERIFY(standAloneWindow != 0);

    // device orientation is landscape
    QCOMPARE(standAloneWindow->isOrientationLocked(), false);
    QCOMPARE(standAloneWindow->orientation(), M::Landscape);
    QCOMPARE(standAloneWindow->property("followsCurrentApplicationWindowOrientation"),
             QVariant());

    // and now change it
    subject->setSystemwideModeOrientation(systemwideModeOrientation);

    QCOMPARE(standAloneWindow->isOrientationLocked(), isOrientationLocked);
#ifdef HAVE_CONTEXTSUBSCRIBER
    QCOMPARE(standAloneWindow->orientation(), orientation);
#else
    Q_UNUSED(orientation);
#endif
    QCOMPARE(standAloneWindow->property("followsCurrentApplicationWindowOrientation"),
             followsCurrentApplicationWindowOrientation);
}

void Ut_MSheet::testHeaderFloating()
{
    subject->appearSystemwide(MSceneWindow::KeepWhenDone);
    STATE_COMPARE(subject->sceneWindowState(), MSceneWindow::Appeared);
    QVERIFY(!subject->isHeaderFloating());

    QSizeF sheetSize = subject->size();
    QSizeF centralWidgetSize = subject->centralWidget()->size();
    subject->setHeaderFloating(true);
    qApp->processEvents();

    QVERIFY(subject->isHeaderFloating());
    QSizeF sheetSizeWithFloatingHeader = subject->size();
    QSizeF centralWidgetSizeWithFloatingHeader = subject->centralWidget()->size();

    QCOMPARE(sheetSize, sheetSizeWithFloatingHeader);
    QCOMPARE(sheetSize, centralWidgetSizeWithFloatingHeader);
    QVERIFY(centralWidgetSize != centralWidgetSizeWithFloatingHeader);
    subject->dismiss();
}

MWindow *Ut_MSheet::fetchStandAloneWindowOfSubject()
{
    if (subject->scene()
            && subject->scene()->views().count() == 1) {
        return qobject_cast<MWindow*>(subject->scene()->views()[0]);
    } else {
        return 0;
    }
}

bool EventSpy::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched);
    typesOfEventsReceived << event->type();
    return false;
}

QTEST_APPLESS_MAIN(Ut_MSheet)
