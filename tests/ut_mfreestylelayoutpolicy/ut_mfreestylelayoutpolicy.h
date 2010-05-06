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

#ifndef UT_MFREESTYLELAYOUTPOLICY_H
#define UT_MFREESTYLELAYOUTPOLICY_H

#include <QtTest/QtTest>
#include <QObject>

class QGraphicsLayoutItem;
class MLayout;
class MFreestyleLayoutPolicy;
class QGraphicsWidget;

class Ut_MFreestyleLayoutPolicy : public QObject
{
    Q_OBJECT

public:
    Ut_MFreestyleLayoutPolicy();
    virtual ~Ut_MFreestyleLayoutPolicy();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testOneItemLayout();
    void testThreeItemLayout();
    void testThreeItemLayoutWithGivenGeometry();
private:

    MLayout *m_mockLayout;
    MFreestyleLayoutPolicy *m_policy;
    QGraphicsLayoutItem *m_mockItem100;
    QGraphicsLayoutItem *m_mockItem200;
    QGraphicsLayoutItem *m_mockItem300;
    QGraphicsWidget *m_form;
};

#endif // Header guard
