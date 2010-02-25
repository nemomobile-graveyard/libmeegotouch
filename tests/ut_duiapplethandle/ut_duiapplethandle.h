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

#ifndef UT_DUIAPPLETHANDLE
#define UT_DUIAPPLETHANDLE

#include <QObject>
#include "duiapplethandle.h"
#include "duiapplethandle_p.h"
#include "duiapplethandlestyle.h"

class DuiTestAppletHandlePrivate : public DuiAppletHandlePrivate
{
};

class DuiTestAppletHandle : public DuiAppletHandle
{
    Q_DECLARE_PRIVATE(DuiTestAppletHandle)

public:
    DuiTestAppletHandle();
    void messageReceivedFromRunner(const DuiAppletMessage &message);
};

class DuiApplication;

// Test case must inherit QObject
class Ut_DuiAppletHandle : public QObject
{
    Q_OBJECT

private:
    // DUI Application for the test case
    DuiApplication *app;
    //! DuiAppletHandle instance under testing.
    DuiTestAppletHandle *handle;

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
    void messageReceived(const DuiAppletMessage &message);
    void connectionFromRunnerEstablished();
    void widgetVisibilityChanged(bool visible);
    void applicationVisibilityChanged(bool visible);
    void operationComplete(const QString &operation, const QString &pkg, const QString &error);
    void operationProgress(const QString &operation, const QString &pkg, int);
};
#endif // UT_DUIEXTENSIONHANDLE
