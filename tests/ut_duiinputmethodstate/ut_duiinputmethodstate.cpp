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

#include <QSignalSpy>

#include "ut_duiinputmethodstate.h"
#include "duiinputmethodstate.h"

Q_DECLARE_METATYPE(Dui::OrientationAngle)

void Ut_DuiInputMethodState::initTestCase()
{
    qRegisterMetaType<Dui::OrientationAngle>("Dui::OrientationAngle");
}

void Ut_DuiInputMethodState::cleanupTestCase()
{
}

void Ut_DuiInputMethodState::init()
{
}

void Ut_DuiInputMethodState::cleanup()
{
}

void Ut_DuiInputMethodState::testActiveWindowOrientationAngle()
{
    DuiInputMethodState *state = DuiInputMethodState::instance();
    QSignalSpy spy(state, SIGNAL(activeWindowOrientationAngleChanged(Dui::OrientationAngle)));

    // Initial state
    QCOMPARE(state->activeWindowOrientationAngle(), Dui::Angle0);


    // Set, get, get notified
    state->setActiveWindowOrientationAngle(Dui::Angle270);
    QCOMPARE(state->activeWindowOrientationAngle(), Dui::Angle270);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<Dui::OrientationAngle>(), Dui::Angle270);

    // Set the same orientation again, don't get notified
    state->setActiveWindowOrientationAngle(Dui::Angle270);
    QCOMPARE(spy.count(), 1);
}

void Ut_DuiInputMethodState::testInputMethodArea()
{
    DuiInputMethodState *state = DuiInputMethodState::instance();
    QSignalSpy spy(state, SIGNAL(inputMethodAreaChanged(QRect)));

    // Initial state
    QCOMPARE(state->inputMethodArea(), QRect());

    // Set, get, get notified
    const QRect rect(0, 0, 10, 10);
    state->setInputMethodArea(rect);
    QCOMPARE(state->inputMethodArea(), rect);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<QRect>(), rect);

    // Set the same orientation again, don't get notified
    state->setInputMethodArea(rect);
    QCOMPARE(spy.count(), 1);
}


QTEST_APPLESS_MAIN(Ut_DuiInputMethodState)
