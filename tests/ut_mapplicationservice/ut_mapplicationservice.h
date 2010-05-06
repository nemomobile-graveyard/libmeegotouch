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

#ifndef UT_MAPPLICATIONSERVICE_H
#define UT_MAPPLICATIONSERVICE_H

#include <QtTest/QtTest>
#include <QObject>

#include <MApplicationService>

Q_DECLARE_METATYPE(MApplicationService *);

class MApplication;

#define MAX_PARAMS 10

class Ut_MApplicationService : public QObject
{
    Q_OBJECT

public:
    // mock control/report variables
    static bool    activeWindowSet;
    static bool    windowRaised;
    static bool    windowActivated;
    static bool    windowShown;
    static bool    windowClosed;
    static bool    applicationExited;
    static bool    allowRegisterService;
    static bool    allowRegisterService2;
    static int     failRegisterServiceTimes;
    static int     failRegisterServiceCounter;
    static bool    programStarted;
    static bool    programArgsCorrect;
    static bool    prestartReleased;
    static bool    prestartRestored;
    static bool    isPrestartedReturnValue;
    static bool    isLazyShutdownReturnValue;
    static QString registeredService;
    static char   *argv[MAX_PARAMS];
    static int     argc;
    static QString     appName;
    static QStringList arguments;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void plainLaunch();
    void incrementAndRegisterTenFail();
    void launchWithRegistrationFailure();
    void launchAnotherWithQProcess();
    void incrementAndRegisterOneFail();
    void misc();
    void prestartLaunch();
    void prestartLaunchNoPrestart();
    void prestartCloseLazyShutdown();
    void prestartCloseNoLazyShutdown();
    void prestartExitLazyShutdown();
    void prestartExitNoLazyShutdown();

private:
    void buildApp();
    MApplicationService *m_subject;
    MApplication *m_mApp;
};

#endif
