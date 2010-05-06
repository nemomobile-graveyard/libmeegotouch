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

#ifndef UT_MAPPLETHANDLE
#define UT_MAPPLETHANDLE

#include <QObject>
#include "mapplethandle.h"
#include "mapplethandle_p.h"
#include "mapplethandlestyle.h"

class MTestAppletHandlePrivate : public MAppletHandlePrivate
{
};

class MTestAppletHandle : public MAppletHandle
{
    Q_DECLARE_PRIVATE(MTestAppletHandle)

public:
    MTestAppletHandle();
    void messageReceivedFromRunner(const MAppletMessage &message);
};

class MApplication;

// Test case must inherit QObject
class Ut_MAppletHandle : public QObject
{
    Q_OBJECT

private:
    // M Application for the test case
    MApplication *app;
    //! MAppletHandle instance under testing.
    MTestAppletHandle *handle;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testInitialization();
    void testAppletIconMessageReceived();
    void testAppletTitleMessageReceived();
    void testAppletTextMessageReceived();

signals:
    void messageReceived(const MAppletMessage &message);
    void connectionFromRunnerEstablished();
    void visibilityChanged();
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void operationProgress(const QString &operation, const QString &pkg, int);
};
#endif // UT_MEXTENSIONHANDLE
