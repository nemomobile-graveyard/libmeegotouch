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

#include "ut_mcommonpixmaps.h"

#include <MApplication>
#include <MTheme>
#include <MGConfItem>
#include "mthemedaemon.h"

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
}

void Ut_MCommonPixmaps::cleanupTestCase()
{
    if (m_commonPixmaps) {
        m_commonPixmaps->clear();
        delete m_commonPixmaps;
        m_commonPixmaps = 0;
    }

    delete m_themeDaemon;
    m_themeDaemon = 0;

    delete m_app;
    m_app = 0;
}

void Ut_MCommonPixmaps::testConstructor()
{
    m_commonPixmaps = new MCommonPixmaps(m_themeDaemon, true);
    QVERIFY(m_commonPixmaps != 0);
}

void Ut_MCommonPixmaps::testLoad()
{
    QList<M::MThemeDaemonProtocol::PixmapHandle> mostUsed;

    mostUsed = m_commonPixmaps->mostUsedPixmapHandles();
    /* nothing has been loaded, so it must be empty */
    QVERIFY(mostUsed.isEmpty());

    m_commonPixmaps->load();

    /*need to wait until the resources are loaded*/
    while (mostUsed.isEmpty()) {
        usleep(10000);
        QCoreApplication::processEvents();
        mostUsed = m_commonPixmaps->mostUsedPixmapHandles();
    }

    /* now that theme data is loaded, this mustn't be empty */
    QVERIFY(!mostUsed.isEmpty());
}

void Ut_MCommonPixmaps::testIncreaseRequestCount()
{
    const M::MThemeDaemonProtocol::PixmapIdentifier id(UncommonIconId, QSize(20,20));
    ImageResource *resource = m_themeDaemon->findImageResource(UncommonIconId);

    for (int i = 0; i<1000; i++) {
        m_commonPixmaps->increaseRequestCount(id, resource);
    }

    /* after this, the icon asked must be in the cache */
    QList<M::MThemeDaemonProtocol::PixmapHandle> mostUsed;
    mostUsed = m_commonPixmaps->mostUsedPixmapHandles();

    bool found = false;
    foreach (const M::MThemeDaemonProtocol::PixmapHandle& handle, mostUsed) {
        if (handle.identifier == id) {
            found = true;
            break;
        }
    }

    QVERIFY(found == true);
}

void Ut_MCommonPixmaps::testReload()
{
    const M::MThemeDaemonProtocol::PixmapIdentifier id(UncommonIconId, QSize(20,20));
    ImageResource *resource = m_themeDaemon->findImageResource(UncommonIconId);

    m_commonPixmaps->reload(id, resource);
}

void Ut_MCommonPixmaps::testSaveAndClear()
{
    QList<M::MThemeDaemonProtocol::PixmapHandle> mostUsed;
    const M::MThemeDaemonProtocol::PixmapIdentifier id(UncommonIconId, QSize(20,20));

    m_commonPixmaps->save();

    /*clear the list afer saving*/
    m_commonPixmaps->clear();

    mostUsed = m_commonPixmaps->mostUsedPixmapHandles();
    /* nothing has been loaded, so it must be empty */
    QVERIFY(mostUsed.isEmpty());
}

QTEST_APPLESS_MAIN(Ut_MCommonPixmaps)
