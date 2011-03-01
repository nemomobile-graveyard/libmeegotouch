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

#ifndef FT_MSTYLESHEET_H
#define FT_MSTYLESHEET_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <mstylesheet.h>
#include <mtheme.h>
#include <mapplication.h>

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


class Ft_MStyleSheet : public QObject
{
    Q_OBJECT
public:
    Ft_MStyleSheet();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void test_supported_attribute_types_data();
    void test_supported_attribute_types();
    void test_inheritance_data();
    void test_inheritance();
    void test_objectnames_data();
    void test_objectnames();
    void test_orientations_data();
    void test_orientations();
    void test_modes_data();
    void test_modes();
    void test_types_data();
    void test_types();
    void test_parent_data();
    void test_parent();
    void test_parent_stylenames_data();
    void test_parent_stylenames();
    void test_wrong_attribute_value_strict();
    void test_wrong_attribute_value_relaxed();
    void test_wrong_attribute_value_with_parent();
    void test_wrong_syntax();

    //void test_cache_size();

private:
    void loadTestObjectSheets(bool testThemeInheritance);

    MStyleSheet      *m_subject;
    //MStyleDescription m_styleDescription;
};

#endif

