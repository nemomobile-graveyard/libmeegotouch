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

#include <mimagedirectory.h>
#include <mapplication.h>

#include <QImageWriter>
#include <QDebug>

#include <msystemdirectories.h>

#include "ut_mimagedirectory.h"

QString MThemeDaemon::systemThemeCacheDirectory()
{
    QString dir(MSystemDirectories::cacheDirectory() + QLatin1String("themedaemon") + QDir::separator());
    return dir;
}

static MThemeResourceManager g_themeResourceManager;

MThemeResourceManager& MThemeResourceManager::instance()
{
    return g_themeResourceManager;
}

QSvgRenderer* MThemeResourceManager::svgRenderer(const QString& absoluteFilePath)
{
    Q_UNUSED( absoluteFilePath );
    return 0;
}

MApplication *app(NULL);

Ut_MImageDirectory::Ut_MImageDirectory() :
        m_mThemeImagesDirectory(0),
        m_pixmapImageResource(0),
        m_svgImageResource(0),
     m_pngImageFileName("ut_mimagedirectory-image-png.png"),
     m_svgImageFileName("ut_mimagedirectory-image-svg.svg")
{
}

void Ut_MImageDirectory::initTestCase()
{
    static char *app_name[1] = { (char *) "./ut_mimagedirectory" };
    static int argc = 0;
    app = new MApplication(argc, app_name);

    m_mThemeImagesDirectory = new MThemeImagesDirectory(QApplication::applicationDirPath() + "/ut_mimagedirectory-samples");
    QVERIFY(m_mThemeImagesDirectory != 0);

    QSize imageSize(16, 16);
    QImage image(imageSize, QImage::Format_ARGB32);
    image.fill(0xFFFF0000);

    QImageWriter imageWriter;
    imageWriter.setFileName(QApplication::applicationDirPath() + QDir::separator() + m_pngImageFileName);
    imageWriter.setFormat("png");
    QVERIFY(imageWriter.write(image));

    m_pixmapImageResource = new PixmapImageResource(QApplication::applicationDirPath() + QDir::separator() + m_pngImageFileName);
    QVERIFY(m_pixmapImageResource != 0);

    m_svgImageResource = new SvgImageResource("testresource", QApplication::applicationDirPath() + QDir::separator() + m_svgImageFileName);
    QVERIFY(m_svgImageResource != 0);
}

void Ut_MImageDirectory::cleanupTestCase()
{
    delete m_mThemeImagesDirectory;
    m_mThemeImagesDirectory = 0;

    delete m_pixmapImageResource;
    m_pixmapImageResource = 0;

    delete m_svgImageResource;
    m_svgImageResource = 0;

    QFile().remove(QApplication::applicationDirPath() + QDir::separator() + m_pngImageFileName);
}

void Ut_MImageDirectory::testReloadLocalizedResources_data()
{
    QTest::addColumn<QString>("setLocale");
    QTest::addColumn<QString>("gotLocale");

    QTest::newRow("sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic")
      << "sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic"
      << "sr_Cyrl_RS_REVISED@collation=phonebook";

    QTest::newRow("sr_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic")
      << "sy_Cyrl_RS_REVISED@collation=phonebook;calendar=islamic"
      << "sy";

    QTest::newRow("de_DE@collation=phonebook")
      << "de_DE@collation=phonebook"
      << "de_DE";

    QTest::newRow("de_DE_lala_foo@collation=phonebook")
      << "de_DE_lala_foo@collation=phonebook"
      << "de_DE_lala";

    QTest::newRow("de_CH@collation=phonebook")
      << "de_CH@collation=phonebook"
      << "de";
}

void Ut_MImageDirectory::testReloadLocalizedResources()
{
    QFETCH(QString, setLocale);
    QFETCH(QString, gotLocale);

    m_mThemeImagesDirectory->reloadLocalizedResources( setLocale );

    QString myLocale = m_mThemeImagesDirectory->locale();

    QCOMPARE( myLocale, gotLocale );
}

QTEST_APPLESS_MAIN(Ut_MImageDirectory);


void Ut_MImageDirectory::testApplyDebugColors()
{
    QSize imageSize(16, 16);
    QImage image(imageSize, QImage::Format_ARGB32);
    image.fill(0xFFFF0000);

    m_pixmapImageResource->applyDebugColors(&image);

    QVERIFY(!image.isNull());
}

void Ut_MImageDirectory::testReleaseWithoutDelete()
{
    QSize size(8, 8);

    m_pixmapImageResource->fetchPixmap(size);

    PixmapCacheEntry *pixmapCacheEntry = m_pixmapImageResource->releaseWithoutDelete(size);

    QVERIFY2(pixmapCacheEntry, "PixmapCacheEntry not found");

    QVERIFY2(!m_pixmapImageResource->pixmapCacheEntries().contains(size),
             "PixmapCacheEntry for given size shall not be found in cache");

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    QCOMPARE(pixmapCacheEntry->handle.size, size);
#else
    QCOMPARE(pixmapCacheEntry->pixmap->size(), size);
#endif
}

void Ut_MImageDirectory::testPixmapCacheEntries()
{
    QCOMPARE(m_pixmapImageResource->pixmapCacheEntries().size(), 0);

    QSize size(8, 8);
    m_pixmapImageResource->fetchPixmap(size); //create new entry in cache

    QHash<QSize, const PixmapCacheEntry*> entries = m_pixmapImageResource->pixmapCacheEntries();

    QCOMPARE(entries.size(), 1);

    QVERIFY2(entries[size], "Entry in cache expected");
}

void Ut_MImageDirectory::testSaveToFsCache()
{
    QSize size(8, 8);
    QImage image(size, QImage::Format_ARGB32_Premultiplied);

    m_pixmapImageResource->saveToFsCache(image, size);

    PixmapCacheEntry *entry = new PixmapCacheEntry();
    QImage loadedImage = m_pixmapImageResource->loadFromFsCache(size, entry);

    QVERIFY2(!loadedImage.isNull(), "Invalid QImage object returned");
}

void Ut_MImageDirectory::testPixmapImageResourceCreatePixmap()
{
    createAndCheckPixmap(m_pixmapImageResource);
}

void Ut_MImageDirectory::testSvgImageResourceCreatePixmap()
{
    createAndCheckPixmap(m_svgImageResource);
}

void Ut_MImageDirectory::createAndCheckPixmap(ImageResource *resource)
{
    QSize size(8, 8);
    QImage image = resource->createPixmap(size);

    QCOMPARE(image.size(), size);
}

void Ut_MImageDirectory::testIsLocalizedResource()
{
    QCOMPARE(m_mThemeImagesDirectory->isLocalizedResource("nonexistent"), false);

    m_mThemeImagesDirectory->addImageResource(QApplication::applicationDirPath().toUtf8().constData(),
                                              m_pngImageFileName.toUtf8().constData(), true);

    QVERIFY2(m_mThemeImagesDirectory->imageResources.size() == 0, "Resources hash is not empty");

    QString imageId = "localizedImageResource";
    m_mThemeImagesDirectory->localizedImageResources.insert(imageId, new PixmapImageResource(""));

    QVERIFY2(m_mThemeImagesDirectory->isLocalizedResource(imageId),
             "Resource expected to be localized");

    m_mThemeImagesDirectory->localizedImageResources.clear();

    m_mThemeImagesDirectory->addImageResource(QApplication::applicationDirPath().toUtf8().constData(),
                                              m_svgImageFileName.toUtf8().constData(), false);

    QVERIFY2(m_mThemeImagesDirectory->imageResources.size() > 0,
             "Resources hash is empty - not localized resource expected");

    QVERIFY2(!m_mThemeImagesDirectory->isLocalizedResource(
                 m_mThemeImagesDirectory->imageResources.keys()[0]),
             "Resource expected to be not localized");
}

void Ut_MImageDirectory::testPath()
{
    QCOMPARE(m_mThemeImagesDirectory->path(), m_mThemeImagesDirectory->m_path);
}

void Ut_MImageDirectory::testSaveIdsInCache()
{
    QStringList idsList;
    idsList.append("test1");
    idsList.append("test2");

    m_mThemeImagesDirectory->saveIdsInCache(idsList, QApplication::applicationDirPath().toUtf8().constData());

    QString cacheFileName = m_mThemeImagesDirectory->createIdCacheFilename(QApplication::applicationDirPath().toUtf8().constData());

    QVERIFY(QFile::exists(cacheFileName));

    QFile::remove(cacheFileName);
}
