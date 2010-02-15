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

#ifndef UT_DUIGRIDLAYOUTPOLICY_H
#define UT_DUIGRIDLAYOUTPOLICY_H

#include <QtTest/QtTest>
#include <QObject>

class QGraphicsLayoutItem;
class QGraphicsWidget;
class DuiLayout;
class DuiGridLayoutPolicy;

class Ut_DuiGridLayoutPolicy : public QObject
{
    Q_OBJECT

public:
    Ut_DuiGridLayoutPolicy();
    virtual ~Ut_DuiGridLayoutPolicy();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testDeletingLayout();
    void testOneItemLayout();
    void testHorizontally();
    void testVertically();
    void testHorizontallyAndVertically();
    void testSpacing();
    void testAlignment();
    void testAlignmentSubLayouts();
    void testRowCount();
private:

    DuiLayout *m_mockLayout;
    DuiGridLayoutPolicy *m_policy;
    QGraphicsLayoutItem *m_mockItem100;
    QGraphicsLayoutItem *m_mockItem200;
    QGraphicsLayoutItem *m_mockItem300;
    QGraphicsLayoutItem *m_mockItem400;
    QGraphicsWidget *m_form;
};

#endif // Header guard
