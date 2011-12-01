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

#ifndef UT_MSHEET_H
#define UT_MSHEET_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include <QEvent>

class MApplication;
class MSheet;
class QGraphicsView;
class MWindow;
class MStatusBar;

class Ut_MSheet : public QObject
{
    Q_OBJECT
public:
    Ut_MSheet();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testClosingStandAloneWindowDismissesSheet();
    void testHidingStandAloneWindowMakesSheetDisappear();
    void testDismissingSheetClosesStandAloneWindow();
    void testMakingSheetDisappearClosesStandAloneWindow();
    void testDismissingSheetTwoTimesDoesNotCrash();
    void testCentralWidgetDoesntGrowBeyondSlotLimits();

    void testSettingInitialSystemwideModeOrientation_data();
    void testSettingInitialSystemwideModeOrientation();

    void testChangingSystemwideModeOrientationAfterAppearance_data();
    void testChangingSystemwideModeOrientationAfterAppearance();

    void testHeaderFloating();

#ifdef HAVE_CONTEXTSUBSCRIBER
    void testPhoneCallForcedStatusBarOnStandalone_data();
    void testPhoneCallForcedStatusBarOnStandalone();
#endif

private:
    MWindow *fetchStandAloneWindowOfSubject();
    MStatusBar *fetchStatusBarForStandAloneWindow();

    MSheet *subject;
    MApplication *app;
};

class EventSpy : public QObject
{
    Q_OBJECT
public:
    EventSpy() : QObject(0) {}
    virtual ~EventSpy() {}

    virtual bool eventFilter(QObject *watched, QEvent *event);

    QList<QEvent::Type> typesOfEventsReceived;
};

#endif
