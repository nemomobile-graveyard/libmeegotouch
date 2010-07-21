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

#include "ut_mremotethemedaemon.h"

#include <MApplication>
#include "mremotethemedaemon.h"
#include "mremotethemedaemon_p.h"
#include <MTheme>
#include <QtTest>
#include <QSignalSpy>

namespace {
    const char *KnownIconId = "icon-m_framework-close";
}

void Ut_MRemoteThemeDaemon::initTestCase()
{
    static int argc = 1;
    static char *appName = (char*) "./Ut_MRemoteThemeDaemon";
    m_app = new MApplication(argc, &appName);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_themeDaemon = new MRemoteThemeDaemon(appName, 5000);
}

void Ut_MRemoteThemeDaemon::cleanupTestCase()
{
    delete m_themeDaemon;
    m_themeDaemon = 0;

    delete m_app;
    m_app = 0;
}

void Ut_MRemoteThemeDaemon::testPixmapHandle()
{
    qRegisterMetaType<Qt::HANDLE>("Qt::HANDLE");

    // The signal pixmapCreated() will be emitted if the request could be fulfilled
    // synchronously, otherwise pixmapChanged() will be emitted.
    QSignalSpy createdSpy(m_themeDaemon, SIGNAL(pixmapCreated(QString, QSize, Qt::HANDLE)));
    m_themeDaemon->pixmapHandle(KnownIconId, QSize());

    bool gotPixmap = !createdSpy.isEmpty();
    if (!gotPixmap) {
        QSignalSpy changedSpy(m_themeDaemon, SIGNAL(pixmapChanged(QString, QSize, Qt::HANDLE)));
        while (m_themeDaemon->hasPendingRequests()) {
            usleep(10000);
            QCoreApplication::processEvents();
        }
        gotPixmap = !changedSpy.isEmpty();
    }

    QVERIFY(gotPixmap);

    m_themeDaemon->releasePixmap(KnownIconId, QSize());
}

void Ut_MRemoteThemeDaemon::testPixmapHandleSync()
{
    qRegisterMetaType<Qt::HANDLE>("Qt::HANDLE");

    QSignalSpy spy(m_themeDaemon, SIGNAL(pixmapCreated(QString, QSize, Qt::HANDLE)));
    m_themeDaemon->pixmapHandleSync(KnownIconId, QSize());
    QVERIFY(!spy.isEmpty());

    m_themeDaemon->releasePixmap(KnownIconId, QSize());
}

void Ut_MRemoteThemeDaemon::testPixmapSearchList()
{
    // TODO: There is no easy way to verify whether the directory has been successfully
    // added to be a new search path for pixmaps. Clarify whether this feature is used
    // at all before investigating further work.
    
    MRemoteThemeDaemonPrivate *d_ptr = m_themeDaemon->d_ptr;
    const int oldSequenceCounter = int(d_ptr->sequenceCounter);

    m_themeDaemon->addDirectoryToPixmapSearchList("foo", M::NonRecursive);
    QCOMPARE(int(d_ptr->sequenceCounter), oldSequenceCounter + 1);

    m_themeDaemon->clearPixmapSearchList();
    QCOMPARE(int(d_ptr->sequenceCounter), oldSequenceCounter + 2);
}

void Ut_MRemoteThemeDaemon::testThemeInheritanceChain()
{
    const QString currentTheme = m_themeDaemon->currentTheme();
    QVERIFY(!currentTheme.isEmpty());

    const QStringList themeInheritanceChain = m_themeDaemon->themeInheritanceChain();
    QVERIFY(!themeInheritanceChain.isEmpty());
    QVERIFY(themeInheritanceChain.first().contains(currentTheme));
    QVERIFY(themeInheritanceChain.last().contains(QLatin1String("base")));
}

void Ut_MRemoteThemeDaemon::testRegisterApplication()
{
    // TODO: There is no easy way to check whether registering the application
    // has been successful. Verify whether it might be worth to extend/modify the API
    // to being able to test this functionality.

    MRemoteThemeDaemonPrivate *d_ptr = m_themeDaemon->d_ptr;
    const int oldSequenceCounter = int(d_ptr->sequenceCounter);

    m_themeDaemon->registerApplicationName("foo");
    QCOMPARE(int(d_ptr->sequenceCounter), oldSequenceCounter + 1);

    m_themeDaemon->registerApplicationName("foo");
    QCOMPARE(int(d_ptr->sequenceCounter), oldSequenceCounter + 2);
}

QTEST_MAIN(Ut_MRemoteThemeDaemon)
