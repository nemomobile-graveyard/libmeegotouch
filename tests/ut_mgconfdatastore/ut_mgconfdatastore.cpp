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

#include "ut_mgconfdatastore.h"
#include <mgconfdatastore.h>

#include <mgconfitem.h>
#include <QtTest/QtTest>

// MGConfItem stubs
struct MGConfItemPrivate {
    QString key;
    QVariant value;
};

QMultiHash<QString, MGConfItem *> gMGConfItems;
QList<QString> gUnsetMGconfItems;

MGConfItem::MGConfItem(const QString &key, QObject *parent)
    : QObject(parent)
{
    priv = new MGConfItemPrivate;
    priv->key = key;

    // Set default value
    if (priv->key == "/gconf/key") {
        priv->value = QVariant("value");
    } else if (priv->key == "/gconf/key1") {
        priv->value = QVariant("value1");
    } else if (priv->key == "/gconf/key2") {
        priv->value = QVariant("value2");
    }

    gMGConfItems.insert(key, this);
}

MGConfItem::~MGConfItem()
{
    gMGConfItems.remove(priv->key, this);
    delete priv;
}

QList<QString> MGConfItem::listEntries() const
{
    QList<QString> entries;
    entries.append("/gconf/key");
    entries.append("/gconf/key1");
    entries.append("/gconf/key2");

    return entries;
}

QVariant MGConfItem::value() const
{
    return priv->value;
}

void MGConfItem::set(const QVariant &val)
{
    priv->value = val;
}

void MGConfItem::unset()
{
    gUnsetMGconfItems.append(priv->key);
    set(QVariant());
}

// Test class
void Ut_MGConfDataStore::initTestCase()
{
}

void Ut_MGConfDataStore::cleanupTestCase()
{
}

void Ut_MGConfDataStore::init()
{
    stringVariantSlotArguments.clear();
    gMGConfItems.clear();
    gUnsetMGconfItems.clear();

    m_subject = new MGConfDataStore("/gconf");
}

void Ut_MGConfDataStore::cleanup()
{
    delete m_subject;
    m_subject = NULL;

    // Check after deleting the test subject that it didn't leave any memory reserved
    QCOMPARE(gMGConfItems.count(), 0);
}

void Ut_MGConfDataStore::testReading()
{
    QCOMPARE(m_subject->value("key").toString(), QString("value"));
}

void Ut_MGConfDataStore::testWriting()
{
    QCOMPARE(m_subject->setValue("key", "newValue"), true);
    QCOMPARE(m_subject->value("key").toString(), QString("newValue"));
    QCOMPARE(m_subject->createValue("foo", "bar"), true);
    QCOMPARE(m_subject->value("foo").toString(), QString("bar"));
}

void Ut_MGConfDataStore::testReadingNonExistingKey()
{
    // Try to read from an empty container
    QCOMPARE(m_subject->value("nonexisting"), QVariant(QVariant::Invalid));

    // Add something to the container but try to read a different key
    QCOMPARE(m_subject->value("nonexisting"), QVariant(QVariant::Invalid));
}

void Ut_MGConfDataStore::testWritingNonExistingKey()
{
    // Try to write to an empty container
    QCOMPARE(m_subject->setValue("nonexisting", "newValue"), false);

    // Add something to the container but try to write a different key
    QCOMPARE(m_subject->setValue("nonexisting", "newValue"), false);
}

void Ut_MGConfDataStore::testSignalReceivedWhenValueChanges()
{
    // Set a new value for the item
    MGConfItem *gconfItem = gMGConfItems.value("/gconf/key");
    gconfItem->set(QString("newValue"));

    // Set up signals
    connect(this, SIGNAL(valueChanged()), gconfItem, SIGNAL(valueChanged()));
    connect(m_subject, SIGNAL(valueChanged(QString, QVariant)), this, SLOT(stringVariantSlot(QString, QVariant)));
    emit valueChanged();

    QCOMPARE(stringVariantSlotArguments.count(), 1);
    {
        const QPair<QString, QVariant>& arguments = stringVariantSlotArguments.at(0);
        QCOMPARE(arguments.first, QString("key"));
        QCOMPARE(arguments.second.type(), QVariant::String);
        QCOMPARE(arguments.second.toString(), QString("newValue"));
    }

    // Add a second key, change the first and check that the correct information is signaled
    gconfItem->set(QString("secondNewValue"));
    stringVariantSlotArguments.clear();
    emit valueChanged();

    QCOMPARE(stringVariantSlotArguments.count(), 1);
    {
        const QPair<QString, QVariant>& arguments = stringVariantSlotArguments.at(0);
        QCOMPARE(arguments.first, QString("key"));
        QCOMPARE(arguments.second.type(), QVariant::String);
        QCOMPARE(arguments.second.toString(), QString("secondNewValue"));
    }

    // Now change the second value and check that the correct information is signaled
    disconnect(this, SIGNAL(valueChanged()), gconfItem, SIGNAL(valueChanged()));
    gconfItem = gMGConfItems.value("/gconf/key1");
    gconfItem->set(QString("newValue1"));
    stringVariantSlotArguments.clear();
    // Set up signals
    connect(this, SIGNAL(valueChanged()), gconfItem, SIGNAL(valueChanged()));
    emit valueChanged();

    QCOMPARE(stringVariantSlotArguments.count(), 1);
    {
        const QPair<QString, QVariant>& arguments = stringVariantSlotArguments.at(0);
        QCOMPARE(arguments.first, QString("key1"));
        QCOMPARE(arguments.second.type(), QVariant::String);
        QCOMPARE(arguments.second.toString(), QString("newValue1"));
    }
}

void Ut_MGConfDataStore::stringVariantSlot(const QString &string, const QVariant &variant)
{
    stringVariantSlotArguments.append(qMakePair(string, variant));
}

void Ut_MGConfDataStore::testAllKeys()
{
    QStringList keys = m_subject->allKeys();
    QCOMPARE(keys.count(), 3);
    QVERIFY(keys.contains("key"));
    QVERIFY(keys.contains("key1"));
    QVERIFY(keys.contains("key2"));
}

void Ut_MGConfDataStore::testRemove()
{
    QVERIFY(m_subject->setValue("key", "newValue"));
    m_subject->remove("key");
    QCOMPARE(m_subject->value("key"), QVariant(QVariant::Invalid));
}

void Ut_MGConfDataStore::testClear()
{
    QCOMPARE(gMGConfItems.count(), 3);
    m_subject->clear();
    QCOMPARE(gMGConfItems.count(), 0);
}

void Ut_MGConfDataStore::testContains()
{
    QVERIFY(m_subject->contains("key"));
    QVERIFY(!m_subject->contains("bar"));
}

void Ut_MGConfDataStore::testSubPathsNotSupported()
{
    QVERIFY(!m_subject->setValue("key/subkey", 123));
    QVERIFY(!m_subject->createValue("key/subkey", 123));
}


QTEST_APPLESS_MAIN(Ut_MGConfDataStore)
