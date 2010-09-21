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

#ifndef FT_THEME_H
#define FT_THEME_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>

class IMThemeDaemon;
class MComponentData;
class TestThemeDaemon;

class Ft_Theme : public QObject
{
    Q_OBJECT;

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    // Test theme reinit with new application name.
    // That have to be the first test case because
    // it sets the application name used in other tests.
    void testThemeReinit();

    // test view creation
    void testViews_data();
    void testViews();

    // test style creation
    void testStyles_data();
    void testStyles();

    // test pixmap loading
    void testPixmaps_data();
    void testPixmaps();

    // test scalable image loading
    void testScalables_data();
    void testScalables();

    // test palette
    void testPalette_data();
    void testPalette();

    // test default fonts
    void testFonts_data();
    void testFonts();

private:
    MComponentData *componentData;
    IMThemeDaemon *daemon;
    TestThemeDaemon *testDaemon;
};

#endif

