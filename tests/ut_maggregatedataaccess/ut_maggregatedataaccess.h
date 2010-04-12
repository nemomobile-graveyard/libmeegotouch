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

#ifndef UT_MAGGREGATEDATAACCESS_H
#define UT_MAGGREGATEDATAACCESS_H

#include <QtTest/QtTest>
#include <QObject>

class MockDataStore;
class MDataAccess;

class Ut_MAggregateDataAccess : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testSettingValueInPrimaryAccess();
    void testSettingValueInSecondaryAccess();
    void testSettingValueDefinedInPrimaryAndSecondaryAccess();
    void testAcquiringValueFromPrimaryAccess();
    void testAcquiringValueFromSecondaryAccess();
    void testAcquiringValueDefinedInPrimaryAndSecondaryAccess();
    void testGettingAggregateKeyList();
    void testGettingAggregateKeyListWithoutDuplicates();
    void testContainsKeyInPrimaryAccess();
    void testContainsKeyInSecondaryAccess();
    void testContainsKeyDefinedInPrimaryAndSecondaryAccess();
    void testValueChangedEmittedFromPrimaryAccess();
    void testValueChangedEmittedFromSecondaryAccess();
    void testValueChangedEmittedFromSharedKey();

    /*!
     * Connected to the valueChanged signal of the testAccess object
     * This method will append the value changed signal contents to the
     * valueChangedSignals list.
     */
    void testAccessValueChanged(const QString &, const QVariant &);

private:
    MockDataStore *primaryAccess;
    MockDataStore *secondaryAccess;
    MDataAccess *testAccess;
    QList< QPair<QString, QVariant> > valueChangedSignals;
};

#endif
