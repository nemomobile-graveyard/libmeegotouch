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

#ifndef UT_MLABEL_H
#define UT_MLABEL_H


#include "mlabel.h"
#include <QtTest/QtTest>
#include <QImage>

class TestMLabel;

class TestMLabel: public MLabel
{
    Q_OBJECT

public:
    TestMLabel();
    virtual ~TestMLabel();

    void simulateClick(const QPointF &pos = QPointF(0, 0));
    QImage getLabelImage();
};


class Ut_MLabel : public QObject
{
    Q_OBJECT

private:

    void anchorHitTest(const QPointF &point, int expectedHits);
    QImage captureImage(MLabel *label);

    //Minumum-size rectangle that containts all non-transparent 
    //pixels in the image
    QRect contentRect(const QImage& image);

    //Height of the painted area in the image
    int contentHeight(const QImage& image);

    //Width of the painted area in the image
    int contentWidth(const QImage& image);
    

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

    //regression test for bug  159042  MLabel: Unable to handle strings in presence of certain characters
    void linefeedBeforeFirstTag();

    //regression test for bug 178905 regexp matching an empty string causes an infinite loop in MLabelHighlighter
    void highlighterEmptyRegexp();

    void removeAllHighlighters();

    void wrapModes_data();
    void wrapModes();

private:
    TestMLabel *label;
};

#endif
