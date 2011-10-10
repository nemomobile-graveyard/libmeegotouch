/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef FT_MSTRINGSEARCH_H
#define FT_MSTRINGSEARCH_H

#include <QtTest/QtTest>
#include <QObject>
#include <Qt>
#include <QTextCodec>

#include <MApplication>

#include <MStringSearch>

Q_DECLARE_METATYPE(QList<int>);
Q_DECLARE_METATYPE(MBreakIterator::Type);
Q_DECLARE_METATYPE(MLocale::CollatorStrength);

class Ft_MStringSearch : public QObject
{
    Q_OBJECT

private:
    MApplication *qap;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testConstructors_data();
    void testConstructors();

    void testSearch_data();
    void testSearch();
};

#endif
