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

#include "ft_mgconfitem.h"

#define MYLOGLEVEL 2
void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
    case QtDebugMsg:
        if (MYLOGLEVEL <= 0)
            fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        if (MYLOGLEVEL <= 1)
            fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        if (MYLOGLEVEL <= 2)
            fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        if (MYLOGLEVEL <= 3)
            fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }
}

void unsetRecursive(MGConfItem *item) {
    QStringList dirs = item->listDirs();
    foreach(QString dir, dirs) {
        MGConfItem gconfDir(dir);
        unsetRecursive(&gconfDir);
        gconfDir.sync();
    }

    QStringList entries = item->listEntries();
    foreach(QString entry, entries) {
        MGConfItem gconfEntry(entry);
        gconfEntry.unset();
    }
}

//
// Definition of testcases: Normal tests
//

void Ft_MGConfItem::timeout()
{
    timed_out = true;
    timer.stop();
}

// Before all tests
void Ft_MGConfItem::initTestCase()
{
    QProcess process;
    QStringList arguments;
    arguments << "-set-keys";
    process.start(qApp->applicationFilePath(), arguments);
    QVERIFY(process.waitForFinished());
    QCOMPARE(process.exitCode(), 0);

    connect(&timer, SIGNAL(timeout()),
            this, SLOT(timeout()));
}

// After all tests
void Ft_MGConfItem::cleanupTestCase()
{
    // Unset all entries
    MGConfItem rootItem("/Test");
    unsetRecursive(&rootItem);
    rootItem.sync();

    QTest::qWait(500); // sync won't happen immediately -> give some time for it

    QCOMPARE(rootItem.listEntries().count(), 0);
}

// Before each test
void Ft_MGConfItem::init()
{
    boolItem = new MGConfItem("/Test/Bool");
    intItem = new MGConfItem("/Test/Int");
    stringItem = new MGConfItem("/Test/String");
    doubleItem = new MGConfItem("/Test/Double");
    stringListItem = new MGConfItem("/Test/StringList");
    intListItem = new MGConfItem("/Test/IntList");
    doubleListItem = new MGConfItem("/Test/DoubleList");
    boolListItem = new MGConfItem("/Test/BoolList");
    unsetBeforeItem = new MGConfItem("/Test/UnsetBefore");
    unsetAfterItem = new MGConfItem("/Test/UnsetAfter");
    signalSpy = new SignalListener();
    QObject::connect(boolItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(intItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(stringItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(doubleItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(stringListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(intListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(doubleListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::connect(boolListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    // wait a bit do give the gconf daemon time to do whatever he wants to do:
    QTest::qWait(500);
}

// After each test
void Ft_MGConfItem::cleanup()
{
    QObject::disconnect(boolItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(intItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(stringItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(doubleItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(stringListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(intListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(doubleListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    QObject::disconnect(boolListItem, SIGNAL(valueChanged()), signalSpy, SLOT(valueChanged()));
    delete signalSpy;
    delete boolItem;
    delete intItem;
    delete stringItem;
    delete doubleItem;
    delete stringListItem;
    delete intListItem;
    delete doubleListItem;
    delete boolListItem;
    delete unsetBeforeItem;
    delete unsetAfterItem;

    timer.stop();
}

void Ft_MGConfItem::path()
{
    QCOMPARE(boolItem->key(), QString("/Test/Bool"));
    QCOMPARE(intItem->key(), QString("/Test/Int"));
    QCOMPARE(stringItem->key(), QString("/Test/String"));
    QCOMPARE(doubleItem->key(), QString("/Test/Double"));
    QCOMPARE(stringListItem->key(), QString("/Test/StringList"));
    QCOMPARE(intListItem->key(), QString("/Test/IntList"));
    QCOMPARE(doubleListItem->key(), QString("/Test/DoubleList"));
    QCOMPARE(boolListItem->key(), QString("/Test/BoolList"));
    QCOMPARE(unsetBeforeItem->key(), QString("/Test/UnsetBefore"));
    QCOMPARE(unsetAfterItem->key(), QString("/Test/UnsetAfter"));
}

void Ft_MGConfItem::external_values()
{
    // These values are set before this program starts.
    QCOMPARE(boolItem->value().toBool(), true);
    QCOMPARE(intItem->value().toInt(), 123);
    QCOMPARE(stringItem->value().toString(), QString("Hello GConf"));
    QCOMPARE(doubleItem->value().toDouble(), 3.5);
    QCOMPARE(stringListItem->value().toStringList(), QStringList() << "Hello" << "GConf" << QString::fromUtf8("ÄÖÜ"));
    QCOMPARE(intListItem->value().toList(), QList<QVariant>() << 1 << 2 << 3 << 4);
    QCOMPARE(doubleListItem->value().toList(), QList<QVariant>() << 3.5 << 3.5 << 3.5);
    QCOMPARE(boolListItem->value().toList(), QList<QVariant>() << false << true << true << false);
    QCOMPARE(unsetBeforeItem->value().isValid(), false);
    QCOMPARE(unsetAfterItem->value().isValid(), true);
}

void Ft_MGConfItem::set_bool()
{
    signalSpy->numberOfCalls = 0;

    boolItem->set(false);
    QCOMPARE(boolItem->value().toBool(), false);
    boolItem->set(true);
    QCOMPARE(boolItem->value().toBool(), true);

    QCOMPARE(signalSpy->numberOfCalls, 2);
}

void Ft_MGConfItem::set_int()
{
    signalSpy->numberOfCalls = 0;

    intItem->set(12);
    QCOMPARE(intItem->value().toInt(), 12);
    intItem->set(-5);
    QCOMPARE(intItem->value().toInt(), -5);

    QCOMPARE(signalSpy->numberOfCalls, 2);
}

void Ft_MGConfItem::set_string()
{
    signalSpy->numberOfCalls = 0;

    stringItem->set("Hi");
    QCOMPARE(stringItem->value().toString(), QString("Hi"));

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_unicode_string()
{
    signalSpy->numberOfCalls = 0;

    stringItem->set(QString::fromUtf8("Höäü"));
    QCOMPARE(stringItem->value().toString(), QString::fromUtf8("Höäü"));

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_double()
{
    signalSpy->numberOfCalls = 0;

    doubleItem->set(1.2345);
    QCOMPARE(doubleItem->value().toDouble(), 1.2345);

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_string_list()
{
    signalSpy->numberOfCalls = 0;

    stringListItem->set(QStringList() << "one" << "two" << "three");
    QCOMPARE(stringListItem->value().toStringList(), QStringList() << "one" << "two" << "three");

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_int_list()
{
    signalSpy->numberOfCalls = 0;

    intListItem->set(QList<QVariant>() << 10 << 11 << 12);
    QCOMPARE(intListItem->value().toList(), QList<QVariant>() << 10 << 11 << 12);

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_double_list()
{
    signalSpy->numberOfCalls = 0;

    doubleListItem->set(QList<QVariant>() << 1.1 << 2.2 << 3.3);
    QCOMPARE(doubleListItem->value().toList(), QList<QVariant>() << 1.1 << 2.2 << 3.3);

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_bool_list()
{
    signalSpy->numberOfCalls = 0;

    boolListItem->set(QList<QVariant>() << true << true << false);
    QCOMPARE(boolListItem->value().toList(), QList<QVariant>() << true << true << false);

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::unset()
{
    signalSpy->numberOfCalls = 0;

    boolItem->unset();
    QCOMPARE(boolItem->value().isValid(), false);

    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::unset_dir()
{
    MGConfItem test("/Test");
    QStringList dirs = test.listDirs();
    QStringList entries = test.listEntries();

    QVERIFY(dirs.contains("/Test/UnsetDir"));
    QVERIFY(!entries.contains("/Test/UnsetDir"));

    MGConfItem testDirItem("/Test/UnsetDir/Item");
    testDirItem.unset();
    testDirItem.sync();

    QTest::qWait(500); // sync won't happen immediately -> give some time for it

    dirs = test.listDirs();
    entries = test.listEntries();

    QVERIFY(!dirs.contains("/Test/UnsetDir"));
    QVERIFY(!entries.contains("/Test/UnsetDir"));
}

void Ft_MGConfItem::list_dirs()
{
    MGConfItem test("/Test");
    QStringList dirs = test.listDirs();

    QVERIFY(!dirs.contains("/Test/Bool"));
    QVERIFY(!dirs.contains("/Test/Int"));
    QVERIFY(!dirs.contains("/Test/String"));
    QVERIFY(!dirs.contains("/Test/Double"));
    QVERIFY(!dirs.contains("/Test/StringList"));
    QVERIFY(!dirs.contains("/Test/IntList"));
    QVERIFY(!dirs.contains("/Test/DoubleList"));
    QVERIFY(!dirs.contains("/Test/BoolList"));
    QVERIFY(!dirs.contains("/Test/UnsetBefore"));
    QVERIFY(!dirs.contains("/Test/UnsetAfter"));
    QVERIFY(dirs.contains("/Test/Dir"));
}

void Ft_MGConfItem::list_entries()
{
    MGConfItem test("/Test");
    test.sync();
    QStringList entries = test.listEntries();

    QVERIFY(!entries.contains("/Test/Bool"));  // has been unset above!
    QVERIFY(entries.contains("/Test/Int"));
    QVERIFY(entries.contains("/Test/String"));
    QVERIFY(entries.contains("/Test/Double"));
    QVERIFY(entries.contains("/Test/StringList"));
    QVERIFY(entries.contains("/Test/IntList"));
    QVERIFY(entries.contains("/Test/DoubleList"));
    QVERIFY(entries.contains("/Test/BoolList"));
    QVERIFY(!entries.contains("/Test/UnsetBefore"));
    QVERIFY(entries.contains("/Test/UnsetAfter"));
    QVERIFY(!entries.contains("/Test/Dir"));
}

void Ft_MGConfItem::get_default()
{
    intItem->unset();
    QCOMPARE(intItem->value(123).toInt(), 123);
    intItem->set(234);
    QCOMPARE(intItem->value(123).toInt(), 234);
}

void Ft_MGConfItem::propagate()
{
    MGConfItem secondIntItem("/Test/Int");
    secondIntItem.set(3000);
    QVERIFY_TIMEOUT(2000, intItem->value() == secondIntItem.value());
    QCOMPARE(signalSpy->numberOfCalls, 1);
}

void Ft_MGConfItem::set_external()
{
    // This must be the last test case.  The values that are set here
    // are checked after this program exits.

    boolItem->set(false);
    intItem->set(54321);
    stringItem->set("Good bye GConf");
    doubleItem->set(-2.5);
    stringListItem->set(QStringList() << "Good" << "bye" << "GConf" << QString::fromUtf8("äöü"));
    intListItem->set(QList<QVariant>() << 5 << 4 << 3 << 2 << 1);
    doubleListItem->set(QList<QVariant>() << -2.5 << -2.5);
    boolListItem->set(QList<QVariant>() << false << false << true << true);
    unsetAfterItem->set(QVariant());

    QProcess process;
    QStringList arguments;
    arguments << "-verify-keys";
    process.start(qApp->applicationFilePath(), arguments);
    QVERIFY(process.waitForFinished());
    QCOMPARE(process.exitCode(), 0);
}

void set_gconf_keys()
{
    MGConfItem boolItem("/Test/Bool");
    boolItem.set(true);

    MGConfItem intItem("/Test/Int");
    intItem.set(123);

    MGConfItem stringItem("/Test/String");
    stringItem.set("Hello GConf");

    MGConfItem doubleItem("/Test/Double");
    doubleItem.set(3.5);

    MGConfItem stringListItem("/Test/StringList");
    stringListItem.set(QStringList() << "Hello" << "GConf" << QString::fromUtf8("ÄÖÜ"));

    MGConfItem intListItem("/Test/IntList");
    intListItem.set(QList<QVariant>() << 1 << 2 << 3 << 4);

    MGConfItem doubleListItem("/Test/DoubleList");
    doubleListItem.set(QList<QVariant>() << 3.5 << 3.5 << 3.5);

    MGConfItem boolListItem("/Test/BoolList");
    boolListItem.set(QList<QVariant>() << false << true << true << false);

    MGConfItem unsetBeforeItem("/Test/UnsetBefore");
    unsetBeforeItem.unset();

    MGConfItem unsetAfterItem ("/Test/UnsetAfter");
    unsetAfterItem.set(1);

    MGConfItem testDirItem("/Test/Dir/Item");
    testDirItem.set("Dir Item");

    MGConfItem testUnsetDirItem("/Test/UnsetDir/Item");
    testUnsetDirItem.set("Dir Item");
}

void verify_gconf_keys()
{
    MGConfItem boolItem("/Test/Bool");
    QCOMPARE(boolItem.value().toBool(), false);

    MGConfItem intItem("/Test/Int");
    QCOMPARE(intItem.value().toInt(), 54321);

    MGConfItem stringItem("/Test/String");
    QCOMPARE(stringItem.value().toString(), QString("Good bye GConf"));

    MGConfItem doubleItem("/Test/Double");
    QCOMPARE(doubleItem.value().toDouble(), -2.5);

    MGConfItem stringListItem("/Test/StringList");
    QCOMPARE(stringListItem.value().toStringList(), QStringList() << "Good" << "bye" << "GConf" << QString::fromUtf8("äöü"));

    MGConfItem intListItem("/Test/IntList");
    QCOMPARE(intListItem.value().toList(), QList<QVariant>() << 5 << 4 << 3 << 2 << 1);

    MGConfItem doubleListItem("/Test/DoubleList");
    QCOMPARE(doubleListItem.value().toList(), QList<QVariant>() << -2.5 << -2.5);

    MGConfItem boolListItem("/Test/BoolList");
    QCOMPARE(boolListItem.value().toList(), QList<QVariant>() << false << false << true << true);

    MGConfItem unsetBeforeItem("/Test/UnsetBefore");
    QVERIFY(!unsetBeforeItem.value().isValid());

    MGConfItem unsetAfterItem("/Test/UnsetAfter");
    QVERIFY(!unsetAfterItem.value().isValid());
}

int main(int argc, char *argv[])
{
    QStringList arguments;
    for (int i = 1; i < argc; i ++) {
        arguments << argv[i];
    }
    QApplication app(argc, argv);
    QTEST_DISABLE_KEYPAD_NAVIGATION

    if (arguments.contains("-set-keys")) {
        set_gconf_keys();
    } else if (arguments.contains("-verify-keys")) {
        verify_gconf_keys();
    } else {
        Ft_MGConfItem tc;
        return QTest::qExec(&tc, argc, argv);
    }

    return 0;
}
