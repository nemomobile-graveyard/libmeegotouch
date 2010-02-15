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

#include <QObject>
#include <QtTest/QtTest>

#include <QDebug>
#include <DuiGConfItem>

// Helper class for listening to signals
class SignalListener : public QObject
{
    Q_OBJECT
public:
    int numberOfCalls;
    SignalListener() : numberOfCalls(0) {
    }

public slots:
    void valueChanged() {
        numberOfCalls++;
    }
};

// Tests for the public API
class DuiGConfItemTests : public QObject
{
    Q_OBJECT

    // Stored pointers etc.
private:
    DuiGConfItem *boolItem;
    DuiGConfItem *intItem;
    DuiGConfItem *stringItem;
    DuiGConfItem *doubleItem;
    DuiGConfItem *stringListItem;
    DuiGConfItem *intListItem;
    DuiGConfItem *doubleListItem;
    DuiGConfItem *boolListItem;
    DuiGConfItem *unsetBeforeItem;
    DuiGConfItem *unsetAfterItem;

    SignalListener *signalSpy;

    QTimer timer;
    bool timed_out;

    // Tests
private slots:
    // Init and cleanup helper functions
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void timeout();

    // Public API
    void path();
    void external_values();
    void set_bool();
    void set_int();
    void set_string();
    void set_unicode_string();
    void set_double();
    void set_string_list();
    void set_int_list();
    void set_double_list();
    void set_bool_list();
    void unset();
    void get_default();
    void list_dirs();
    void list_entries();
    void propagate();
    void set_external();
};

// Useful if you need to process some events until a condition becomes
// true.

#define QVERIFY_TIMEOUT(msecs, expr)                                        \
    do {                                                                    \
        timed_out = false;                                                  \
        timer.start(msecs);                                                 \
        while (!timed_out && !(expr)) {                                     \
            QApplication::processEvents(QEventLoop::WaitForMoreEvents);     \
        }                                                                   \
        QVERIFY((expr));                                                    \
    } while(0)
