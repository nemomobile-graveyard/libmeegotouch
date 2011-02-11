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

#ifndef UT_MPANNABLEVIEWPORT_H
#define UT_MPANNABLEVIEWPORT_H

#include <QtTest/QtTest>

class MPannableViewport;

class Ut_MPannableViewport : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void constructor();
    void setWidget();
    void setGeometry_data();
    void setGeometry();
    void event_data();
    void event();
    void updatePosition();
    void updateSamePosition();
    void sizePosChangedAfterPopulatingPannedWidget();

    //Changing position indicator in the viewport
    void settingNewPositionIndicator();
    void settingNULLPositionIndicatorShouldNotBeAccepted();

    void disabledViewportShouldNotAllowWigetToBeBiggerThanViewport();
    void rangeOfViewportShouldBeSizeOfPannedWidgetMinusViewportSize();

    //See NB#226518 or NB#217727
    void testPositionIndicatorPositionAfterReenabled();
    void testPositionIndicatorRangeAfterReenabled();
    void testPositionIndicatorViewportSizeAfterReenabled();

    void testSetClipping();
    void testRangeSetting();
    void testExtendedRange_data();
    void testExtendedRange();
    void testSizeHint_data();
    void testSizeHint();

private:
    MPannableViewport *subject;
};

#endif
