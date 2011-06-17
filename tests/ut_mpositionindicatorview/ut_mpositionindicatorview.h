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

#ifndef UT_MPOSITIONINDICATORVIEW_H
#define UT_MPOSITIONINDICATORVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <QtTest/QtTest>
#include <QString>

#include <mpositionindicator.h>
#include <mpositionindicatorview.h>

Q_DECLARE_METATYPE(MPositionIndicatorView *);

class Ut_MPositionIndicatorView : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testIndicatorDisappearsWhenItGetsOffDisplay_data();
    void testIndicatorDisappearsWhenItGetsOffDisplay();

    void testIndicatorAppearsWhenItGetsOnDisplay_data();
    void testIndicatorAppearsWhenItGetsOnDisplay();

    void testNoActivityInSwitcher_data();
    void testNoActivityInSwitcher();

private:
    MPositionIndicator *m_controller;
    MPositionIndicatorView *m_subject;
};


#endif
