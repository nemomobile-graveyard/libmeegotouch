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

#ifndef UT_DUIAPPLICATION_H
#define UT_DUIAPPLICATION_H

#include <QtTest/QtTest>
#include <DuiApplication>
#include <QObject>

class DuiApplicationService;

#define MAX_PARAMS 10
class Ut_DuiApplication: public QObject
{
    Q_OBJECT

public:
    static QString registeredService;
    static bool m_releasePrestart;
    static bool m_restorePrestart;

private:
    DuiApplication *app;
    DuiApplication *buildApp(int count, const QString &params, const QString &appId = QString(), DuiApplicationService *service = 0);
    DuiApplication *buildApp(int count, const QString &params, DuiApplicationService *service);
    // Build application used in prestart API tests.
    DuiApplication *buildPrestartApp(int count, const QString &params, DuiApplicationService *service = 0);
    QDir generateThemeDirectory();
    QDir generateApplicationThemeDirectory(const QString &applicationName);

    // Helper flags
    bool m_prestartReleased;
    bool m_prestartRestored;

private slots:
    void init();
    void cleanup();

    void duiApplicationConstructor_data();
    void duiApplicationConstructor();
    void duiApplicationConstructorWithAppId();
    void duiApplicationConstructorWithService();

    void duiApplicationTranslationPath();
    void duiApplicationNoFeedback();

    //void testActiveApplicationWindow();
    //void testApplicationWindows();
    void testPrestartMode();
    void testReleasePrestartModeLazyShutdown();
    void testReleasePrestartModeTerminateOnClose();
    void testRestorePrestartModeLazyShutdown();
    void testRestorePrestartModeTerminateOnClose();
    void testIsPrestartedNoPrestart();
    void testIsPrestartedTerminateOnClose();
    void testIsPrestartedLazyShutdown();

public slots:

    // Test slots for prestart signals
    void prestartReleased();
    void prestartRestored();

#if 0
    void duiApplicationName();
    void themeFileLoadingFromCurrentPath();
    void themeFileLoadingFromApplicationThemePath();
    void themeFileLoadingFromGlobalThemePath();
#endif
};

#endif
