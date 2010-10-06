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

    const QString serverAddress = "m.mtestthemedaemon";

    m_process = new QProcess();
    QStringList arguments;
    arguments.append("-address");
    arguments.append(serverAddress);
    m_process->start("mthemedaemon", arguments);
    QVERIFY(m_process->waitForStarted());

    m_themeDaemon = new MRemoteThemeDaemon(appName, 5000, serverAddress);
}

void Ut_MRemoteThemeDaemon::cleanupTestCase()
{
    delete m_themeDaemon;
    m_themeDaemon = 0;

    m_process->close();
    delete m_process;
    m_process = 0;

    delete m_app;
    m_app = 0;
}

void Ut_MRemoteThemeDaemon::testPixmapHandle()
{
    qRegisterMetaType<Qt::HANDLE>("MPixmapHandle");

    // The signal pixmapCreatedOrChanged() will be emitted if the request could be fulfilled
    // synchronously, otherwise pixmapCreatedOrChanged() will be emitted.
    QSignalSpy createdSpy(m_themeDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)));
    m_themeDaemon->pixmapHandle(KnownIconId, QSize());

    bool gotPixmap = !createdSpy.isEmpty();
    if (!gotPixmap) {
        QSignalSpy changedSpy(m_themeDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)));
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
    qRegisterMetaType<Qt::HANDLE>("MPixmapHandle");

    QSignalSpy spy(m_themeDaemon, SIGNAL(pixmapCreatedOrChanged(QString, QSize, MPixmapHandle)));
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
}

void Ut_MRemoteThemeDaemon::testLoadThemeDaemonPriorities_data()
{
    QTest::addColumn<QString>("filename");
    QTest::addColumn<qint32>("priorityForegroundApplication");
    QTest::addColumn<qint32>("priorityBackgroundApplication");
    QTest::addColumn<qint32>("priorityPrestartedApplication");
    QTest::addColumn<QHash<QString, qint32> >("applicationSpecificPriorities");

    // test default values for not existing file
    QTest::newRow("not existing") << QString() << 100 << 0 << -10 << QHash<QString, qint32>();

    QHash<QString, qint32> priorities;
    priorities.insert("sysuid", -90);
    priorities.insert("duihome", 23);
    priorities.insert("meego-im-uiserver", 55);
    QTest::newRow("valid") <<  QString(m_app->applicationDirPath() + QLatin1String("/priorities.conf")) << 3 << 7 << -100 << priorities;
}

typedef QHash<QString, qint32> StringIntHash;
void Ut_MRemoteThemeDaemon::testLoadThemeDaemonPriorities()
{
    QFETCH(QString, filename);
    QFETCH(qint32, priorityForegroundApplication);
    QFETCH(qint32, priorityBackgroundApplication);
    QFETCH(qint32, priorityPrestartedApplication);
    QFETCH(StringIntHash, applicationSpecificPriorities);

    MRemoteThemeDaemonPrivate *d_ptr = m_themeDaemon->d_ptr;
    d_ptr->loadThemeDaemonPriorities(filename);

    QCOMPARE(d_ptr->priorityForegroundApplication, priorityForegroundApplication);
    QCOMPARE(d_ptr->priorityBackgroundApplication, priorityBackgroundApplication);
    QCOMPARE(d_ptr->priorityPrestartedApplication, priorityPrestartedApplication);
    QCOMPARE(d_ptr->applicationSpecificPriorities, applicationSpecificPriorities);
}

Q_DECLARE_METATYPE(StringIntHash)
QTEST_APPLESS_MAIN(Ut_MRemoteThemeDaemon)
