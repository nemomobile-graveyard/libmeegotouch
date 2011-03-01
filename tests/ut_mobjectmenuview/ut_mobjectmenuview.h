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

#ifndef UT_MOBJECTMENUVIEW_H
#define UT_MOBJECTMENUVIEW_H

#include <QtTest/QtTest>
#include <QObject>

#include <mobjectmenuview.h>

class MObjectMenu;
class MTheme;

class Ut_MObjectMenuView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testSetView();
    void testTitleAndIcon();
    void testActions();

private:
    MObjectMenuView *m_subject;
    MObjectMenu *m_menu;
    MTheme *m_theme;
};

#endif
