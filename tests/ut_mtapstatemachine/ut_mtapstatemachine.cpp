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

#include "ut_mtapstatemachine.h"
#include "mtapstatemachine.h"
#include "mtapstatemachine_p.h"
#include "mtapstatemachinestyle.h"

#include <QMouseEvent>
#include <QtTest/QtTest>

#include <QGraphicsSceneMouseEvent>

#include <QStateMachine>
#include <MApplication>

//Mocking MThemePrivate, because the style object is private to library
//and we need to compile some functionality into the unittest.
#include "mtheme_p.h"
void MThemePrivate::unregisterStyleContainer(MStyleContainer*)
{
}

void MThemePrivate::registerStyleContainer(MStyleContainer *)
{
}

#include "mtheme.h"
//Filling the values of the style object.
static const int initialWaitTime = 150;
static const int blinkDuration = 300;

MTapStateMachineStyle stateMachineStyle;
const MStyle *MTheme::style(const char *,
                            const QString &) {
    stateMachineStyle.setInitialWaitTime(initialWaitTime);
    stateMachineStyle.setBlinkDuration(blinkDuration);
    return &stateMachineStyle;
}

void MTheme::releaseStyle(const MStyle *)
{
}

MApplication *app;
void Ut_MTapStateMachine::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_mtapstatemachine" };
    app = new MApplication(argc, app_name);
    fakeObject = new QObject();
}

void Ut_MTapStateMachine::cleanupTestCase()
{
    delete app;
}

void Ut_MTapStateMachine::init()
{

    machine = new MTapStateMachine(fakeObject);
}

void Ut_MTapStateMachine::cleanup()
{
    delete machine;
}

void Ut_MTapStateMachine::testLongPressAndRelease()
{
    QSignalSpy *spyDelayedPress = new QSignalSpy(machine, SIGNAL(delayedPress()));
    QSignalSpy *spyRelease = new QSignalSpy(machine, SIGNAL(release()));

    QCoreApplication::processEvents();

    QCoreApplication::sendEvent(fakeObject, new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress));
    QCoreApplication::processEvents();
    sleep(1);
    QCoreApplication::processEvents();
    QCoreApplication::sendEvent(fakeObject, new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease));
    QCoreApplication::processEvents();

    QCOMPARE(spyDelayedPress->count(),1);
    QCOMPARE(spyRelease->count(),1);
}

void Ut_MTapStateMachine::testShortTapAndRelease()
{
    QSignalSpy *spyDelayedPress = new QSignalSpy(machine, SIGNAL(delayedPress()));
    QSignalSpy *spyRelease = new QSignalSpy(machine, SIGNAL(release()));

    QCoreApplication::processEvents();

    QCoreApplication::sendEvent(fakeObject, new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMousePress));
    QCoreApplication::processEvents();
    QCoreApplication::sendEvent(fakeObject, new QGraphicsSceneMouseEvent(QEvent::GraphicsSceneMouseRelease));
    QCoreApplication::processEvents();

    QCOMPARE(spyDelayedPress->count(),1);
    QCOMPARE(spyRelease->count(),0);
    sleep(1);
    QCoreApplication::processEvents();
    QCOMPARE(spyRelease->count(),1);
}


QTEST_APPLESS_MAIN(Ut_MTapStateMachine)

