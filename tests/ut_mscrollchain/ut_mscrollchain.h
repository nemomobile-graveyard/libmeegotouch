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

#ifndef UT_MSCROLLCHAIN_H
#define UT_MSCROLLCHAIN_H

#include <QtTest/QtTest>
#include <QObject>
#include <memory>
#include "mapplication.h"

class MScrollChain;
class MSceneWindow;

class QGraphicsScene;
class QGraphicsWidget;

class Ut_MScrollChain : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    void testPointScroll_data();
    void testPointScroll();
    void testRestore();
    void testMultiplePointScrolls_data();
    void testMultiplePointScrolls();

private:
    void addBottomUpScroll(QGraphicsWidget *widget, const QPoint &offset);

private:
    std::auto_ptr<MApplication> app;
    std::auto_ptr<MScrollChain> subject;
    std::auto_ptr<QGraphicsScene> scene;
    QGraphicsWidget *rootElement;
};

#endif // UT_MSCROLLCHAIN_H
