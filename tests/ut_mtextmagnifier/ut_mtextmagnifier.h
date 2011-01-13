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

#ifndef UT_MTEXTMAGNIFIER_H
#define UT_MTEXTMAGNIFIER_H

#include <QObject>
#include <QtTest/QtTest>

class MApplication;
class MApplicationWindow;
class MTextMagnifier;
class MWidget;

class Ut_MTextMagnifier : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testMagnifierConsumesPanGesture_data();
    void testMagnifierConsumesPanGesture();

    void testOverlayGeometry_data();
    void testOverlayGeometry();

    void testOutOfScreen_data();
    void testOutOfScreen();

private:
    MApplication *m_app;
    MApplicationWindow *m_appWindow;
    MTextMagnifier *subject;
    MWidget *magnifiedWidget;
};

#endif

