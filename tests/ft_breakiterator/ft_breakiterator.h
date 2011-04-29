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

#ifndef FT_BREAKITERATOR_H
#define FT_BREAKITERATOR_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include <QCoreApplication>

#include <mbreakiterator.h>


#define MAX_PARAMS 10

Q_DECLARE_METATYPE(QList<int>);


class Ft_BreakIterator : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *qap;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void constructors_data();
    void constructors();
    void forward_data();
    void forward();
    void backward_data();
    void backward();
    void aroundIndex_data();
    void aroundIndex();
    void peek_data();
    void peek();
};

#endif
