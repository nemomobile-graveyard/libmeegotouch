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

#ifndef UT_DUILINEARLAYOUTPOLICY_H
#define UT_DUILINEARLAYOUTPOLICY_H

#include <QtTest/QtTest>
#include <QObject>

class QGraphicsLayoutItem;
class QGraphicsWidget;
class DuiLayout;
class DuiLinearLayoutPolicy;

class Ut_DuiLinearLayoutPolicy : public QObject
{
    Q_OBJECT

public:
    Ut_DuiLinearLayoutPolicy();
    virtual ~Ut_DuiLinearLayoutPolicy();

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testStretch_data();
    void testStretch();
    void testSpacing();
    void testInsertingItems();

    void init();
    void cleanup();
private:

    DuiLayout *m_mockLayout;
    DuiLinearLayoutPolicy *m_policy;
    QGraphicsLayoutItem *m_mockItem100;
    QGraphicsLayoutItem *m_mockItem200;
    QGraphicsLayoutItem *m_mockItem300;
    QGraphicsLayoutItem *m_mockItem400;
    QGraphicsWidget *m_form;
};

#endif // Header guard
