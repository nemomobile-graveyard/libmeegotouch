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

#ifndef FT_PRESTART_H
#define FT_PRESTART_H

#include <QtTest/QtTest>
#include <QObject>

#include <MApplicationService>

Q_DECLARE_METATYPE(MApplicationService *);

class MApplication;
class MApplicationWindow;

#define MAX_PARAMS 10

class Ft_Prestart : public QObject
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
    static QString registeredService;
    static char   *argv[MAX_PARAMS];
    static int     argc;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void PrestartLaunchTerminateOnClose();
    void closePrestartedLazyShutdownApplication();
    void exitPrestartedLazyShutdownApplication();
    void closePrestartedNoPrestartApplication();
    void exitPrestartedNoPrestartApplication();
    void closePrestartedTerminateOnCloseApplication();
    void exitPrestartedTerminateOnCloseApplication();

    void testPrestartedNoPrestartShow();
    void testPrestartedLazyShutdownShow();
    void testPrestartedTerminateOnCloseShow();
    void testPrestartedNoPrestartClose();
    void testPrestartedLazyShutdownClose();
    void testPrestartedTerminateOnCloseClose();

private:
    void initGlobals();
    void buildPrestartedApp();
    MApplicationWindow *m_mWin;
    MApplicationService *m_subject;
    MApplication *m_mApp;
};

#endif
