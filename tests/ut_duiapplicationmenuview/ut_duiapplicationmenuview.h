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

#ifndef UT_DUIAPPLICATIONMENUVIEW_H
#define UT_DUIAPPLICATIONMENUVIEW_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/DuiApplicationMenuView class declaration
#include <duiapplicationmenuview.h>

Q_DECLARE_METATYPE(DuiApplicationMenuView *);

class DuiApplicationMenu;

class Ut_DuiApplicationMenuView : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    // class testing
    void testSetView();

private:
    DuiApplicationMenuView *m_subject;
    DuiApplicationMenu *m_menu;
};

#endif
