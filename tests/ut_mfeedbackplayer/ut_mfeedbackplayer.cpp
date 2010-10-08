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

#include "ut_mfeedbackplayer.h"
#include "mfeedbackplayer.h"
#include "mfeedbackplayer_p.h"
#include "mapplicationprivate_mock.h"
#include "qlocalsocket_mock.h"
#include <MDebug>

void Ut_MFeedbackPlayer::init()
{
    feedbackPlayerContainer = new MApplicationPrivate("fooApplication");
    feedbackPlayer = feedbackPlayerContainer->feedbackPlayer;
}

void Ut_MFeedbackPlayer::cleanup()
{
    delete feedbackPlayerContainer;
    feedbackPlayerContainer = 0;
}

void Ut_MFeedbackPlayer::initTestCase()
{
}

void Ut_MFeedbackPlayer::cleanupTestCase()
{
}

/*
 * Check that socket connection is established and that initial
 * communication is correct.
 */
void Ut_MFeedbackPlayer::initialCommunicaton()
{
    QLocalSocket *testSocket;
    QString writtenString;
    QString writtenVersion;

    // Get written string
    testSocket = QLocalSocket::instance();
    QDataStream testStream(*testSocket->getWrittenData());
    testStream >> writtenVersion;
    testStream >> writtenString;

    // Make sure the socket was connected
    QCOMPARE(testSocket->state(), QLocalSocket::ConnectedState);

    // Make sure the data was as expected
    QCOMPARE(writtenString, QString("fooApplication"));
    QCOMPARE(writtenVersion, QString("FeedbackProtocolVersion#1"));
}

/*
 * Check that reconnection works as expected if socket is unexpectedly
 * disconnected.
 */
void Ut_MFeedbackPlayer::reconnect()
{
    QLocalSocket *testSocket;
    QString writtenString;
    QString writtenVersion;

    // Prepare socket
    testSocket = QLocalSocket::instance();
    testSocket->clearReceivedData();
    testSocket->suddenDisconnect();

    // First reconnection takes place after 10ms, so wait
    // for that reconnection.
    QTest::qWait(15);

    // Make sure all events are processed (= timer above
    // has been handled) before proceeding.
    QApplication::processEvents();

    QDataStream testStream(*testSocket->getWrittenData());
    testStream >> writtenVersion;
    testStream >> writtenString;

    // Make sure the socket was connected
    QCOMPARE(testSocket->state(), QLocalSocket::ConnectedState);

    // Make sure the data was as expected
    QCOMPARE(writtenString, QString("fooApplication"));
    QCOMPARE(writtenVersion, QString("FeedbackProtocolVersion#1"));
}

/*
 * Check that reconnection attempts stops if there are many successfull connections
 * that then get disconnected very fast.
 */
void Ut_MFeedbackPlayer::tooManyReconnections()
{
    QLocalSocket *testSocket;
    QString writtenString;
    QString writtenVersion;

    // Prepare socket
    testSocket = QLocalSocket::instance();

    // 10 fast reconnections should be made successfully
    for (int i = 0; i < 10; ++i) {
        testSocket->clearReceivedData();
        testSocket->suddenDisconnect();

        // First reconnection takes place after 10ms, so wait
        // for that reconnection.
        QTest::qWait(15);

        // Make sure all events are processed (= timer above
        // has been handled) before proceeding.
        QApplication::processEvents();

        QDataStream testStream(*testSocket->getWrittenData());
        testStream >> writtenVersion;
        testStream >> writtenString;

        // Make sure the socket was connected
        QCOMPARE(testSocket->state(), QLocalSocket::ConnectedState);

        // Make sure the data was as expected
        QCOMPARE(writtenString, QString("fooApplication"));
        QCOMPARE(writtenVersion, QString("FeedbackProtocolVersion#1"));
    }

    // Reconnection attempts should end now
    testSocket->clearReceivedData();
    testSocket->suddenDisconnect();

    QTest::qWait(60);

    // Make sure all events are processed (= timer above
    // has been handled) before proceeding.
    QApplication::processEvents();

    // Make sure the socket was not connected
    QCOMPARE(testSocket->state(), QLocalSocket::UnconnectedState);
}

/*
 * Check that feedbacks are played as expected.
 */
void Ut_MFeedbackPlayer::playFeedback()
{
    QLocalSocket *testSocket;
    QString writtenString;

    // Prepare socket
    testSocket = QLocalSocket::instance();
    testSocket->clearReceivedData();

    // Play a few feedbacks
    feedbackPlayer->play(QString("fooFeedback"));
    feedbackPlayer->play(QString());
    feedbackPlayer->play(QString("barFeedback"));

    QDataStream testStream(*testSocket->getWrittenData());

    // Make sure the data was as expected
    testStream >> writtenString;
    QCOMPARE(writtenString, QString("fooFeedback"));
    // Empty feedback should not be sent at all
    testStream >> writtenString;
    QCOMPARE(writtenString, QString("barFeedback"));
}

QTEST_MAIN(Ut_MFeedbackPlayer)

