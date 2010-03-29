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

#ifndef UT_DUIWINDOW_H
#define UT_DUIWINDOW_H

#include <QtTest>
#include <QObject>
#include <DuiNamespace>

class DuiWindow;
class DuiApplication;

#define MAX_PARAMS 10
class Ut_DuiWindow: public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testNoSceneManager();
    void testNoScene();
    void testSceneManagerAutocreation();
    void testIfSceneExistsWhenSceneManagerAutocreated();
    void testConstructorWithSceneManager();
    void testSetSceneManager();
    void testOrientation_data();
    void testOrientation();
    void testSetOrientationAngle_data();
    void testSetOrientationAngle();
    void testVisibleSceneSize_data();
    void testVisibleSceneSize();
    void testOrientationChangedSignalPropagationFromSceneManager();
    void testNoOrientationChangedSignalWhenRotatingBy180Degrees();
    void testIsOnDisplay();
    void testEnterDisplayEventHandler();
    void testExitDisplayEventHandler();
    void testDisplayEnteredSignal();
    void testDisplayExitedSignal();
    void testDisplayExitedOnClose();
    void testDisplayExitedOnCloseLazyShutdownApp();
    void testCloseOnLazyShutdown();

public slots:
    void onDisplayTestSlot();

public:
    static bool m_onDisplayHandlerCalled;
    static bool m_onDisplaySignalSent;
};

Q_DECLARE_METATYPE(Dui::Orientation);
Q_DECLARE_METATYPE(Dui::OrientationAngle);

#endif
