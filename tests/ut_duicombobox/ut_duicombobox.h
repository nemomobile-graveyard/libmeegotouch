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

#ifndef UT_DUICOMBOBOX_H
#define UT_DUICOMBOBOX_H

#include <QtTest/QtTest>
#include <QObject>
#include <QStringListModel>
#include <QStringList>

#include <DuiComboBox>

Q_DECLARE_METATYPE(DuiComboBox *);

class Ut_DuiComboBox : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void initTestCase();
    void cleanupTestCase();

    void testItemModel();
    void testActions();
    void testCurrentIndex();
    void testFunctions();
    void testCancelEvent();
    void testClickSlot();
    void testBuiltinModel();
    void testModelSwitching();
    void testStringListModel();
    void testStringListModelSetStringList();
    void testActivatedSignal();
    void testSetCurrentIndex();

private:
    // disabled because of race conditions with DuiTheme daemon
    void testIconVisibility();

    DuiComboBox *m_combobox;
    QStringListModel m_model;
    QStringList m_stringList;
};

#endif
