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

#ifndef UT_MSETTINGSLANGUAGEFACTORY_H
#define UT_MSETTINGSLANGUAGEFACTORY_H

#include <QObject>

class MSettingsLanguageSettings;
class MDataStore;
class MApplication;

class Ut_MSettingsLanguageWidgetFactory : public QObject
{
    Q_OBJECT

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test creation of children
    void testChildrenCreation();

private:
    MApplication *app;
};

#endif // UT_MSETTINGSLANGUAGEFACTORY_H
