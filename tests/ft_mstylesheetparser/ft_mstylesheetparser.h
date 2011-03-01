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

#ifndef FT_CSS_H
#define FT_CSS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <mstylesheetparser.h>
#include <mtheme.h>
#include <mapplication.h>

class MLogicalValues;

class Ft_MStyleSheetParser : public QObject
{
    Q_OBJECT
public:
    Ft_MStyleSheetParser();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void test_load();
    void test_import();
    void test_constants();
    void test_binary_equality();
    void test_set_binary_file_directory();

    /*void test_inheritance();
    void test_objectnames();
    void test_orientations();
    void test_modes();
    void test_types();
    void test_complex();


    void test_cache_size();
    */

private:

    MLogicalValues         *m_logicalValues;
    MStyleSheetParser      *m_subject;
};

#endif

