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

#ifndef UT_MAPPLETGCONFDATASTORE_H
#define UT_MAPPLETGCONFDATASTORE_H

#include <QObject>
#include <QVariant>

class MGConfDataStore;

class Ut_MGConfDataStore : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Tests
    void testReading();
    void testWriting();
    void testReadingNonExistingKey();
    void testWritingNonExistingKey();

    void testSignalReceivedWhenValueChanges();

    void testAllKeys();
    void testRemove();
    void testClear();
    void testContains();
    void testSubPathsNotSupported();

signals:
    void valueChanged();

private slots:
    void stringVariantSlot(const QString &string, const QVariant &variant);

private:
    MGConfDataStore *m_subject;

    QList<QPair<QString, QVariant> > stringVariantSlotArguments;
};

#endif  // UT_MAPPLETGCONFDATASTORE_H
