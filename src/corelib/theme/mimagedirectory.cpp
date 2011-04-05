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

#include "mimagedirectory.h"
#include "mthemedaemon.h"
#include "mdebug.h"

#include "mgraphicssystemhelper.h"

#include <QDateTime>
#include <QDir>
#include <QPainter>
#include <QPixmap>

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
#ifdef  Q_WS_X11
    // When starting an application, the X11 window must be filled with a default background
    // image in a fast way. The filling can only be done fast if the background image is available
    // as X11 pixmap.
    convertToX11 = MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()
                   && absoluteFilePath.contains("forcex11");
#endif
}

MPixmapHandle ImageResource::fetchPixmap(const QSize &size)
{
    QHash<QSize, PixmapCacheEntry*>::iterator it = cachedPixmaps.find(size);
    PixmapCacheEntry *cacheEntry;
    if (it == cachedPixmaps.end()) {
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

                if (shouldBeCached()) {
                    saveToFsCache(image, size);
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
    if (cacheFile) {
        cacheFile->unmap(buffer);
        delete cacheFile;
    }
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
    Q_ASSERT_X(cachedPixmaps.contains(size), "ImageResource", "Cannot release pixmap because the cache entry cannot be found for the pixmap!");

    PixmapCacheEntry *cacheEntry = cachedPixmaps[size];

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

            cacheFile = new QFile(cacheFileName);
            if (!cacheFile->open(QFile::ReadOnly)) {
                delete cacheFile;
                cacheFile = 0;
                return QImage();
            }

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

void ImageResource::saveToFsCache(QImage &image, const QSize &size)
{
    saveToFsCache(image, size, uniqueKey());
}

void ImageResource::saveToFsCache(QImage &image, const QSize& size, const QString &uniqueKey)
{
    static bool failedCacheSaveAttempt = false;

    if (failedCacheSaveAttempt)
        return;

    const QString cacheFileName = createCacheFilename(size, uniqueKey);
    if (cacheFileName.isEmpty()) {
        return;
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
            return;
        }
    }

    // FIXME: Uncomment once the proper support for non-alpha images is ready.
    if (/*pixmap.format() != QImage::Format_RGB32 &&*/ image.format() != QImage::Format_ARGB32_Premultiplied) {
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }

    cache.write((const char*)image.constBits(), image.byteCount());
    cache.flush();
    cache.close();

    const QString cacheMetaFileName = cacheFileName + QLatin1String(".meta");
    QFile meta(cacheMetaFileName);
    if (!meta.open(QFile::WriteOnly)) {
        mWarning("ImageResource") <<
                 "Wrong permissions for cache directory" <<
                 MThemeDaemon::systemThemeCacheDirectory() <<
                 ". Cache is disabled.";
        failedCacheSaveAttempt = true;
        return;
    }

    QDataStream stream(&meta);
    stream << IMAGE_CACHE_VERSION;
    QFileInfo fileInfo(absoluteFilePath());
    stream << fileInfo.lastModified().toTime_t();
    stream << image.size();
    stream << image.format();
    meta.flush();
    meta.close();
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
}

void MThemeImagesDirectory::readSvgResources(const QString& path, bool localized)
{
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
}

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
    }
    else {
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
