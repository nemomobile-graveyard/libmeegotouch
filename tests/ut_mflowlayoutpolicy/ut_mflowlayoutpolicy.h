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

#ifndef UT_MFLOWLAYOUTPOLICY_H
#define UT_MFLOWLAYOUTPOLICY_H

#include <QtTest/QtTest>
#include <QObject>

class QGraphicsLayoutItem;
class MLayout;
class MFlowLayoutPolicy;
class QGraphicsWidget;

class Ut_MFlowLayoutPolicy : public QObject
{
    Q_OBJECT

public:
    Ut_MFlowLayoutPolicy();
    virtual ~Ut_MFlowLayoutPolicy();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testHorizontalJustification_data();
    void testHorizontalJustification();
    void testReorderingFlow();
    void testOneItemLayout();
    void testTwoItemsHorizontally();
    void testTwoItemsVertically();
    void testHorizontallyAndVertically();
    void testMargins();
    void testMarginsAndTwoItems();
    void testSpacing();
    void testHorizontalVerticalSpacing();
    void testEmptyCount();
    void testTwoByTwoThenRemoveThird_data();
    void testTwoByTwoThenRemoveThird();
    void testBigItemInSmallLayout();
    void testMarginsAndMinimumHeight();
    void testAddingRemovingAdding();
    void testAlignment();
private:

    MLayout *m_mockLayout;
    MFlowLayoutPolicy *m_policy;
    QGraphicsLayoutItem *m_mockItem100;
    QGraphicsLayoutItem *m_mockItem200;
    QGraphicsLayoutItem *m_mockItem300;
    QGraphicsWidget *m_form;
};

#endif // Header guard
