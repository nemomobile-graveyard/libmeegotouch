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

#include "ut_mfeedback.h"
#include "mfeedback.h"
#include "mfeedback_p.h"
#include "mapplication_mock.h"
#include "mfeedbackplayer_mock.h"

void Ut_MFeedback::init()
{
}

void Ut_MFeedback::cleanup()
{
}

void Ut_MFeedback::initTestCase()
{
}

void Ut_MFeedback::cleanupTestCase()
{
}

/*
 * Check that MFeedback name is handled correctly.
 */
void Ut_MFeedback::name()
{
    MFeedback feedback1;
    MFeedback feedback2("foo");

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
 * Check that MFeedback::play() works as expected.
 */
void Ut_MFeedback::play()
{
    MFeedbackPlayer *testPlayer = MApplication::feedbackPlayer();
    testPlayer->playedFeedbacks.clear();

    MFeedback feedback1("press-foo");
    MFeedback feedback2;
    MFeedback feedback3("release-foo");
    MFeedback feedback4("cancel-foo");

    // Play the feedbacks
    feedback1.play();
    feedback2.play();
    feedback3.play();
    feedback4.play();

    // See that the feedbacks actually got played
    QCOMPARE(testPlayer->playedFeedbacks.size(), static_cast<int>(4));
    QCOMPARE(testPlayer->playedFeedbacks.at(0), QString("press-foo"));
    QCOMPARE(testPlayer->playedFeedbacks.at(1), QString());
    QCOMPARE(testPlayer->playedFeedbacks.at(2), QString("release-foo"));
    QCOMPARE(testPlayer->playedFeedbacks.at(3), QString("cancel-foo"));
}

/*
 * Check that MFeedback::play(const QString&) works as expected.
 */
void Ut_MFeedback::playWithName()
{
    MFeedbackPlayer *testPlayer = MApplication::feedbackPlayer();
    testPlayer->playedFeedbacks.clear();

    // Play the feedbacks
    MFeedback::play("press-bar");
    MFeedback::play("cancel-everything");

    // See that the feedbacks actually got played
    QCOMPARE(testPlayer->playedFeedbacks.size(), static_cast<int>(2));
    QCOMPARE(testPlayer->playedFeedbacks.at(0), QString("press-bar"));
    QCOMPARE(testPlayer->playedFeedbacks.at(1), QString("cancel-everything"));
}

QTEST_MAIN(Ut_MFeedback)

