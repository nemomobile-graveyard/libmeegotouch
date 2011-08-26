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

#include "ut_mcommonpixmaps.h"

#include <QDir>
#include <QSignalSpy>
#include <MApplication>
#include <MTheme>
#include <MGConfItem>
#include "mthemedaemon.h"
#include <mimagedirectory.h>

namespace {
    const char *UncommonIconId = "meegotouch-speechbubble-incoming-background";
};

void Ut_MCommonPixmaps::initTestCase()
{
    static int argc = 1;
    static char *appName = (char*) "./Ut_MCommonPixmaps";
    m_app = new MApplication(argc, &appName);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_commonPixmaps = 0;

    MGConfItem currentLocale("/meegotouch/i18n/language");
    MGConfItem currentTheme("/meegotouch/theme/name");
    m_themeDaemon = new MThemeDaemon(MThemeDaemon::LocalDaemon);
    bool themeActivated = m_themeDaemon->activateTheme(currentTheme.value().toString(),
                                                       currentLocale.value().toString(),
                                                       QList<MThemeDaemonClient*>(),
                                                       pixmapsToDelete);
    QVERIFY(themeActivated);

    m_themeDaemon->themeImageDirs.append(new MThemeImagesDirectory(qApp->applicationDirPath()));

    qRegisterMetaType< M::MThemeDaemonProtocol::MostUsedPixmaps >("M::MThemeDaemonProtocol::MostUsedPixmaps");

    createCache();
}

void Ut_MCommonPixmaps::cleanupTestCase()
{
    delete m_themeDaemon;
    m_themeDaemon = 0;

    delete m_app;
    m_app = 0;
}

void Ut_MCommonPixmaps::init()
{
    m_commonPixmaps = new MCommonPixmaps(m_themeDaemon, true);
    QVERIFY(m_commonPixmaps != 0);
}

void Ut_MCommonPixmaps::cleanup()
{
    if (m_commonPixmaps) {
        m_commonPixmaps->clear();
        delete m_commonPixmaps;
        m_commonPixmaps = 0;
    }
}

void Ut_MCommonPixmaps::createCache()
{
    //more than MCommonPixmaps loads at once
    cacheImages.append("meegotouch-speechbubble-incoming-background-pressed");
    cacheImages.append("meegotouch-speechbubble-outgoing-background");
    cacheImages.append("meegotouch-speechbubble-outgoing-background-pressed");
    cacheImages.append("meegotouch-speechbubble-incoming-mirrored-background-pressed");
    cacheImages.append("meegotouch-speechbubble-outgoing-mirrored-background");
    cacheImages.append("meegotouch-speechbubble-outgoing-mirrored-background-pressed");

    MCommonPixmaps *commonPixmaps = new MCommonPixmaps(m_themeDaemon, true);
    commonPixmaps->setCacheFilename(QDir::tempPath() + "/ut_mcommonpixmaps_cache.list");

    foreach(QString imageId, cacheImages) {
        const M::MThemeDaemonProtocol::PixmapIdentifier id(imageId, QSize(20,20));
        ImageResource *resource = m_themeDaemon->findImageResource(imageId);
        QVERIFY2(resource != NULL, qPrintable(QString("Cannot find requested image: %1").arg(imageId)));
        commonPixmaps->increaseRequestCount(id, resource);
    }

    commonPixmaps->save();
    commonPixmaps->clear();
    delete commonPixmaps;
}

void Ut_MCommonPixmaps::loadCache(const QString &cacheFilename)
{
    QSignalSpy signalSpy(m_commonPixmaps, SIGNAL(mostUsedPixmapsChanged(const M::MThemeDaemonProtocol::MostUsedPixmaps&)));

    m_commonPixmaps->setCacheFilename(QDir::tempPath() + "/" + (cacheFilename.isEmpty() ? "ut_mcommonpixmaps_cache.list" : cacheFilename));
    m_commonPixmaps->load();
    m_commonPixmaps->setCacheFilename(QDir::tempPath() + "/ut_mcommonpixmaps_cache.dummy"); //to prevent saving over

    //need to wait until resources are loaded (60sec timeout)
    int waitCount = 0;
    while (signalSpy.count() == 0 && waitCount<600) {
        QTest::qWait(100);
        waitCount++;
    }

    QVERIFY2(signalSpy.count() == 1, "No mostUsedPixmapsChanged signal in time");
}

bool Ut_MCommonPixmaps::isInCache(const QString &imageId)
{
    QList<M::MThemeDaemonProtocol::PixmapHandle> mostUsed = m_commonPixmaps->mostUsedPixmapHandles();

    bool found = false;
    foreach (const M::MThemeDaemonProtocol::PixmapHandle& handle, mostUsed) {
        if (handle.identifier.imageId == imageId) {
            found = handle.pixmapHandle.isValid();
            break;
        }
    }

    return found;
}

int Ut_MCommonPixmaps::cacheItemCount()
{
    return m_commonPixmaps->mostUsedPixmapHandles().count();
}


void Ut_MCommonPixmaps::testLoad()
{
    loadCache();

    QCOMPARE(cacheItemCount(), cacheImages.count());

    foreach(QString imageId, cacheImages) {
        QVERIFY(isInCache(imageId) == true);
    }
}

void Ut_MCommonPixmaps::testIncreaseRequestCount()
{
    loadCache();

    const M::MThemeDaemonProtocol::PixmapIdentifier id(UncommonIconId, QSize(20,20));
    ImageResource *resource = m_themeDaemon->findImageResource(UncommonIconId);
    m_commonPixmaps->increaseRequestCount(id, resource);

    //must be in the cache now
    QVERIFY(isInCache(id.imageId) == true);
    QCOMPARE(cacheItemCount(), cacheImages.count()+1);
}

void Ut_MCommonPixmaps::testReload()
{
    loadCache();

    QSize resourceSize = QSize(20,20);
    const M::MThemeDaemonProtocol::PixmapIdentifier id(cacheImages.at(0), resourceSize);
    ImageResource *resource = m_themeDaemon->findImageResource(cacheImages.at(0));

    //must be in cache with valid handle before reloading
    QVERIFY(isInCache(id.imageId) == true);
    QVERIFY(resource->pixmapHandle(resourceSize).isValid() == true);

    m_commonPixmaps->reload(id, resource);

    //handle must be released now
    QVERIFY(resource->pixmapHandle(resourceSize).isValid() == false);

    m_commonPixmaps->increaseRequestCount(id, resource);

    //now it must be in cache with valid handle
    QVERIFY(isInCache(id.imageId) == true);
    QVERIFY(resource->pixmapHandle(resourceSize).isValid() == true);
}

void Ut_MCommonPixmaps::testSaveAndClear()
{
    loadCache();

    const M::MThemeDaemonProtocol::PixmapIdentifier id(UncommonIconId, QSize(20,20));
    ImageResource *resource = m_themeDaemon->findImageResource(UncommonIconId);

    //should not be in the cache yet
    QVERIFY(isInCache(id.imageId) == false);

    m_commonPixmaps->increaseRequestCount(id, resource);

    //should be in the cache now
    QCOMPARE(cacheItemCount(), cacheImages.count()+1);
    QVERIFY(isInCache(id.imageId) == true);

    m_commonPixmaps->setCacheFilename(QDir::tempPath() + "/ut_mcommonpixmaps_cache.test");
    m_commonPixmaps->save();
    m_commonPixmaps->clear();

    //cache should be empty now
    QVERIFY(cacheItemCount() == 0);

    //recreate and load saved cache file
    delete m_commonPixmaps;
    m_commonPixmaps = new MCommonPixmaps(m_themeDaemon, true);
    QVERIFY(m_commonPixmaps != 0);
    loadCache("ut_mcommonpixmaps_cache.test");

    //Should be in the cache again
    QCOMPARE(cacheItemCount(), cacheImages.count()+1);
    QVERIFY(isInCache(id.imageId) == true);
}

QTEST_APPLESS_MAIN(Ut_MCommonPixmaps)
