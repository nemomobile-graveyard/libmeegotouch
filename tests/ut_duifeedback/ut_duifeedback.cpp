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

#include "ut_duifeedback.h"
#include "duifeedback.h"
#include "duifeedback_p.h"
#include "duiapplication_mock.h"
#include "duifeedbackplayer_mock.h"

void Ut_DuiFeedback::init()
{
}

void Ut_DuiFeedback::cleanup()
{
}

void Ut_DuiFeedback::initTestCase()
{
}

void Ut_DuiFeedback::cleanupTestCase()
{
}

/*
 * Check that DuiFeedback name is handled correctly.
 */
void Ut_DuiFeedback::name()
{
    DuiFeedback feedback1;
    DuiFeedback feedback2("foo");

    // Make sure initial values are correct
    QCOMPARE(feedback1.name(), QString());
    QCOMPARE(feedback2.name(), QString("foo"));

    // Set a different name and verify
    feedback2.setName("bar");
    QCOMPARE(feedback2.name(), QString("bar"));

    // Copy feedback and verify
    feedback1 = feedback2;
    QCOMPARE(feedback1.name(), QString("bar"));
}

/*
 * Check that DuiFeedback playing works as expected.
 */
void Ut_DuiFeedback::play()
{
    DuiFeedbackPlayer *testPlayer;
    DuiFeedback feedback1("press-foo");
    DuiFeedback feedback2;
    DuiFeedback feedback3("release-foo");
    DuiFeedback feedback4("cancel-foo");

    // Play the feedbacks
    feedback1.play();
    feedback2.play();
    feedback3.play();
    feedback4.play();

    // See that the feedbacks actually got played
    testPlayer = DuiApplication::feedbackPlayer();
    QCOMPARE(testPlayer->playedFeedbacks.size(), static_cast<int>(4));
    QCOMPARE(testPlayer->playedFeedbacks.at(0), QString("press-foo"));
    QCOMPARE(testPlayer->playedFeedbacks.at(1), QString());
    QCOMPARE(testPlayer->playedFeedbacks.at(2), QString("release-foo"));
    QCOMPARE(testPlayer->playedFeedbacks.at(3), QString("cancel-foo"));
}

QTEST_MAIN(Ut_DuiFeedback)

