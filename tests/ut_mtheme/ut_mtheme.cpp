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

namespace {
    const char *KnownIconId = "meegotouch-combobox-indicator";
    const char *UnknownIconId = "whatever";
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

QSettings *themeFile(const QString &theme)
{
    // Determine whether this is a m theme:
    // step 1: we need to have index.theme file
    QDir themeDir(THEMEDIR);
    const QString themeIndexFileName = themeDir.absolutePath() + QDir::separator() + theme + QDir::separator() + "index.theme";
    if (!QFile::exists(themeIndexFileName))
        return NULL;

    // step 2: it needs to be a valid ini file
    QSettings *themeIndexFile = new QSettings(themeIndexFileName, QSettings::IniFormat);
    if (themeIndexFile->status() != QSettings::NoError) {
        delete themeIndexFile;
        return NULL;
    }

    // step 3: we need to have X-MeeGoTouch-Metatheme group in index.theme

    // remove the X-DUI-Metatheme statement again when duitheme is phased out.
    if ((!themeIndexFile->childGroups().contains(QString("X-MeeGoTouch-Metatheme")))
        &&(!themeIndexFile->childGroups().contains(QString("X-DUI-Metatheme"))))
    {
        delete themeIndexFile;
        return NULL;
    }
    return themeIndexFile;
}

struct ThemeInfo {
    QString theme;
    QString themeName;
    QString themeIcon;
};

QList<ThemeInfo> findAvailableThemes()
{
    QList<ThemeInfo> themes;

    // find all directories under the theme directory
    QDir themeDir(THEMEDIR);
    const QFileInfoList directories = themeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach(const QFileInfo & dir, directories) {
        ThemeInfo info;

        const QSettings *themeIndexFile = themeFile(dir.baseName());
        if (!themeIndexFile)
            continue;

        // check if this theme is visible
        if (!themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Visible", true).toBool()) {
            delete themeIndexFile;
            continue;
        }

        info.theme = dir.baseName();
        info.themeName = themeIndexFile->value("Desktop Entry/Name", "").toString();
        info.themeIcon = themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Icon", "").toString();

        // remove this again, when duitheme is phased out
        if ( info.themeIcon.isEmpty() )
        {
            info.themeIcon = themeIndexFile->value("X-DUI-Metatheme/X-Icon", "").
                toString();
        }
        // end remove

        // ok it's a valid theme. Add it to list of themes
        themes.append(info);
        delete themeIndexFile;
    }

    return themes;
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

    const MStyle *labelStyle3 = m_theme->style("MLabelStyle", "MyButton", "active", QString(), M::Landscape);
    QVERIFY(labelStyle3 != 0);

    // Loading of invalid styles is not supported, qFatal() is triggered in this case
    // const MStyle *invalidStyle = m_theme->style("InvalidStyle", "MyButton", "active", QString(), M::Landscape);
    // QVERIFY(invalidStyle == 0);

    m_theme->releaseStyle(labelStyle1);
    m_theme->releaseStyle(labelStyle2);
    m_theme->releaseStyle(labelStyle3);
}

void Ut_MTheme::testThemeChangeCompleted()
{
    QList<ThemeInfo> themes = findAvailableThemes();

    MGConfItem themeNameItem("/meegotouch/theme/name");

    QSignalSpy themeChange(m_theme, SIGNAL(themeChangeCompleted()));

    QEventLoop eventLoop;

    const QString initialTheme = themeNameItem.value().toString();

    connect(m_theme, SIGNAL(themeChangeCompleted()), &eventLoop, SLOT(quit()));

    //This time should be same as: THEME_CHANGE_TIMEOUT at mthemedaemon (3 seconds)
    QTimer::singleShot(3000, &eventLoop, SLOT(quit()));

    for (int i = 0; i < themes.size(); i++) {
        //We set every theme available except the current one
        if (m_theme->currentTheme() != themes[i].theme) {
            themeNameItem.set(themes[i].theme);
            QVERIFY(themeNameItem.value().toString() == themes[i].theme);
            eventLoop.exec();
        }
    }
    //Coming back to the initial theme
    themeNameItem.set(initialTheme);
    eventLoop.exec();
    QVERIFY(themeNameItem.value().toString() == initialTheme);
    //After all we should have so many signals as themes installed due to we go through all the themes
    QCOMPARE(themeChange.count(), themes.size());
}

void Ut_MTheme::testPixmap()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
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
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(KnownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapWithSize()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
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
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(KnownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testUnknownPixmap()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
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
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(UnknownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapCopy()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    QPixmap *pixmap = m_theme->pixmapCopy(KnownIconId);
    m_theme->cleanupGarbage();
    QVERIFY(pixmap != 0);
    QVERIFY(!pixmap->size().isEmpty());
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapCopyWithSize()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    QPixmap *fixedSizePixmap = m_theme->pixmapCopy(KnownIconId, QSize(100, 150));
    m_theme->cleanupGarbage();
    QVERIFY(fixedSizePixmap != 0);
    QCOMPARE(fixedSizePixmap->size(), QSize(100, 150));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testUnknownPixmapCopy()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
    QSignalSpy spy(m_theme, SIGNAL(pixmapRequestsFinished()));

    QPixmap *unknownPixmap = m_theme->pixmapCopy(UnknownIconId);
    m_theme->cleanupGarbage();
    QVERIFY(unknownPixmap != 0);
    QCOMPARE(unknownPixmap->size(), QSize(50, 50));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testPixmapCaching()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
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
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(KnownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 2);

    m_theme->releasePixmap(fixedSizePixmap);
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(KnownIconId, QSize(100, 150)));
    QCOMPARE(cachedIconCount(), 1);

    m_theme->releasePixmap(unknownPixmap);
    m_theme->cleanupGarbage();
    QVERIFY(!isIconCached(UnknownIconId, QSize()));
    QCOMPARE(cachedIconCount(), 0);
}

void Ut_MTheme::testApplicationPixmapDirs()
{
    QSKIP("Resource cleanup is temporary disabled ...", SkipAll);
    const QPixmap *pixmap1;
    const QPixmap *pixmap2;

    m_theme->addPixmapDirectory(qApp->applicationDirPath());

    QString pixmap1_id="ut_mtheme";
    QString pixmap2_id="ut_mtheme_second";

    /*!
     * We request to local pixmap (ut_mtheme folder)
     */
    pixmap1 = m_theme->pixmap("ut_mtheme");
    pixmap2 = m_theme->pixmap("ut_mtheme_second");
    /*!
     * Checking if the pixmaps are different (expected)
     */
    QVERIFY(pixmap1->toImage() != pixmap2->toImage());
    QVERIFY(pixmap1->size() != pixmap2->size());
    /*!
     * Releasing pixmaps
     */
    m_theme->releasePixmap(pixmap1);
    m_theme->releasePixmap(pixmap2);
    /*!
     * We released but pixmap still in cached (no clean applied yet)
     */
    QVERIFY(isIconCached(pixmap1_id, QSize()));
    QVERIFY(isIconCached(pixmap2_id, QSize()));

    /*!
     * Cleaning released pixmaps
     */
    m_theme->cleanupGarbage();
    /*!
     * The local pixmaps shouldn't be cached
     */
    QCOMPARE(cachedIconCount(), 0);
    QVERIFY(!isIconCached(pixmap1_id, QSize()));
    QVERIFY(!isIconCached(pixmap2_id, QSize()));
    m_theme->clearPixmapDirectories();
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
        QTimer::singleShot(10000, &eventLoop, SLOT(quit())); // fallback if pixmapRequestsFinished() is not send
        eventLoop.exec();

        QCOMPARE(spy.count(), 1);
    }
}

QTEST_APPLESS_MAIN(Ut_MTheme)
