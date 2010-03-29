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

#ifndef UT_DUISERVICEFWGEN_H
#define UT_DUISERVICEFWGEN_H

#include <QtTest/QtTest>
#include <DuiApplication>
#include <QObject>

#define MAX_PARAMS 10
class Ut_DuiServiceFwGen: public QObject
{
    Q_OBJECT

public:

private:
    DuiApplication *app;
    DuiApplication *buildApp();

private slots:
    void init();
    void cleanup();

    void testServiceFwMethods();
};

#endif
