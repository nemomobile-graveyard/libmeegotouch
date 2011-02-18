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

#ifndef UT_MAPPLICATIONMENUVIEW_H
#define UT_MAPPLICATIONMENUVIEW_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/MApplicationMenuView class declaration
#include <mapplicationmenuview.h>

Q_DECLARE_METATYPE(MApplicationMenuView *);

class MApplicationMenu;

class Ut_MApplicationMenuView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testSetView();
    void testActionsModel();

private:
    MApplicationMenuView *m_subject;
    MApplicationMenu *m_menu;
};

#endif
