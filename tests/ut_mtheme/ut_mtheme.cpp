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

#include "ut_mtheme.h"

#include "mtheme_p.h"

#include <MApplication>
#include <MGConfItem>
#include <MLabel>
#include <MScalableImage>
#include <MTheme>

#include <QEventLoop>
#include <QtTest>
#include <QString>
#include <QProcess>
#include <QSignalSpy>

namespace {
    const char *KnownIconId = "meegotouch-combobox-indicator";
    const char *UnknownIconId = "xxx-unknown-icon-id";
};

void Ut_MTheme::initTestCase()
{
    static int argc = 1;
    static char *appName = (char*) "./Ut_MTheme";
    m_app = new MApplication(argc, &appName);

    while (MTheme::hasPendingRequests()) {
        usleep(10000);
        QCoreApplication::processEvents();
    }

    m_theme = MTheme::instance();
}

void Ut_MTheme::cleanupTestCase()
{
    delete m_app;
    m_app = 0;
}

void Ut_MTheme::testStyle()
{
    const MStyle *labelStyle1 = m_theme->style("MLabelStyle");
    QVERIFY(labelStyle1 != 0);

    const MStyle *labelStyle2 = m_theme->style("MLabelStyle", QString(), QString(), QString(), M::Landscape);
    QVERIFY(labelStyle2 != 0);

    // Check overload behavior of MTheme::style()
    QCOMPARE(labelStyle1, labelStyle2);

    const MStyle *labelStyle3 = m_theme->style("MLabelStyle", "MyButton", "active", "default", M::Landscape);
    QVERIFY(labelStyle3 != 0);

    // Loading of invalid styles is not supported, qFatal() is triggered in this case
    // const MStyle *invalidStyle = m_theme->style("InvalidStyle", "MyButton", "active", "default", M::Landscape);
    // QVERIFY(invalidStyle == 0);

    m_theme->releaseStyle(labelStyle1);
    m_theme->releaseStyle(labelStyle2);
    m_theme->releaseStyle(labelStyle3);
}

void Ut_MTheme::testThemeChangeCompleted()
{
    QSignalSpy spy(m_theme, SIGNAL(themeChangeCompleted()));

    MGConfItem themeNameItem("/meegotouch/theme/name");
    const QString currentThemeName = themeNameItem.value().toString();
    if (currentThemeName == QLatin1String("plankton")) {
        themeNameItem.set("blanco");
    } else {
        themeNameItem.set("plankton");
    }

    // Wait until the signal themeChangeCompleted() has been received
    QEventLoop eventLoop;
    connect(m_theme, SIGNAL(themeChangeCompleted()), &eventLoop, SLOT(quit()));
    QTimer::singleShot(20000, &eventLoop, SLOT(quit())); // fallback if themeChangeCompleted() is not send
    eventLoop.exec();

    QCOMPARE(spy.count(), 1);

    // Reset theme again
    themeNameItem.set(currentThemeName);
    eventLoop.exec();
    QCOMPARE(spy.count(), 2);
}

void Ut_MTheme::testPixmap()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    const QPixmap *pixmap = m_theme->pixmap(KnownIconId);
    QVERIFY(pixmap != 0);
    if (MTheme::hasPendingRequests()) {
        QVERIFY(pixmap->size().isEmpty());
        QCOMPARE(spy.count(), 0);
    } else {
        QVERIFY(!pixmap->size().isEmpty());
        QCOMPARE(spy.count(), 1);
    }
    QVERIFY(isIconCached(KnownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 1);

    waitForPendingThemeRequests();

    // Release icon
    m_theme->releasePixmap(pixmap);
    QVERIFY(!isIconCached(KnownIconId, pixmap->size()));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapWithSize()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    const QPixmap *fixedSizePixmap = m_theme->pixmap(KnownIconId, QSize(100, 150));
    QVERIFY(fixedSizePixmap != 0);
    QCOMPARE(fixedSizePixmap->size(), QSize(100, 150));
    QCOMPARE(spy.count(), MTheme::hasPendingRequests() ? 0 : 1);
    QVERIFY(isIconCached(KnownIconId, fixedSizePixmap->size()));
    QCOMPARE(cachedIconCount(), 1);

    waitForPendingThemeRequests();

    // Release icon
    m_theme->releasePixmap(fixedSizePixmap);
    QVERIFY(!isIconCached(KnownIconId, fixedSizePixmap->size()));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testUnknownPixmap()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    const QPixmap *unknownPixmap = m_theme->pixmap(UnknownIconId);
    QVERIFY(unknownPixmap != 0);
    if (MTheme::hasPendingRequests()) {
        QVERIFY(unknownPixmap->size().isEmpty());
        QCOMPARE(spy.count(), 0);
    } else {
        QVERIFY(!unknownPixmap->size().isEmpty());
        QCOMPARE(spy.count(), 1);
    }
    QVERIFY(isIconCached(UnknownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 1);

    waitForPendingThemeRequests();

    // Release icon
    m_theme->releasePixmap(unknownPixmap);
    QVERIFY(!isIconCached(UnknownIconId, unknownPixmap->size()));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapCopy()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    QPixmap *pixmap = m_theme->pixmapCopy(KnownIconId);
    QVERIFY(pixmap != 0);
    QVERIFY(!pixmap->size().isEmpty());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);

    m_theme->releasePixmap(pixmap);
}

void Ut_MTheme::testPixmapCopyWithSize()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));
    
    QPixmap *fixedSizePixmap = m_theme->pixmapCopy(KnownIconId, QSize(100, 150));
    QVERIFY(fixedSizePixmap != 0);
    QCOMPARE(fixedSizePixmap->size(), QSize(100, 150));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);

    m_theme->releasePixmap(fixedSizePixmap);
}

void Ut_MTheme::testUnknownPixmapCopy()
{
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    QPixmap *unknownPixmap = m_theme->pixmapCopy(UnknownIconId);
    QVERIFY(unknownPixmap != 0);
    QCOMPARE(unknownPixmap->size(), QSize(50, 50));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);

    m_theme->releasePixmap(unknownPixmap);
}

void Ut_MTheme::testPixmapCaching()
{
    QCOMPARE(cachedIconCount(), 0);

    const QPixmap *pixmap = m_theme->pixmap(KnownIconId);
    QVERIFY(isIconCached(KnownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 1);

    const QPixmap *fixedSizePixmap = m_theme->pixmap(KnownIconId, QSize(100, 150));
    QVERIFY(isIconCached(KnownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 2);

    const QPixmap *unknownPixmap = m_theme->pixmap(UnknownIconId);
    QVERIFY(isIconCached(UnknownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 3);

    m_theme->releasePixmap(pixmap);
    QVERIFY(!isIconCached(KnownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 2);

    m_theme->releasePixmap(fixedSizePixmap);
    QVERIFY(!isIconCached(KnownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 1);

    m_theme->releasePixmap(unknownPixmap);
    QVERIFY(!isIconCached(UnknownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testScalableImage()
{
    const MScalableImage *image = m_theme->scalableImage(KnownIconId, 1, 2, 3, 4);
    QVERIFY(image != 0);

    int left = -1;
    int right = -1;
    int top = -1;
    int bottom = -1;

    image->borders(&left, &right, &top, &bottom);
    QCOMPARE(left, 1);
    QCOMPARE(right, 2);
    QCOMPARE(top, 3);
    QCOMPARE(bottom, 4);

    const MScalableImage *unknownImage = m_theme->scalableImage(UnknownIconId, 1, 2, 3, 4);
    QVERIFY(unknownImage != 0);

    left = right = top = bottom = -1;
    unknownImage->borders(&left, &right, &top, &bottom);
    QCOMPARE(left, 1);
    QCOMPARE(right, 2);
    QCOMPARE(top, 3);
    QCOMPARE(bottom, 4);

    m_theme->releaseScalableImage(image);
    m_theme->releaseScalableImage(unknownImage);
}

void Ut_MTheme::testView()
{
    MLabel label(0, 0);
    MWidgetView *view = m_theme->view(&label);
    QVERIFY(view != 0);
}

bool Ut_MTheme::isIconCached(const QString &id, const QSize &size) const
{
    const QSize usedSize = size.isEmpty() ? QSize(0, 0) : size;

    MThemePrivate *d_ptr = m_theme->d_ptr;

    QHash<QString, CachedPixmap> pixmapIdentifiers = d_ptr->pixmapIdentifiers;
    QHashIterator<QString, CachedPixmap> it(pixmapIdentifiers);
    while (it.hasNext()) {
        it.next();
        if (it.value().imageId == id) {
            const QString sizeString = QLatin1Char('_') +
                                       QString::number(usedSize.width()) +
                                       QLatin1Char('_') +
                                       QString::number(usedSize.height());
            if (it.key().endsWith(sizeString)) {
                return true;
            }
        }
    }

    return false;
}

int Ut_MTheme::cachedIconCount() const
{
    MThemePrivate *d_ptr = m_theme->d_ptr;
    return d_ptr->pixmapIdentifiers.count();
}

void Ut_MTheme::waitForPendingThemeRequests()
{
    if (MTheme::hasPendingRequests()) {
        QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

        QEventLoop eventLoop;
        connect(m_theme, SIGNAL(pixmapRequestsFinished()), &eventLoop, SLOT(quit()));
        QTimer::singleShot(5000, &eventLoop, SLOT(quit())); // fallback if pixmapRequestsFinished() is not send
        eventLoop.exec();

        QCOMPARE(spy.count(), 1);
    }
}

QTEST_MAIN(Ut_MTheme)
