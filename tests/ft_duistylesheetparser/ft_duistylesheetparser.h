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

#ifndef FT_CSS_H
#define FT_CSS_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>
#include <duistylesheetparser.h>
#include <duitheme.h>
#include <duiapplication.h>

class DuiLogicalValues;

class Ft_DuiStyleSheetParser : public QObject
{
    Q_OBJECT
public:
    Ft_DuiStyleSheetParser();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void test_load();
    void test_import();
    void test_constants();
    void test_constants_binary();
    void test_binary_equality();
    void test_parser_speed();
    void test_binary_speed();

    /*void test_inheritance();
    void test_objectnames();
    void test_orientations();
    void test_modes();
    void test_types();
    void test_complex();


    void test_cache_size();
    */

private:

    DuiLogicalValues         *m_logicalValues;
    DuiStyleSheetParser      *m_subject;
};

#endif

