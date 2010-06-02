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

#include <QObject>
#include <QSignalSpy>

#include "ut_mextensionwatcher.h"
#include "mextensionwatcher.h"
#include "mappletinstancedata.h"
#include "mapplicationextensionmetadata.h"
#include "mdesktopentry_stub.h"

// QFileSystemWatcher stubs
static QSet<QString> gWatchedPaths;
void QFileSystemWatcher::addPath(const QString &path)
{
    gWatchedPaths.insert(path);
}

// QFile stubs
static bool gFileExists = true;
bool QFile::exists(const QString& /*fileName*/)
{
    return gFileExists;
}

static const QString EXTENSION_FILE_NAME = "/usr/share/extensions/foo.desktop";
static const QString NOT_WATCHED_FILE_NAME = "FILE_THAT_IS_NOT_WATCHED";

void Ut_MExtensionWatcher::init()
{
    m_subject = new MExtensionWatcher();
}

void Ut_MExtensionWatcher::cleanup()
{
    delete m_subject;
    m_subject = 0;
}

void Ut_MExtensionWatcher::initTestCase()
{
    gWatchedPaths.clear();
    gFileExists = true;
}

void Ut_MExtensionWatcher::cleanupTestCase()
{
}

void Ut_MExtensionWatcher::testConstruction()
{
    // verify that the constructor did set up the filesystemwatcher signal
    QCOMPARE(disconnect(&m_subject->watcher,
                        SIGNAL(fileChanged(const QString &)),
                        m_subject, SLOT(notifyDataChanged(const QString &))),
             true);
    connect(&m_subject->watcher,
            SIGNAL(fileChanged(const QString &)),
            m_subject, SLOT(notifyDataChanged(const QString &)));
}

void Ut_MExtensionWatcher::testExtensionAddition()
{
    gMDesktopEntryStub->stubSetReturnValue("fileName", EXTENSION_FILE_NAME);
    QSharedPointer<const MApplicationExtensionMetaData>  testExtension(new MApplicationExtensionMetaData(EXTENSION_FILE_NAME));
    m_subject->addExtension(testExtension);
    QHash<QString, QSharedPointer<const MDesktopEntry> >::iterator extension =
        m_subject->extensionsToWatch.find(EXTENSION_FILE_NAME);
    QVERIFY(extension != m_subject->extensionsToWatch.end());
    QCOMPARE(extension.value().data(), testExtension.data());
    QCOMPARE(gWatchedPaths.contains(EXTENSION_FILE_NAME), true);
}

void Ut_MExtensionWatcher::testExtensionRemoval()
{
    gMDesktopEntryStub->stubSetReturnValue("fileName", EXTENSION_FILE_NAME);
    QSharedPointer<const MApplicationExtensionMetaData>  testExtension(new MApplicationExtensionMetaData(EXTENSION_FILE_NAME));
    m_subject->addExtension(testExtension);
    QCOMPARE(m_subject->extensionsToWatch.count(), 1);
    m_subject->removeExtension(testExtension->fileName());
    QCOMPARE(m_subject->extensionsToWatch.count(), 0);

    // Make sure that the signal is not emitted for an extension that is not watched
    QSignalSpy extensionSpy(m_subject,
                            SIGNAL(extensionChanged(const MDesktopEntry &)));
    gFileExists = true;
    emit fileChanged(EXTENSION_FILE_NAME);
    QCOMPARE(extensionSpy.count(), 0);
}

void Ut_MExtensionWatcher::testFileWatcherNotifications()
{
    connect(this,
            SIGNAL(fileChanged(const QString &)),
            m_subject, SLOT(notifyDataChanged(const QString &)));
    QSignalSpy extensionSpy(m_subject,
                            SIGNAL(extensionChanged(const MDesktopEntry &)));
    gMDesktopEntryStub->stubSetReturnValue("fileName", EXTENSION_FILE_NAME);
    QSharedPointer<const MApplicationExtensionMetaData>  testExtension(new MApplicationExtensionMetaData(EXTENSION_FILE_NAME));
    m_subject->addExtension(testExtension);

    gFileExists = false;
    emit fileChanged(EXTENSION_FILE_NAME);
    QCOMPARE(extensionSpy.count(), 0);

    gFileExists = true;
    emit fileChanged(EXTENSION_FILE_NAME);
    QCOMPARE(extensionSpy.count(), 1);

    emit fileChanged(NOT_WATCHED_FILE_NAME);
    QCOMPARE(extensionSpy.count(), 1);

    disconnect(this,
            SIGNAL(fileChanged(const QString &)),
            m_subject, SLOT(notifyDataChanged(const QString &)));
}

QTEST_MAIN(Ut_MExtensionWatcher)
