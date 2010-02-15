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

#ifndef UT_DUIAPPLICATIONWINDOW
#define UT_DUIAPPLICATIONWINDOW

#include <QObject>
#include <QtTest>
#include <DuiNamespace>

class DuiApplicationWindow;
class DuiApplicationPage;

class Ut_DuiApplicationWindow : public QObject
{
    Q_OBJECT

public:
    static bool m_windowShown;
    static bool m_windowClosed;
    static bool m_prestartRestored;

private slots:
    void init();   // Executed before each test function
    void cleanup();  // Executed after each test function
    void initTestCase(); // Executed before all
    void cleanupTestCase(); // Executed after all tests have been run

    void testConstructorWithoutScene();
    void testConstructorWithScene();
    void testPageChanged();
    void testWindowActivate();
    void testDeleteOnClose();

    void testIsOnDisplay();
    void testPrestartNoPrestart();
    void testPrestartTerminateOnClose();
    void testPrestartLazyShutdown();

    void testDisplayExitedOnClose();
    void testDisplayExitedOnCloseLazyShutdownApp();

private:
    DuiApplicationWindow *m_subject;

};

Q_DECLARE_METATYPE(DuiApplicationPage *);
Q_DECLARE_METATYPE(Dui::OrientationAngle);
Q_DECLARE_METATYPE(Dui::Orientation);

#endif
