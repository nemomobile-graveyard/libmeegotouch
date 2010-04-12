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

#ifndef UT_MAPPLICATION_H
#define UT_MAPPLICATION_H

#include <QtTest/QtTest>
#include <MApplication>
#include <QObject>

class MApplicationService;

#define MAX_PARAMS 10
class Ut_MApplication: public QObject
{
    Q_OBJECT

public:
    static QString registeredService;
    static bool m_releasePrestart;
    static bool m_restorePrestart;

private:
    MApplication *app;
    MApplication *buildApp(int count, const QString &params, const QString &appId = QString(), MApplicationService *service = 0);
    MApplication *buildApp(int count, const QString &params, MApplicationService *service);
    // Build application used in prestart API tests.
    MApplication *buildPrestartApp(int count, const QString &params, MApplicationService *service = 0);
    QDir generateThemeDirectory();
    QDir generateApplicationThemeDirectory(const QString &applicationName);

    // Helper flags
    bool m_prestartReleased;
    bool m_prestartRestored;

private slots:
    void init();
    void cleanup();

    void mApplicationConstructor_data();
    void mApplicationConstructor();
    void mApplicationConstructorWithAppId();
    void mApplicationConstructorWithService();

    void mApplicationTranslationPath();
    void mApplicationNoFeedback();

    //void testActiveApplicationWindow();
    //void testApplicationWindows();
    void testPrestartMode();
    void testReleasePrestartModeLazyShutdown();
    void testReleasePrestartModeTerminateOnClose();
    void testReleasePrestartModeLazyShutdownMultiWindow();
    void testReleasePrestartModeTerminateOnCloseMultiWindow();
    void testRestorePrestartModeLazyShutdown();
    void testRestorePrestartModeTerminateOnClose();
    void testRestorePrestartModeLazyShutdownMultiWindow();
    void testRestorePrestartModeTerminateOnCloseMultiWindow();
    void testIsPrestartedNoPrestart();
    void testIsPrestartedTerminateOnClose();
    void testIsPrestartedLazyShutdown();
    void testIsPrestartedTerminateOnCloseMultiWindow();
    void testIsPrestartedLazyShutdownMultiWindow();
    void testPrestartedProperty();
public slots:

    // Test slots for prestart signals
    void prestartReleased();
    void prestartRestored();

#if 0
    void mApplicationName();
    void themeFileLoadingFromCurrentPath();
    void themeFileLoadingFromApplicationThemePath();
    void themeFileLoadingFromGlobalThemePath();
#endif
};

#endif
