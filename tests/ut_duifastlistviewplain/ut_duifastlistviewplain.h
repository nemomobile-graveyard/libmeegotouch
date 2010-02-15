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

#ifndef UT_DUILABEL_H
#define UT_DUILABEL_H

#include <QtTest/QtTest>
#include <QAbstractListModel>
#include <DuiWidget>
#include "duiabstractcellcreator.h"
const int GroupHeaderHeight = 40;

class DuiFastPlainListViewPrivate;

class Ut_DuiListNewView : public QObject
{
    Q_OBJECT

private:
    void makePhoneBook();
    void makePhoneBookModel();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private:
    DuiFastPlainListViewPrivate *fastListViewPrivate;
};

#endif
