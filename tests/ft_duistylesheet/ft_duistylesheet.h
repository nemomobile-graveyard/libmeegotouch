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

#ifndef FT_DUISTYLESHEET_H
#define FT_DUISTYLESHEET_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <duistylesheet.h>
#include <duitheme.h>
#include <duiapplication.h>

// Test class, derives QObject->TestObject
class TestObject : public QObject
{
    Q_OBJECT

public:
    TestObject(QObject *parent = 0) : QObject(parent) {};
};

// Test class, derives QObject->TestObject->TestObject2
class TestObject2 : public TestObject
{
    Q_OBJECT

public:
    TestObject2(QObject *parent = 0) : TestObject(parent) {};
};


class Ft_DuiStyleSheet : public QObject
{
    Q_OBJECT
public:
    Ft_DuiStyleSheet();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void test_supported_attribute_types();
    void test_inheritance();
    void test_objectnames();
    void test_orientations();
    void test_modes();
    void test_types();
    void test_parent();

    //void test_cache_size();

private:

    DuiStyleSheet      *m_subject;
    //DuiStyleDescription m_styleDescription;
};

#endif

