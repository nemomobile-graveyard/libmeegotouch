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

#ifndef UT_MPANNABLEWIDGET_HH
#define UT_MPANNABLEWIDGET_HH


#include <QtTest/QtTest>
#include <QObject>
#include "mapplication.h"
class MPannableWidget;
class DummyGraphicsItem;

class Ut_MPannableWidget : public QObject
{
    Q_OBJECT

public:

private:

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void usingCustomPhysics();
    void settingNewPhysicsShouldEmitPhysicsChangeSignal();
    void settingPhysicsToNULLShouldNotBreakTheWidget();

    void panGestureMovesPhysicsPointer();
    void panGestureAgainstPanningDirectionStopsPanning();

private:
    MPannableWidget *widget;
};


#endif
