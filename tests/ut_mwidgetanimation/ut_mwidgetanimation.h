/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MWIDGETANIMATION_H
#define UT_MWIDGETANIMATION_H

#include <QtTest/QtTest>
#include <QtGui/QMainWindow>
#include "mwidgetanimation.h"
#include "mwidgetanimation_p.h"
#include <MApplication>

#define DURATION 800

class MApplication;

class Ut_MWidgetAnimation : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testAddWidget();
    void testRemoveWidget();
    void testSetDuration();
    void testSetEasingCurve ();
    void testSetTargetValue ();
    void testSetTargetGeometry ();
    void testSetTargetOpacity ();
    void testSetTargetSize ();
    void testSetTargetPosition ();

 private:
    MWidgetAnimation *animation;
    MApplication *app;

};

#endif // UT_MWIDGETANIMATION_H
