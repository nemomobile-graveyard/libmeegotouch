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

#ifndef UT_DUILABEL_H
#define UT_DUILABEL_H


#include "duilabel.h"
#include <QtTest/QtTest>
#include <QImage>

class TestDuiLabel;

class TestDuiLabel: public DuiLabel
{
    Q_OBJECT

public:
    TestDuiLabel();
    virtual ~TestDuiLabel();

    void simulateClick(const QPointF &pos = QPointF(0, 0));
    QImage getLabelImage();
};


class Ut_DuiLabel : public QObject
{
    Q_OBJECT

private:

    void anchorHitTest(const QPointF &point, int expectedHits);
    QImage captureImage(DuiLabel *label);

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testTextAlignment_data();
    void testLayoutDirection_data();
    void testTextWordWrap_data();
    void testTextElide_data();
    void testClear_data();
    void testPlainTextColor_data();

    void testTestConstruction();
    void testTextAlignment();
    void testLayoutDirection();
    void testTextWordWrap();
    void testTextElide();

    void testHighlighting();

    void testAnchor();

    void testClear();
    void testPlainTextColor();

    void testSizeHint_data();
    void testSizeHint();

    void testFont_data();
    void testFont();

    void testColor_data();
    void testColor();

    //regression test for bug  159042  DuiLabel: Unable to handle strings in presence of certain characters
    void linefeedBeforeFirstTag();


private:
    TestDuiLabel *label;
};

#endif
