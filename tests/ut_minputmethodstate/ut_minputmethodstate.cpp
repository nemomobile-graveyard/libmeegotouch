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

#include <QSignalSpy>

#include "ut_minputmethodstate.h"
#include "minputmethodstate.h"

void Ut_MInputMethodState::initTestCase()
{
    qRegisterMetaType<M::OrientationAngle>("M::OrientationAngle");

    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_minputmethodstate" };

    m_app.reset(new MApplication(argc, app_name));
}

void Ut_MInputMethodState::cleanupTestCase()
{
    m_app->quit();
}

void Ut_MInputMethodState::init()
{
}

void Ut_MInputMethodState::cleanup()
{
}

void Ut_MInputMethodState::testActiveWindowOrientationAngle()
{
    MInputMethodState *state = MInputMethodState::instance();
    QSignalSpy spy(state, SIGNAL(activeWindowOrientationAngleChanged(M::OrientationAngle)));

    // Initial state
    QCOMPARE(state->activeWindowOrientationAngle(), M::Angle0);


    // Set, get, get notified
    state->setActiveWindowOrientationAngle(M::Angle270);
    QCOMPARE(state->activeWindowOrientationAngle(), M::Angle270);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.first().first().value<M::OrientationAngle>(), M::Angle270);

    // Set the same orientation again, don't get notified
    state->setActiveWindowOrientationAngle(M::Angle270);
    QCOMPARE(spy.count(), 1);
}

void Ut_MInputMethodState::testInputMethodArea()
{
    MInputMethodState *state = MInputMethodState::instance();
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


QTEST_APPLESS_MAIN(Ut_MInputMethodState)
