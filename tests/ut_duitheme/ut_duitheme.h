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

#ifndef UT_DUITHEME_H
#define UT_DUITHEME_H

#include <QObject>
#include <QtTest/QtTest>


#include "duitheme.h"

class Ut_DuiTheme : public QObject
{
    Q_OBJECT

private slots:
    void init();   // Executed before each test function
    void cleanup();  // Executed after each test function
    void initTestCase(); // Executed before all
    void cleanupTestCase(); // Executed after all tests have been run

    // Actual test functions
    void loadCSS();
#if QT_VERSION < 0x040500
    void testPixmapCreationWithAllBitDepths();
#endif // QT_VERSION < 0x040500

private:
    DuiTheme *m_subject;
};
#endif
