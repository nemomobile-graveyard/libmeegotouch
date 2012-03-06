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

#ifndef UT_MPOPUPLISTITEM_H
#define UT_MPOPUPLISTITEM_H

#include <QtTest/QtTest>
#include <QObject>

class MPopupListItem;
class MApplication;

class Ut_MPopupListItem : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSetTitle_data();
    void testSetTitle();
    void testSetTitleForeground_data();
    void testSetTitleForeground();

private:
    MPopupListItem *subject;
    MApplication *app;
};

#endif
