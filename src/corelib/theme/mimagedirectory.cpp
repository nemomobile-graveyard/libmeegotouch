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


#ifndef Q_OS_WIN
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>


#ifndef MAX_PATH_WITHOUT_ALLOC
#define MAX_PATH_WITHOUT_ALLOC 256
#endif
#endif


#include "mimagedirectory.h"
#include "mthemedaemon.h"
#include "mdebug.h"

#include "mgraphicssystemhelper.h"

#include <QDateTime>
#include <QDir>
#include <QPainter>
#include <QPixmap>

#include <QLinkedList>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <QtMeeGoGraphicsSystemHelper>
#endif

#ifdef  Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif


namespace {
    const unsigned int ID_CACHE_VERSION = 1;
    const unsigned int IMAGE_CACHE_VERSION = 4;

      // reimplementing these in order to delete (instead of free()ing) the return values
    char *strDup(const char *src);
    char *appendToPath(const char *basepath, const char *subdir);
}

uint qHash(const QSize &size)
{
    return (size.width() & 0x0000ffff) | (size.height() << 16);
}

ImageResource::ImageResource(const QString& absoluteFilePath) :
#ifdef  Q_WS_X11
    convertToX11(false),
#endif
    filePath(absoluteFilePath),
    buffer(0),
    cacheFile(0)
{
}

MPixmapHandle ImageResource::fetchPixmap(const QSize &size)
{
    QHash<QSize, PixmapCacheEntry*>::iterator it = cachedPixmaps.find(size);
    PixmapCacheEntry *cacheEntry;
    if (it == cachedPixmaps.end()) {
#ifdef  Q_WS_X11
        // When starting an application, the X11 window must be filled with a default background
        // image in a fast way. The filling can only be done fast if the background image is available
        // as X11 pixmap.
        convertToX11 = MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()
                       && filePath.contains("forcex11");
#endif

        cacheEntry = new PixmapCacheEntry();
        cachedPixmaps.insert(size, cacheEntry);

        // try to load it from filesystem cache
        QImage image;
        if (shouldBeCached()) {
            image = loadFromFsCache(size, cacheEntry);
        }

        if (image.isNull()) {
            // we didn't have the correctly sized pixmap in cache, so we need to create one.
            image = createPixmap(size);

            if (!image.isNull()) {
                applyDebugColors(&image);

                if (shouldBeCached() && saveToFsCache(image, size)) {
                    QImage imageFromCache = loadFromFsCache(size, cacheEntry);
                    if (imageFromCache.isNull()) {
                        qCritical() << "Themedaemon: Failed to reload image" << uniqueKey() << "from cache. Something bad is happening.";
                        fillCacheEntry(cacheEntry, image, size);
                    } else {
                        image = imageFromCache;
                    }
                } else {
                    fillCacheEntry(cacheEntry, image, size);
                }
            }
        }
    } else  {
        cacheEntry = *it;
    }

    ++cacheEntry->refCount;


    return cacheEntry->handle;
}

void ImageResource::applyDebugColors(QImage *image)
{
    if (image->depth() != 32) {
        return;
    }

    static QByteArray visualizationEnv = qgetenv("MTHEMEDAEMON_TRANSPARENCY_VISUALIZATION");
    static bool visualizeCompleteTransparency = (visualizationEnv == "complete") || (visualizationEnv == "both");
    static bool visualizePartialTransparency =  (visualizationEnv == "partial") || (visualizationEnv == "both");

    if (! (visualizeCompleteTransparency || visualizePartialTransparency )) {
        return;
    }

    QImage backup = *image;

    bool completelyTransparent = true;
    for (int i = 0; i < image->height(); ++i) {
        QRgb *scanline = reinterpret_cast<QRgb*>(image->scanLine(i));
        for (int j = 0; j < image->width(); ++j) {
            QRgb *pixel = &scanline[j];
            if (*pixel == qRgba(0, 0, 0, 0)) {
                if (visualizePartialTransparency) {
                    *pixel = qRgb(0, 0, 255);
                }
            } else {
                if (visualizePartialTransparency && qAlpha(*pixel) < 255) {
                    *pixel = qRgb(0, 255, 0);
                }
                completelyTransparent = false;
            }
        }
    }
    if (completelyTransparent) {
        if (visualizeCompleteTransparency) {
            image->fill(qRgb(255, 0, 127));
        } else if (visualizePartialTransparency){
            *image = backup;
        }
    }
}

ImageResource::~ImageResource()
{
    deleteCacheFile();
}

void ImageResource::releasePixmap(const QSize &size)
{
    Q_ASSERT_X(cachedPixmaps.contains(size), "ImageResource", "Cannot release pixmap because the cache entry cannot be found for the pixmap!");

    PixmapCacheEntry *cacheEntry = cachedPixmaps[size];

    // decrease the refcount.
    --cacheEntry->refCount;

    // if this was the last reference, release the pixmap
    if (cacheEntry->refCount == 0) {
        // remove the cache entry from this resource
        delete cacheEntry;
        cachedPixmaps.remove(size);
    }
}

PixmapCacheEntry* ImageResource::releaseWithoutDelete(const QSize &size)
{
    QHash<QSize, PixmapCacheEntry*>::iterator iter = cachedPixmaps.find(size);
    if (iter == cachedPixmaps.end())
        return NULL;

    PixmapCacheEntry *cacheEntry = *iter;

    // decrease the refcount.
    --cacheEntry->refCount;

    // if this was the last reference, release the pixmap
    if (cacheEntry->refCount == 0) {
        // remove the cache entry from this resource
        cachedPixmaps.remove(size);

        return cacheEntry;
    }
    return NULL;
}

MPixmapHandle ImageResource::pixmapHandle(const QSize &size)
{
    QHash<QSize, PixmapCacheEntry*>::iterator it = cachedPixmaps.find(size);
    if (it == cachedPixmaps.end()) {
        return MPixmapHandle();
    } else {
        return (*it)->handle;
    }
}

QHash<QSize, const PixmapCacheEntry*> ImageResource::pixmapCacheEntries() const
{
    QHash<QSize, const PixmapCacheEntry*> entries;

    QHashIterator<QSize, PixmapCacheEntry*> it(cachedPixmaps);
    while (it.hasNext()) {
        it.next();
        entries.insert(it.key(), it.value());
    }

    return entries;
}

QImage ImageResource::loadFromFsCache(const QSize& size, PixmapCacheEntry *cacheEntry)
{
    const QString cacheFileName = createCacheFilename(size, uniqueKey());
    const QString cacheMetaFileName = cacheFileName + ".meta";

    if (cacheFileName.isEmpty()) {
        return QImage();
    }

    if (QFile::exists(cacheFileName) && QFile::exists(cacheMetaFileName)) {
        QFile meta(cacheMetaFileName);

        if (meta.open(QFile::ReadOnly)) {
            QDataStream stream(&meta);
            uint version;
            stream >> version;
            if (version != IMAGE_CACHE_VERSION) {
                // will be replaced with up to date version
                meta.close();
                return QImage();
            }
            uint timestamp;
            stream >> timestamp;
            QFileInfo fileInfo(absoluteFilePath());
            if (timestamp != fileInfo.lastModified().toTime_t()) {
                // will be replaced with up to date version
                meta.close();
                return QImage();
            }

            QSize imageSize;
            int imageFormat;

            stream >> imageSize;
            stream >> imageFormat;
            meta.close();

            deleteCacheFile();

            cacheFile = new QFile(cacheFileName);
            if (!cacheFile->open(QFile::ReadOnly)) {
                delete cacheFile;
                cacheFile = 0;
                return QImage();
            }

            Q_ASSERT(!buffer);
            buffer = cacheFile->map(0, cacheFile->size());
            QImage image((const uchar*)buffer, imageSize.width(), imageSize.height(), (QImage::Format)imageFormat);

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
            if(QMeeGoGraphicsSystemHelper::isRunningMeeGo() && !convertToX11) {
                cacheEntry->handle.format = (QImage::Format)imageFormat;
                cacheEntry->handle.eglHandle = QMeeGoGraphicsSystemHelper::imageToEGLSharedImage(image);
                cacheEntry->handle.numBytes = image.numBytes();
                cacheEntry->handle.shmHandle = qPrintable(cacheFileName);
                cacheEntry->handle.size = imageSize;
                cacheEntry->handle.directMap = true;
            } else
#endif
            {
                fillCacheEntry(cacheEntry, image, size);
            }

            cacheFile->close();
            return image;
        } else {
            mWarning("ImageResource") << "Failed to load pixmap from cache" << cacheFileName;
        }
    }

    return QImage();
}

bool ImageResource::saveToFsCache(QImage &image, const QSize &size)
{
    return saveToFsCache(image, size, uniqueKey());
}

bool ImageResource::saveToFsCache(QImage &image, const QSize& size, const QString &uniqueKey)
{
    static bool failedCacheSaveAttempt = false;

    if (failedCacheSaveAttempt)
        return false;

    const QString cacheFileName = createCacheFilename(size, uniqueKey);
    if (cacheFileName.isEmpty()) {
        return false;
    }

    QFile cache(cacheFileName);
    if (!cache.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(cacheFileName).absolutePath());
        if (!cache.open(QFile::WriteOnly)) {
            mWarning("ImageResource") <<
                     "Wrong permissions for cache directory" <<
                     MThemeDaemon::systemThemeCacheDirectory() <<
                     ". Cache is disabled.";
            failedCacheSaveAttempt = true;
            return false;
        }
    }

    // FIXME: Uncomment once the proper support for non-alpha images is ready.
    if (/*pixmap.format() != QImage::Format_RGB32 &&*/ image.format() != QImage::Format_ARGB32_Premultiplied) {
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    int written = cache.write((const char*)image.constBits(), image.byteCount());
    bool flushed = cache.flush();
    cache.close();

      // TODO: basically this occurs when the system is low on disk space,
      // now failedCacheSaveAttempt is set when this occurs which means
      // writing caches will be disabled until TD is restarted. It should
      // be considered whether to have some sort of a system to monitor
      // available disk space, stop writing when available disk space goes
      // below some point and resume after there's again enough disk space.
      // This of course means that images that are loaded during low disk
      // space conditions will be shared via shm segment instead of mmap to
      // a cache file and won't be saved into cache until they are reloaded
      // (in practice meaning all users free the image after which new user
      // should request the image).
    if (written < image.byteCount() || !flushed) {
        QFile::remove(cacheFileName);
        failedCacheSaveAttempt = true;
        return false;
    }

    const QString cacheMetaFileName = cacheFileName + QLatin1String(".meta");
    QFile meta(cacheMetaFileName);
    if (!meta.open(QFile::WriteOnly)) {
        mWarning("ImageResource") <<
                 "Wrong permissions for cache directory" <<
                 MThemeDaemon::systemThemeCacheDirectory() <<
                 ". Cache is disabled.";
        failedCacheSaveAttempt = true;
        return false;
    }
      // TODO: check if it's really enough to check if flushing failed to
      // detect writing failures
    QDataStream stream(&meta);
    stream << IMAGE_CACHE_VERSION;
    QFileInfo fileInfo(absoluteFilePath());
    stream << fileInfo.lastModified().toTime_t();
    stream << image.size();
    stream << image.format();
    flushed = meta.flush();
    meta.close();

    if (!flushed)  {
        QFile::remove(cacheFileName);
        QFile::remove(cacheMetaFileName);
        failedCacheSaveAttempt = true;
        return false;
    }

    return true;
}

QString ImageResource::createCacheFilename(const QSize& size, const QString &cacheKey)
{
    QString tmp = cacheKey;
    if (tmp.isEmpty()) {
        return QString();
    }
    tmp.replace(QLatin1Char('_'), QLatin1String("__"));
    tmp.replace(QLatin1Char('/'), QLatin1String("_."));
    tmp += '(' + QString::number(size.width()) + QLatin1Char(',') + QString::number(size.height()) + QLatin1Char(')');

    return MThemeDaemon::systemThemeCacheDirectory() + QLatin1String("images") + QDir::separator() + tmp;
}


void ImageResource::fillCacheEntry(PixmapCacheEntry *cacheEntry, QImage &image, const QSize &size)
{
#ifdef  Q_WS_X11
    if (convertToX11) {
        Pixmap x11Pixmap = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                                                 image.width(), image.height(), 16);
        cacheEntry->pixmap = new QPixmap(QPixmap::fromX11Pixmap(x11Pixmap));

        QPainter painter(cacheEntry->pixmap);
        painter.drawImage(0, 0, image);
        painter.end();

        XSync(QX11Info::display(), false);
        cacheEntry->handle.xHandle = cacheEntry->pixmap->handle();
    } else {
        MGraphicsSystemHelper::pixmapFromImage(cacheEntry, image, uniqueKey(), size);
    }
#else
    MGraphicsSystemHelper::pixmapFromImage(cacheEntry, image, uniqueKey(), size);
#endif
}

void ImageResource::deleteCacheFile()
{
    if (cacheFile) {
        cacheFile->unmap(buffer);
        buffer = 0;
        delete cacheFile;
        cacheFile = 0;
    }
}

bool ImageResource::shouldBeCached()
{
    return true;
}

QImage PixmapImageResource::createPixmap(const QSize &size)
{
    QImage image(absoluteFilePath());

    if (size.isNull() || size == image.size()) {
        // the requested size is (0,0) or the original image is the same size as the requested one.
        // we can use the original image
        return image;
    }

    // we need to resize the image.
    return image.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

QString PixmapImageResource::uniqueKey()
{
    return absoluteFilePath();
}

bool PixmapImageResource::shouldBeCached()
{
    return true;
}

QImage IconImageResource::createPixmap(const QSize &size)
{
   QSvgRenderer renderer(absoluteFilePath());

    QSize svgImageSize = size;
    if (size.isNull()) {
        if (renderer.isValid()) {
            svgImageSize = renderer.defaultSize();
        } else if (svgImageSize.isNull()) {
            mWarning("IconImageResource") << "    Invalid svg, size of the document is (0,0)";
        }
    }

    QImage image(svgImageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    if (renderer.isValid()) {
        QPainter painter(&image);
        renderer.render(&painter);
    }

    return image;
}

QString IconImageResource::uniqueKey()
{
    return absoluteFilePath();
}

QImage SvgImageResource::createPixmap(const QSize &size)
{
    QSvgRenderer renderer(absoluteFilePath());

    QSize svgImageSize = size;
    if (size.isNull()) {
        // the requested size is (0,0) so we need to fetch the default size from the svg.
        if (renderer.isValid()) {
            svgImageSize = renderer.boundsOnElement(imageId).size().toSize();
        } else if (svgImageSize.isNull()) {
            mWarning("SvgImageResource") << "    Invalid svg, size of id" << imageId << "is (0,0)";
        }
    }

    QImage image(svgImageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    if (renderer.isValid()) {
        QPainter painter(&image);
        renderer.render(&painter, imageId);
    }

    return image;
}

QString SvgImageResource::uniqueKey()
{
    return absoluteFilePath() + QLatin1String("$$") + imageId;
}

const QString MThemeImagesDirectory::pixmapsDir = "images";
const QString MThemeImagesDirectory::iconsDir = "icons";
const QString MThemeImagesDirectory::svgDir = "svg";

MThemeImagesDirectory::MThemeImagesDirectory(const QString &path, const QString &locale) :
    m_path(path),
    m_locale(locale)
{
    QList<QString> directories;

    // read images, icons and svgs
    readImageResources(m_path + QDir::separator() + pixmapsDir);
    readImageResources(m_path + QDir::separator() + iconsDir);
    readSvgResources(m_path + QDir::separator() + svgDir);

    reloadLocalizedResources(locale);
}

MThemeImagesDirectory::~MThemeImagesDirectory()
{
    qDeleteAll(imageResources);
    imageResources.clear();
    qDeleteAll(localizedImageResources);
    localizedImageResources.clear();
    idsInLocalizedSvgImages.clear();
}

ImageResource *MThemeImagesDirectory::findImage(const QString &imageId)
{
    // First check from locale specific icons
    ImageResource *resource = localizedImageResources.value(imageId, NULL);
    if (resource) {
        qDebug() << "Found icon" << imageId << "from locale";
    }

    if (!resource) {
        QList<QString> svgPaths = idsInLocalizedSvgImages.values(imageId);
        if (!svgPaths.empty()) {
            qDebug() << "Found icon" << imageId << "from locale";
            if (svgPaths.count() > 1) {
                mWarning("MThemeImagesDirectory") << "Found multiple svgs with candidates for id" << imageId << "Using first one: " << svgPaths;
            }
            resource = new SvgImageResource(imageId, svgPaths.first());
            localizedImageResources.insert(imageId, resource);
        }
    }

    if (!resource) {
        // Check if we have this id already resolved
        resource = imageResources.value(imageId, NULL);
    }

    if (!resource) {
        QList<QString> svgPaths = idsInSvgImages.values(imageId);
        if (!svgPaths.empty()) {
            if (svgPaths.count() > 1) {
                mWarning("MThemeImagesDirectory") << "Found multiple svgs with candidates for id" << imageId << "Using first one: " << svgPaths;
            }
            resource = new SvgImageResource(imageId, svgPaths.first());
            imageResources.insert(imageId, resource);
        }
    }

    return resource;
}

bool MThemeImagesDirectory::isLocalizedResource(const QString &imageId) const
{
    return localizedImageResources.contains(imageId) || idsInLocalizedSvgImages.contains(imageId);
}

void MThemeImagesDirectory::reloadLocalizedResources(const QString &locale)
{
    qDeleteAll(localizedImageResources);
    localizedImageResources.clear();
    idsInLocalizedSvgImages.clear();

    if( locale.isEmpty() ) {
        m_locale = "";
        return;
    }

    //try to find a matching directory for the specified locale, if exactly matching
    //directory is not found we try to truncate the locale string to find next matching
    //directory ("ar_SA" ==> "ar", "en_GB" ==> "en" etc. )
    QString localePath;
    m_locale = locale;
    while(1) {
        //check if directory for the locale is found and break the loop
        localePath = m_path + QDir::separator() + "locale" + QDir::separator() + m_locale + QDir::separator();

        if( QFile::exists(localePath) ) {
            break;
        }

        // according to http://userguide.icu-project.org/locale the separators
        // that specify the parts of a locale are "_", "@", and ";", e.g.
        // in sr_Latn_RS_REVISED@currency=USD;calendar=islamic-civil
        // so we remove them from the end of the locale string.

        int semicolonIndex = m_locale.lastIndexOf( ';' );
        if ( semicolonIndex != -1 )
        {
            // found semicolon, remove it and remaining part of string
            m_locale.truncate( semicolonIndex );
        }
        else
        {
            int atIndex = m_locale.lastIndexOf( '@' );
            if ( atIndex != -1 )
            {
                // found "@", remove it and remaining part of string
                m_locale.truncate( atIndex );
            }
            else
            {
                int underscoreIndex = m_locale.lastIndexOf( '_' );
                if ( underscoreIndex != -1 )
                {
                    // found "_", remove it and remaining part of string
                    m_locale.truncate( underscoreIndex );
                }
                else
                {
                    // locale not found, and no way to shorten it, return
                    m_locale = "";
                    mWarning("MThemeImagesDirectory")
                      << "Could not find locale" << m_locale << ".";
                    return;
                }
            }
        }
    }

    //output a warning if the original locale was truncated
    if( m_locale != locale )
        mWarning("MThemeImagesDirectory") << "Using" << m_locale << "as locale, because no localized resources was found for" << locale;

    // read localized images, icons and svgs
    readImageResources(localePath + pixmapsDir, true);
    readImageResources(localePath + iconsDir, true);
    readSvgResources(localePath + svgDir, true);
}

QString MThemeImagesDirectory::path() const
{
    return m_path;
}

QString MThemeImagesDirectory::locale() const
{
    return m_locale;
}


void MThemeImagesDirectory::readImageResources(const QString& path, bool localized)
{
#ifdef Q_OS_WIN
    QList<QString> directories;
    directories.append(path);
    while (!directories.isEmpty()) {
        //if dir does not exist jump to next one
        QDir dir(directories.takeFirst());
        if (!dir.exists()) {
            continue;
        }

        // go through all files in this directory.
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == QLatin1String("png") || i->suffix() == QLatin1String("jpg") || i->suffix() == QLatin1String("svg")) {
                addImageResource(*i, localized);
            }
        }
    }
#else
    crawlImageDirectory(imageCrawlerHandler, path, localized);
#endif
}

void MThemeImagesDirectory::readSvgResources(const QString& path, bool localized)
{
#ifdef Q_OS_WIN
    QList<QString> directories;
    directories.append(path);
    while (!directories.isEmpty()) {
        //if dir does not exist jump to next one
        QDir dir(directories.takeFirst());
        if (!dir.exists()) {
            continue;
        }

        // go through all files in this directory.
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == QLatin1String("svg")) {
                addSvgResource(*i, localized);
            }
        }
    }
#else
    crawlImageDirectory(svgCrawlerHandler, path, localized);
#endif
}


#ifdef Q_OS_WIN
void MThemeImagesDirectory::addImageResource(const QFileInfo& fileInfo, bool localized)
{
    if( !localized ) {
        //only one image resource from the theme with a same name is allowed
        if (imageResources.contains(fileInfo.baseName())) {
            mWarning("MThemeDaemon") << "Multiple reference of " << fileInfo.baseName()
                                     << "Using " << imageResources.value(fileInfo.baseName())->absoluteFilePath()
                                     << "instead of" << fileInfo.absoluteFilePath();
        } else {
            //if "svg" add IconImageResource, if "jpg" or "png" add PixmapImageResource
            ImageResource *res = 0;
            if(fileInfo.suffix() == QLatin1String("svg"))
                res = new IconImageResource(fileInfo.absoluteFilePath());
            else
                res = new PixmapImageResource(fileInfo.absoluteFilePath());

            imageResources.insert(fileInfo.baseName(), res);
        }
    } else {
        //add localized image resource only if same named resource was found from the original theme
        if (!imageResources.contains(fileInfo.baseName()) && !idsInSvgImages.contains(fileInfo.baseName())) {
            mWarning("MThemeDaemon") << "Ignoring localized image resource" << fileInfo.absoluteFilePath() << "because it was not found from the original theme!";
        } else {
            //if "svg" add IconImageResource, if "jpg" or "png" add PixmapImageResource
            localizedImageResources.insert(fileInfo.baseName(), fileInfo.suffix() == QLatin1String("svg") ? (ImageResource*) new IconImageResource(fileInfo.absoluteFilePath()) : (ImageResource*) new PixmapImageResource(fileInfo.absoluteFilePath()));
        }
    }
}

void MThemeImagesDirectory::addSvgResource(const QFileInfo& fileInfo, bool localized)
{
    // matches and id specified in a svg file.
    // the id may either appear in a g, image or path tag.
    static QRegExp idRegexp("<(g|image|path)\\s[^>]*id=\"([^\"]*)\"[^>]*>");
    if (!loadIdsFromCache(fileInfo, localized)) {
        QFile svgFile(fileInfo.filePath());
        if (svgFile.open(QIODevice::ReadOnly)) {
            QByteArray content = svgFile.readAll();
            int pos = 0;
            QStringList ids;
            QMultiHash<QString, QString>& svgIds = localized ? idsInLocalizedSvgImages : idsInSvgImages;
            while ((pos = idRegexp.indexIn(content, pos)) != -1) {
                QString id = idRegexp.cap(2);
                svgIds.insert(id, fileInfo.absoluteFilePath());

                pos += idRegexp.matchedLength();
                ids << id;
            }
            saveIdsInCache(ids, fileInfo);
        } else {
                mWarning("MThemeImagesDirectory") << "Failed to load ids from" << fileInfo.absoluteFilePath();
        }
    }
}

bool MThemeImagesDirectory::loadIdsFromCache(const QFileInfo& svgFileInfo, bool localized)
{
    const QString idCacheFile = createIdCacheFilename(svgFileInfo.absoluteFilePath());
    if (QFile::exists(idCacheFile)) {
        QFile file(idCacheFile);
        if (file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            uint version;
            stream >> version;
            if (version != ID_CACHE_VERSION) {
                file.close();
                return false;
            }
            uint timestamp;
            stream >> timestamp;
            if (timestamp != svgFileInfo.lastModified().toTime_t()) {
                file.close();
                return false;
            }

            QStringList ids;
            stream >> ids;
            QMultiHash<QString, QString>& svgIds = localized ? idsInLocalizedSvgImages : idsInSvgImages;
            foreach(const QString& id, ids) {
                svgIds.insert(id, svgFileInfo.absoluteFilePath());
            }
            file.close();
            return true;
        } else {
            mWarning("MThemeImagesDirectory") << "Failed to load id cache file" << idCacheFile;
        }
        return false;
    }
    return false;
}

void MThemeImagesDirectory::saveIdsInCache(const QStringList& ids, const QFileInfo& svgFileInfo) const
{
    const QString idCacheFile = createIdCacheFilename(svgFileInfo.absoluteFilePath());

    QFile file(idCacheFile);
    if (!file.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(idCacheFile).absolutePath());
        if (!file.open(QFile::WriteOnly)) {
            mDebug("MThemeImagesDirectory") << "Failed to save id cache file" << svgFileInfo.fileName() << "to" << idCacheFile;
            return;
        }
    }

    QDataStream stream(&file);
    stream << ID_CACHE_VERSION;
    stream << svgFileInfo.lastModified().toTime_t();
    stream << ids;
    file.close();
}
#else
void MThemeImagesDirectory::addImageResource(const char *path, const char *filename, bool localized)
{
    const char *a = 0;
    char *basename;

    for(const char *t = filename; t; t = strstr(t+1, "."))
        a = t;
    if(filename < a) {
        int len = a - filename;
        basename = new char[len + 1];
        memcpy(basename, filename, len);
        basename[len] = 0;
    } else {
        return;  // no suffix, we can return now - though this should be never triggered (checks in calling function)
    }

    if( !localized ) {
        //only one image resource from the theme with a same name is allowed
        if (imageResources.contains(basename)) {
            mWarning("MThemeDaemon") << "Multiple reference of " << basename
                                     << "Using " << imageResources.value(basename)->absoluteFilePath()
                                     << "instead of" << path;
        } else {
            //if "svg" add IconImageResource, if "jpg" or "png" add PixmapImageResource
            ImageResource *res = 0;
            if (!strcasecmp(a+1, "svg"))
                res = new IconImageResource(path);
            else
                res = new PixmapImageResource(path);

            imageResources.insert(basename, res);
        }
    }
    else {
        //add localized image resource only if same named resource was found from the original theme
        if (!imageResources.contains(basename) && !idsInSvgImages.contains(basename)) {
            mWarning("MThemeDaemon") << "Ignoring localized image resource" << path << "because it was not found from the original theme!";
        } else {
            //if "svg" add IconImageResource, if "jpg" or "png" add PixmapImageResource
            localizedImageResources.insert(basename, !strcasecmp(a+1, "svg") ? (ImageResource*) new IconImageResource(path) : (ImageResource*) new PixmapImageResource(path));
        }
    }

    delete[] basename;
}

void MThemeImagesDirectory::addSvgResource(const char *path, bool localized)
{
    // matches and id specified in a svg file.
    // the id may either appear in a g, image or path tag.
    static QRegExp idRegexp("<(g|image|path)\\s[^>]*id=\"([^\"]*)\"[^>]*>");
    if (!loadIdsFromCache(path, localized)) {
        QFile svgFile(path);
        if (svgFile.open(QIODevice::ReadOnly)) {
            QByteArray content = svgFile.readAll();
            int pos = 0;
            QStringList ids;
            QMultiHash<QString, QString>& svgIds = localized ? idsInLocalizedSvgImages : idsInSvgImages;
            while ((pos = idRegexp.indexIn(content, pos)) != -1) {
                QString id = idRegexp.cap(2);
                svgIds.insert(id, path);

                pos += idRegexp.matchedLength();
                ids << id;
            }
            saveIdsInCache(ids, path);
        } else {
                mWarning("MThemeImagesDirectory") << "Failed to load ids from" << path;
        }
    }
}


void MThemeImagesDirectory::crawlImageDirectory(MThemeImagesDirectory::FileCrawlerHandler handler, const QString& path, bool localized)
{
    QLinkedList<char *> dirs;
    dirs.append(::strDup(qPrintable(path)));
    while (!dirs.isEmpty()) {
        char *s = dirs.takeFirst();
        DIR *d = opendir(s);
        if (!d) {
            delete[] s;
            continue;
        }

          // allocate buffer for file names here in order to avoid allocating it
          // once per file (unless the length of filename exceeds 256 - 2
          // characters) - only per-file operation required is copying the file
          // name at the end of buffer containing the path
        char *fnbuf = new char[strlen(s) + MAX_PATH_WITHOUT_ALLOC];
        memcpy(fnbuf, s, strlen(s));
        char *fnptr = fnbuf + strlen(s);
        *(fnptr++) = '/';

        for (struct dirent *e = readdir(d); e; e = readdir(d)) { // FIXME: retarded . and .. detection
            if (e->d_type == DT_DIR) {
                bool ignoreMe = e->d_name[0] == '.' && (e->d_name[1] == 0 || (e->d_name[1] == '.' && e->d_name[2] == 0));
                if (!ignoreMe)
                    dirs.append(::appendToPath(s, e->d_name));
            } else if (e->d_type == DT_REG) { // TODO: handle symlinks
                unsigned l = strlen(e->d_name);
                if(MAX_PATH_WITHOUT_ALLOC - 2 < l) { // avoid  hazardous buffer overflow by extra alloc/free round - slow path
                    char *tmp = ::appendToPath(s, e->d_name);
                    handler(this, tmp, tmp + strlen(s) + 1, localized);
                    delete[] tmp;
                } else {
                    memcpy(fnptr, e->d_name, l);
                    fnptr[l] = 0;
                    handler(this, fnbuf, fnptr, localized);
                }
            }
        }

        delete[] s;
        delete[] fnbuf;
        closedir(d);
    }
}


  // extra suffix checking round happens in these two - something that should be avoided...
void MThemeImagesDirectory::imageCrawlerHandler(MThemeImagesDirectory *self, const char *path, const char *filename, bool localized)
{
    const char *suffix = 0;
    for (const char *i = filename; i; i = strstr(suffix, "."))
        suffix = i + 1;

      // TODO: use some clever suffix dictionary rather than having logically
      // OR'd strcasecmp list here
    if (!(!strcasecmp(suffix, "png") || !strcasecmp(suffix, "jpg") ||
          !strcasecmp(suffix, "svg") || !strcasecmp(suffix, "jpeg")))
        return;

    self->addImageResource(path, filename, localized);
}

void MThemeImagesDirectory::svgCrawlerHandler(MThemeImagesDirectory *self, const char *path, const char *filename, bool localized)
{
    const char *suffix = 0;
    for (const char *i = filename; i; i = strstr(suffix, "."))
        suffix = i + 1;

    if (strcasecmp(suffix, "svg"))
        return;

    self->addSvgResource(path, localized);
}


bool MThemeImagesDirectory::loadIdsFromCache(const char *path, bool localized)
{
    const QString idCacheFile = createIdCacheFilename(path);
    if (QFile::exists(idCacheFile)) {
        struct stat sb;

        if(stat(path, &sb))
            return false;

        QFile file(idCacheFile);
        if (file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            uint version;
            stream >> version;
            if (version != ID_CACHE_VERSION) {
                file.close();
                return false;
            }
            uint timestamp;
            stream >> timestamp;
            if (timestamp != (uint)sb.st_mtime) {
                file.close();
                return false;
            }

            QStringList ids;
            stream >> ids;
            QMultiHash<QString, QString>& svgIds = localized ? idsInLocalizedSvgImages : idsInSvgImages;
            foreach(const QString& id, ids) {
                svgIds.insert(id, path);
            }
            file.close();
            return true;
        } else {
            mWarning("MThemeImagesDirectory") << "Failed to load id cache file" << idCacheFile;
        }
        return false;
    }
    return false;
}

void MThemeImagesDirectory::saveIdsInCache(const QStringList& ids, const char *path) const
{
    const QString idCacheFile = createIdCacheFilename(path);

    struct stat sb;
    if(stat(path, &sb))
        return;

    QFile file(idCacheFile);
    if (!file.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(idCacheFile).absolutePath());
        if (!file.open(QFile::WriteOnly)) {
            mDebug("MThemeImagesDirectory") << "Failed to save id cache file" << path << "to" << idCacheFile;
            return;
        }
    }

    QDataStream stream(&file);
    stream << ID_CACHE_VERSION;
    stream << (uint)sb.st_mtime;
    stream << ids;
    file.close();
}
#endif

QString MThemeImagesDirectory::createIdCacheFilename(const QString &filePath) const
{
    QString filePathEncoded(filePath);
    filePathEncoded.replace(QLatin1Char('_'), QLatin1String("__"));
    filePathEncoded.replace(QLatin1Char('/'), QLatin1String("_."));
    return MThemeDaemon::systemThemeCacheDirectory() + QLatin1String("ids") + QDir::separator() + filePathEncoded;
}

MImageDirectory::MImageDirectory(const QString &path, M::RecursionMode recursionMode)
{
    QList<QDir> directories;
    directories.append(QDir(path));

    // loop as long as there's directories left.
    while (!directories.isEmpty()) {
        QDir dir = directories.takeFirst();
        if (!dir.exists()) {
            mWarning("MThemeDaemon") << "directory" << dir.absolutePath() << "does not exist!";
            continue;
        }

        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();
        // go trough all files in this directory.
        for (; i != end; ++i) {

            if (i->isDir() && recursionMode == M::Recursive) {
                directories.append(QDir(i->absoluteFilePath()));
            } else if (i->suffix() == QLatin1String("png") || i->suffix() == QLatin1String("jpg")) {
                if (imageResources.contains(i->baseName())) {
                    mDebug("MThemeDaemon") << "Path" << path << "contains multiple images with id" << i->baseName();
                } else {
                    imageResources.insert(i->baseName(), new PixmapImageResource(i->absoluteFilePath()));
                }
            } else if (i->suffix() == QLatin1String("svg")) {
                svgFiles.insert(i->absoluteFilePath());
            }
        }
    }
}

MImageDirectory::~MImageDirectory()
{
    qDeleteAll(imageResources);
    imageResources.clear();
    svgFiles.clear();
    notFoundIds.clear();
}

ImageResource *MImageDirectory::findImage(const QString &imageId)
{
    ImageResource *resource = imageResources.value(imageId, NULL);

    if (!resource) {

        // is this image already marked as not found?
        if (notFoundIds.contains(imageId)) {
            return NULL;
        }

        // it was not resolved, so we need to go trough all svg-files.
        foreach (const QString &svgFile, svgFiles) {
            QSvgRenderer renderer(svgFile);
            // does this svg contain the element we're looking for?
            if (renderer.isValid() && renderer.elementExists(imageId)) {
                resource = new SvgImageResource(imageId, svgFile);
                imageResources.insert(imageId, resource);
                break;
            }
        }
    }

    if (!resource) {
        // mark the image id as not found, so next time someone is asking this, we don't need to go trough all svg-files
        notFoundIds.insert(imageId);
    }

    return resource;
}

QStringList MImageDirectory::activeImageIds() const
{
    return imageResources.keys();
}

  // the C string operations
namespace
{     // the only purpose of this function is using new (and avoiding using C free() )
    char *strDup(const char *src)
    {
        unsigned l = 0;
        for (const char *i = src; *i; i++)
            l++;

        char *ret = new char[l + 1];
        memcpy(ret, src, l);
        ret[l] = 0;

        return ret;
    }

    char *appendToPath(const char *basepath, const char *subdir)
    {
        unsigned l = 0;
        for (const char *i = basepath; *i; i++)
            l++;
        for (const char *i = subdir; *i; i++)
            l++;

        char *ret = new char[l + 2];
        char *dest = ret;
        for (const char *s = basepath; *s; s++)
            *(dest++) = *s;

        *(dest++) = '/';

        for (const char *s = subdir; *s; s++)
            *(dest++) = *s;

        *dest = 0;

        return ret;
    }
}

