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

#ifndef _UT_MEXTENSIONRUNNER_
#define _UT_MEXTENSIONRUNNER_

#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>

class MExtensionRunner;
class MAppletMessage;
class MAppletMetaData;

class MouseEventFilter : public QObject
{
    Q_OBJECT

public:
    ~MouseEventFilter();

protected:
    bool eventFilter(QObject *o, QEvent *e);

public:
    QList<QGraphicsSceneMouseEvent *> events;
};

// Test case must inherit QObject
class Ut_MExtensionRunner: public QObject
{
    Q_OBJECT

private:

    /** MExtensionRunner instance under testing. */
    MExtensionRunner *m_instance;
    MAppletMetaData *meta;

public:
    static bool timerImmediateTimeout;
    static bool quitCalled;
    static QHash<QString, QString> gConfKeyMap;

private slots:

    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitializationSuccess();
    void testInitializationFailCommunicationFails();
    void testInitializationFailSharedMemoryAttachFails();
    void testRelayingMouseEvents();
    void testAppletRunnerQuitsIfMessageReceivingTimeouts();
    void testMessageReceivedBeforeAppletInit();
    void testPaintingOnPixmap();
    void testSendUpdateGeometryMessage();
    void testSceneChanged();
    void testObjectMenuRequestMessage();
    void testObjectMenuRequestMessageGetsPropagated();
#ifdef HAVE_CONTEXTSUBSCRIBER
    void testWhenDisplayBlankedThenAliveCheckingStops();
#endif

signals:
    void sendMessage(const MAppletMessage &message);
    void changeScene(const QList<QRectF> &region);
};

#endif //_UT_MEXTENSIONRUNNER_
