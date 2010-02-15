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

#ifndef _UT_DUIAPPLETRUNNER_
#define _UT_DUIAPPLETRUNNER_

#include <QObject>
#include <QLocalServer>
#include <duiappletinterface.h>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>

class DuiAppletRunner;
class DuiAppletMessage;
class DuiAppletMetaData;

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
class Ut_DuiAppletRunner: public QObject
{
    Q_OBJECT

private:

    /** DuiAppletRunner instance under testing. */
    DuiAppletRunner *m_instance;
    DuiAppletMetaData *meta;

public:
    static bool timerImmediateTimeout;
    static bool quitCalled;
    static QHash<QString, QString> gConfKeyMap;
    static bool processingInstanceSettingsFile;
    static bool processingGlobalSettingsFile;

private slots:

    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testInitializationSuccess();
    void testInitializationFailInvalidAppletId();
    void testInitializationFailInvalidInstanceDataLocation();
    void testInitializationFailInvalidMetadata();
    void testInitializationFailCommunicationFails();
    void testInitializationFailConstructWidgetFails();
    void testInitializationFailSharedMemoryAttachFails();
    void testRelayingMouseEvents();
    void testAppletRunnerQuitsIfMessageReceivingTimeouts();
    void testMessageReceivedBeforeAppletInit();
    void testPaintingOnPixmap();
    void testSendUpdateGeometryMessage();
    void testSceneChanged();
    void testObjectMenuRequestMessage();
    void testObjectMenuRequestMessageGetsPropagated();

signals:
    void sendMessage(const DuiAppletMessage &message);
    void changeScene(const QList<QRectF> &region);
};

#endif //_UT_DUIAPPLETRUNNER_
