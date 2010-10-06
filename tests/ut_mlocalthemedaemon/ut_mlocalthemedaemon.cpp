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

#include "ut_mlocalthemedaemon.h"

#include <MApplication>
#include "mlocalthemedaemon.h"
#include "mpixmaphandle.h"

#include <MTheme>
#include <QtTest>
#include <QSignalSpy>

namespace {
    const char *KnownIconId = "icon-m_framework-close";
}

void Ut_MLocalThemeDaemon::initTestCase()
{
    static int argc = 1;
    static char *appName = (char*) "./Ut_MLocalThemeDaemon";
    m_app = new MApplication(argc, &appName);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_themeDaemon = new MLocalThemeDaemon(appName);
}

void Ut_MLocalThemeDaemon::cleanupTestCase()
{
    delete m_themeDaemon;
    m_themeDaemon = 0;

    delete m_app;
    m_app = 0;
}

void Ut_MLocalThemeDaemon::testPixmapHandle()
{
    qRegisterMetaType<MPixmapHandle>("MPixmapHandle");

    // MLocalThemeDaemon::pixmapHandle() for the local theme daemon is equal
    // to MLocalThemeDaemon::pixmapHandleSync() and hence synchronous.
    QSignalSpy spy(m_themeDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)));
    m_themeDaemon->pixmapHandle(KnownIconId, QSize());
    QVERIFY(!spy.isEmpty());

    m_themeDaemon->releasePixmap(KnownIconId, QSize());
}

void Ut_MLocalThemeDaemon::testPixmapHandleSync()
{
    qRegisterMetaType<MPixmapHandle>("MPixmapHandle");

    QSignalSpy spy(m_themeDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)));
    m_themeDaemon->pixmapHandleSync(KnownIconId, QSize());
    QVERIFY(!spy.isEmpty());

    m_themeDaemon->releasePixmap(KnownIconId, QSize());
}

void Ut_MLocalThemeDaemon::testPixmapSearchList()
{
    // TODO: There is no easy way to verify whether the directory has been successfully
    // added to be a new search path for pixmaps. Clarify whether this feature is used
    // at all before investigating further work.
    m_themeDaemon->addDirectoryToPixmapSearchList("foo", M::NonRecursive);
    m_themeDaemon->clearPixmapSearchList();
}

void Ut_MLocalThemeDaemon::testThemeInheritanceChain()
{
    const QString currentTheme = m_themeDaemon->currentTheme();
    QVERIFY(!currentTheme.isEmpty());

    const QStringList themeInheritanceChain = m_themeDaemon->themeInheritanceChain();
    QVERIFY(!themeInheritanceChain.isEmpty());
    QVERIFY(themeInheritanceChain.first().contains(currentTheme));
    QVERIFY(themeInheritanceChain.last().contains(QLatin1String("base")));
}

void Ut_MLocalThemeDaemon::testRegisterApplication()
{
    // TODO: There is no easy way to check whether registering the application
    // has been successful. Verify whether it might be worth to extend/modify the API
    // to being able to test this functionality.
    m_themeDaemon->registerApplicationName("foo");
}

QTEST_APPLESS_MAIN(Ut_MLocalThemeDaemon)
