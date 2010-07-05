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

#ifndef UT_STATUSBARVIEW_H
#define UT_STATUSBARVIEW_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;

// the real unit/MStatusBarView class declaration
#include <mstatusbarview.h>

Q_DECLARE_METATYPE(MStatusBarView *);

class Ut_MStatusBarView : public QObject
{
    Q_OBJECT
public:
    Ut_MStatusBarView();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();
#ifdef Q_WS_X11
//    void testXDamageWhileTogglingVisibility();
//    void testXDamageWhileTogglingProviderAvailability();
#endif
    // Test when mouse is moved over a certain threshold then status indicator menu appears.
    void testWhenMouseMovesAboveThresholdStatusIndicatorMenuAppears();
    // Test when swipe less than a particular amount, status indicator will not be triggered.
    void testWhenSwipeLessThanThresholdStatusIndicatorMenuDoesNotAppear();
    // Test when mouse is moved over a certain threshold then haptics done.
    void testWhenMousePressHapticsDone();

private:
    void mouseDownWorker();
    void mouseMoveworker(QPointF moveTo);
    MStatusBarView *m_subject;
    MStatusBar *m_statusbar;
    MApplication* app;
};

#endif
