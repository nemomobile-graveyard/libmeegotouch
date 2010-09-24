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

#ifndef UT_MBANNER_H
#define UT_MBANNER_H

#include <QtTest/QtTest>
#include <QObject>
#include <QDateTime>
#include "mbanner.h"

class Ut_MBanner : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testTitle();
    void testSubTitle();
    void testIcon();
    void testTimeStamp();
    void testBannerCreation();
    void testBannerClicking();

private:
    MBanner *m_subject;
    MBanner *m_tmp;
};

#endif
