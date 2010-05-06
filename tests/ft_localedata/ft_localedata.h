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

#ifndef FT_LOCALEDATA_H
#define FT_LOCALEDATA_H

#include <QtTest/QtTest>
#include <QObject>
#include <MLocale>

Q_DECLARE_METATYPE(MLocale::CalendarType);
Q_DECLARE_METATYPE(MLocale);


#define MAX_PARAMS 10
class Ft_LocaleData : public QObject
{
    Q_OBJECT

private:
    QCoreApplication *app;

private slots:
    void initTestCase();

    void cleanup();

    void testNumberData_data();
    void testNumberData();
    // void testMFoo();
    // void testMBar_data();
    // void testMBar();
};


#endif
