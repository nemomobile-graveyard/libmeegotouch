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

#include <maggregatedataaccess.h>
#include <mdataaccess.h>
#include "ut_maggregatedataaccess.h"
#include "../stubs/mockdatastore.h"

void Ut_MAggregateDataAccess::init()
{
    primaryAccess = new MockDataStore;
    secondaryAccess = new MockDataStore;
    testAccess = new MAggregateDataAccess(*primaryAccess, *secondaryAccess);
}

void Ut_MAggregateDataAccess::cleanup()
{
    valueChangedSignals.clear();
    delete testAccess;
    delete primaryAccess;
    delete secondaryAccess;
}

void Ut_MAggregateDataAccess::testAccessValueChanged(const QString &key, const QVariant &value)
{
    valueChangedSignals.append(QPair<QString, QVariant>(key, value));
}

void Ut_MAggregateDataAccess::testSettingValueInPrimaryAccess()
{
    primaryAccess->createValue(QString("primaryKey"), QVariant(1));
    testAccess->setValue(QString("primaryKey"), QVariant(2));

    QCOMPARE(primaryAccess->value(QString("primaryKey")), QVariant(2));
}

void Ut_MAggregateDataAccess::testSettingValueInSecondaryAccess()
{
    secondaryAccess->createValue(QString("secondaryKey"), QVariant(1));
    testAccess->setValue(QString("secondaryKey"), QVariant(2));

    QCOMPARE(secondaryAccess->value(QString("secondaryKey")), QVariant(2));
}

void Ut_MAggregateDataAccess::testSettingValueDefinedInPrimaryAndSecondaryAccess()
{
    primaryAccess->createValue(QString("sharedKey"), QVariant(1));
    secondaryAccess->createValue(QString("sharedKey"), QVariant(1));
    testAccess->setValue(QString("sharedKey"), QVariant(2));

    QCOMPARE(primaryAccess->value(QString("sharedKey")), QVariant(2));
    QCOMPARE(secondaryAccess->value(QString("sharedKey")), QVariant(1));
}

void Ut_MAggregateDataAccess::testGettingAggregateKeyList()
{
    primaryAccess->createValue(QString("primaryKey"), QVariant(1));
    secondaryAccess->createValue(QString("secondaryKey"), QVariant(1));

    QCOMPARE(testAccess->allKeys().count(), 2);
    QCOMPARE(testAccess->allKeys().contains("primaryKey"), QBool(true));
    QCOMPARE(testAccess->allKeys().contains("secondaryKey"), QBool(true));
}

void Ut_MAggregateDataAccess::testGettingAggregateKeyListWithoutDuplicates()
{
    primaryAccess->createValue(QString("sharedKey"), QVariant(1));
    secondaryAccess->createValue(QString("sharedKey"), QVariant(2));

    QCOMPARE(testAccess->allKeys().count(), 1);
    QCOMPARE(testAccess->allKeys().contains("sharedKey"), QBool(true));
}

void Ut_MAggregateDataAccess::testContainsKeyInPrimaryAccess()
{
    primaryAccess->createValue(QString("primaryKey"), QVariant(1));

    QCOMPARE(testAccess->contains(QString("primaryKey")), true);
    QCOMPARE(testAccess->contains(QString("secondaryKey")), false);
}

void Ut_MAggregateDataAccess::testContainsKeyInSecondaryAccess()
{
    secondaryAccess->createValue(QString("secondaryKey"), QVariant(1));

    QCOMPARE(testAccess->contains(QString("primaryKey")), false);
    QCOMPARE(testAccess->contains(QString("secondaryKey")), true);
}

void Ut_MAggregateDataAccess::testContainsKeyDefinedInPrimaryAndSecondaryAccess()
{
    primaryAccess->createValue(QString("sharedKey"), QVariant(1));
    secondaryAccess->createValue(QString("sharedKey"), QVariant(2));

    QCOMPARE(testAccess->contains(QString("sharedKey")), true);
}

void Ut_MAggregateDataAccess::testAcquiringValueFromPrimaryAccess()
{
    primaryAccess->createValue(QString("primaryKey"), QVariant(3));

    QCOMPARE(testAccess->value(QString("primaryKey")), QVariant(3));
}

void Ut_MAggregateDataAccess::testAcquiringValueFromSecondaryAccess()
{
    secondaryAccess->createValue(QString("secondaryKey"), QVariant(3));

    QCOMPARE(testAccess->value(QString("secondaryKey")), QVariant(3));
}

void Ut_MAggregateDataAccess::testAcquiringValueDefinedInPrimaryAndSecondaryAccess()
{
    primaryAccess->createValue(QString("sharedKey"), QVariant(3));
    secondaryAccess->createValue(QString("sharedKey"), QVariant(2));

    QCOMPARE(testAccess->value(QString("sharedKey")), QVariant(3));
}

void Ut_MAggregateDataAccess::testValueChangedEmittedFromPrimaryAccess()
{
    connect(testAccess, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(testAccessValueChanged(QString, QVariant)));
    primaryAccess->createValue(QString("primaryKey"), QVariant(3));

    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("primaryKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 3);
    valueChangedSignals.clear();

    primaryAccess->setValue(QString("primaryKey"), QVariant(5));

    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("primaryKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 5);
}

void Ut_MAggregateDataAccess::testValueChangedEmittedFromSecondaryAccess()
{
    connect(testAccess, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(testAccessValueChanged(QString, QVariant)));
    secondaryAccess->createValue(QString("secondaryKey"), QVariant(2));

    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("secondaryKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 2);
    valueChangedSignals.clear();

    secondaryAccess->setValue(QString("secondaryKey"), QVariant(8));

    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("secondaryKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 8);
}

void Ut_MAggregateDataAccess::testValueChangedEmittedFromSharedKey()
{
    connect(testAccess, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(testAccessValueChanged(QString, QVariant)));

    secondaryAccess->createValue(QString("sharedKey"), QVariant(2));
    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("sharedKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 2);
    valueChangedSignals.clear();

    primaryAccess->createValue(QString("sharedKey"), QVariant(4));
    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("sharedKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 4);
    valueChangedSignals.clear();

    secondaryAccess->setValue(QString("sharedKey"), QVariant(3));
    QCOMPARE(valueChangedSignals.count(), 0);

    primaryAccess->setValue(QString("sharedKey"), QVariant(7));
    QCOMPARE(valueChangedSignals.count(), 1);
    QCOMPARE(valueChangedSignals.at(0).first, QString("sharedKey"));
    QCOMPARE(valueChangedSignals.at(0).second.toInt(), 7);
}

QTEST_MAIN(Ut_MAggregateDataAccess)
