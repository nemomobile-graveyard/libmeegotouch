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

#ifndef UT_MAPPLETINSTANTIATOR_H
#define UT_MAPPLETINSTANTIATOR_H

#include <QtTest/QtTest>
#include <QObject>

class MAppletInstantiator;
class QDBusPendingCallWatcher;

class Ut_MAppletInstantiator : public QObject
{
    Q_OBJECT

public:
    // For storing information about the D-Bus stub calls made
    static bool captureCalls;
    static QList<QString> callServiceNames;
    static QList<QString> callObjectPaths;
    static QList<QString> callInterfaces;
    static QList<QString> callMethods;
    static QList< QList<QVariant> > callArguments;
    static bool callError;
    static QMap<QString, QVariant> replyDataMap;

signals:
    void receivePackageData(QDBusPendingCallWatcher *watcher);

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testInstantiateAppletInPackageToKnownCanvas();
    void testInstantiateAppletInPackageToKnownCanvasFromLocalFile();
    void testInstantiateAppletInPackageToInvalidCanvas();
    void testReceivePackageData();
    void testReceivePackageDataFromLocalFile();
    void testReceivePackageDataWithError();

    // Future test cases
    /*
    void testInstantiateAppletInPackageToUnknownCanvas();
    void testInstantiateAppletsInMetaPackageToKnownCanvas();
    void testInstantiateAppletsInMetaPackageToUnknownCanvas();
    */
};

#endif
