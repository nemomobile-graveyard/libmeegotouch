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
#include "mthemeresourcemanager.h"
#include "mthemedaemon.h"

#include "mdebug.h"
#include <QDir>
#include <QPainter>
#include <QPixmap>

#ifdef  Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif

namespace {
    const unsigned int ID_CACHE_VERSION = 1;
    const unsigned int IMAGE_CACHE_VERSION = 1;
}

uint qHash(const QSize &size)
{
    return (size.width() & 0x0000ffff) | (size.height() << 16);
}

Qt::HANDLE ImageResource::fetchPixmap(const QSize &size)
{
    PixmapCacheEntry &cacheEntry = cachedPixmaps[size];

    if (!cacheEntry.pixmap) {
        // try to load it from filesystem cache
        cacheEntry.pixmap = loadFromFsCache(size);

        if (!cacheEntry.pixmap) {
            // we didn't have the correctly sized pixmap in cache, so we need to create one.
            cacheEntry.pixmap = createPixmap(size);
            saveToFsCache(cacheEntry.pixmap, size);
        }

#ifdef  Q_WS_X11
        // Sync X-Server, without this the pixmap handle is still invalid in client-side
        XSync(QX11Info::display(), false);
#endif
    }

    ++cacheEntry.refCount;
#ifdef  Q_WS_X11
//#ifndef Q_WS_MAC  - #MS - windows build: This function is X11 specific; using it is non-portable.
    return cacheEntry.pixmap->handle();
#else
    return cacheEntry.pixmap;
#endif
}

void ImageResource::releasePixmap(const QSize &size)
{
    Q_ASSERT_X(cachedPixmaps.contains(size), "ImageResource", "Cannot release pixmap because the cache entry cannot be found for the pixmap!");

    PixmapCacheEntry &cacheEntry = cachedPixmaps[size];

    // decrease the refcount.
    --cacheEntry.refCount;

    // if this was the last reference, release the pixmap
    if (cacheEntry.refCount == 0) {

        // release the pixmap
        delete cacheEntry.pixmap;

        // remove the cache entry from this resource
        cachedPixmaps.remove(size);
    }
}

QPixmap* ImageResource::releaseWithoutDelete(const QSize &size)
{
    Q_ASSERT_X(cachedPixmaps.contains(size), "ImageResource", "Cannot release pixmap because the cache entry cannot be found for the pixmap!");

    PixmapCacheEntry &cacheEntry = cachedPixmaps[size];

    // decrease the refcount.
    --cacheEntry.refCount;

    // if this was the last reference, release the pixmap
    if (cacheEntry.refCount == 0) {

        QPixmap* pixmap = cacheEntry.pixmap;

        // remove the cache entry from this resource
        cachedPixmaps.remove(size);

        return pixmap;
    }
    return NULL;
}

Qt::HANDLE ImageResource::pixmapHandle(const QSize &size)
{
    QHash<QSize, PixmapCacheEntry>::iterator it = cachedPixmaps.find(size);
    if (it == cachedPixmaps.end()) {
        return 0;
    } else {
#if defined(Q_WS_X11)
        return cachedPixmaps[size].pixmap->handle();
#else
        return &(cachedPixmaps[size].pixmap);
#endif
    }
}

QPixmap *ImageResource::loadFromFsCache(const QSize& size)
{
    const QString cacheFileName = createCacheFilename(size);
    if (cacheFileName.isEmpty()) {
        return 0;
    }

    if (QFile::exists(cacheFileName)) {
        QFile file(cacheFileName);
        if (file.open(QFile::ReadOnly)) {
            QDataStream stream(&file);
            uint version;
            stream >> version;
            if (version != IMAGE_CACHE_VERSION) {
                // will be replaced with up to date version
                file.close();
                return 0;
            }
            uint timestamp;
            stream >> timestamp;
            QFileInfo fileInfo(absoluteFilePath());
            if (timestamp != fileInfo.lastModified().toTime_t()) {
                // will be replaced with up to date version
                file.close();
                return 0;
            }

            QPixmap *pixmap = new QPixmap();
            stream >> *pixmap;
#ifdef  Q_WS_X11
            // Sync X-Server, without this the pixmap handle is still invalid in client-side
            XSync(QX11Info::display(), false);
#endif
            file.close();
            return pixmap;
        } else {
            mWarning("ImageResource") << "Failed to load pixmap from chache" << cacheFileName;
        }
        return 0;
    }

    return 0;
}

void ImageResource::saveToFsCache(const QPixmap* pixmap, const QSize& size)
{
    const QString cacheFileName = createCacheFilename(size);
    if (cacheFileName.isEmpty()) {
        return;
    }

    QFile file(cacheFileName);
    if (!file.open(QFile::WriteOnly)) {
        //Maybe it failed because the directory doesn't exist
        QDir().mkpath(QFileInfo(cacheFileName).absolutePath());
        if (!file.open(QFile::WriteOnly)) {
            mWarning("ImageResource") << "Failed to save cache file for" << absoluteFilePath() << "to" << cacheFileName;
        return;
        }
    }

    QDataStream stream(&file);
    stream << IMAGE_CACHE_VERSION;
    QFileInfo fileInfo(absoluteFilePath());
    stream << fileInfo.lastModified().toTime_t();
    stream << *pixmap;

    file.close();
}

QString ImageResource::createCacheFilename(const QSize& size)
{
    QString cacheKey = uniqueKey();
    if (cacheKey.isEmpty()) {
        return QString();
    }
    cacheKey.replace(QLatin1Char('_'), QLatin1String("__"));
    cacheKey.replace(QLatin1Char('/'), QLatin1String("_."));
    cacheKey += '(' + QString::number(size.width()) + QLatin1Char(',') + QString::number(size.height()) + QLatin1Char(')');

    return MThemeDaemon::systemThemeCacheDirectory() + QLatin1String("images/") + cacheKey;
}

QPixmap *PixmapImageResource::createPixmap(const QSize &size)
{
    QImage image(absoluteFilePath());

    if (size.isNull() || size == image.size()) {
        // the requested size is (0,0) or the original image is the same size as the requested one.
        // we can use the original image
        return new QPixmap(QPixmap::fromImage(image));
    }

    // we need to resize the image.
    return new QPixmap(QPixmap::fromImage(image.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//Qt::FastTransformation)));
}

QString PixmapImageResource::uniqueKey()
{
    // caching does not really make sense, return empty id
    return QString();
}

QPixmap *IconImageResource::createPixmap(const QSize &size)
{
    QSvgRenderer* renderer = MThemeResourceManager::instance().svgRenderer(absoluteFilePath());

    QSize svgImageSize = size;
    if (size.isNull()) {
        if (renderer) {
            svgImageSize = renderer->defaultSize();
        } else if (svgImageSize.isNull()) {
            mWarning("IconImageResource") << "    Invalid svg, size of the document is (0,0)";
        }
    }

    QPixmap *pixmap = new QPixmap(svgImageSize);
    pixmap->fill(QColor(Qt::transparent));
    if (renderer) {
        QPainter painter(pixmap);
        renderer->render(&painter);
    }

    return pixmap;
}

QString IconImageResource::uniqueKey()
{
    return absoluteFilePath();
}

QPixmap *SvgImageResource::createPixmap(const QSize &size)
{
    QSvgRenderer* renderer = MThemeResourceManager::instance().svgRenderer(absoluteFilePath());

    QSize svgImageSize = size;
    if (size.isNull()) {
        // the requested size is (0,0) so we need to fetch the default size from the svg.
        if (renderer) {
            svgImageSize = renderer->boundsOnElement(imageId).size().toSize();
        } else if (svgImageSize.isNull()) {
            mWarning("SvgImageResource") << "    Invalid svg, size of id" << imageId << "is (0,0)";
        }
    }

    QPixmap *pixmap = new QPixmap(svgImageSize);
    pixmap->fill(QColor(Qt::transparent));
    if (renderer) {
        QPainter painter(pixmap);
        renderer->render(&painter, imageId);
    }

    return pixmap;
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

    m_locale = locale;
    QString localePath = m_path + QDir::separator() + "locale" + QDir::separator() + m_locale + QDir::separator();
    if (m_locale.isEmpty() || !QFile::exists(localePath)) {
        return;
    }

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
            } else if (i->suffix() == "png" || i->suffix() == "jpg" || i->suffix() == "svg") {
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
            } else if (i->suffix() == "svg") {
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
            imageResources.insert(fileInfo.baseName(), fileInfo.suffix() == "svg" ? (ImageResource*) new IconImageResource(fileInfo.absoluteFilePath()) : (ImageResource*) new PixmapImageResource(fileInfo.absoluteFilePath()));
        }
    }
    else {
        //add localized image resource only if same named resource was found from the original theme 
        if (!imageResources.contains(fileInfo.baseName()) && !idsInSvgImages.contains(fileInfo.baseName())) {
            mWarning("MThemeDaemon") << "Ignoring localized image resource" << fileInfo.absoluteFilePath() << "because it was not found from the original theme!";
        } else {
            //if "svg" add IconImageResource, if "jpg" or "png" add PixmapImageResource
            localizedImageResources.insert(fileInfo.baseName(), fileInfo.suffix() == "svg" ? (ImageResource*) new IconImageResource(fileInfo.absoluteFilePath()) : (ImageResource*) new PixmapImageResource(fileInfo.absoluteFilePath()));
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
    return MThemeDaemon::systemThemeCacheDirectory() + QLatin1String("ids/") + filePathEncoded;
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
            } else if (i->suffix() == "png" || i->suffix() == "jpg") {
                if (imageResources.contains(i->baseName())) {
                    mDebug("MThemeDaemon") << "Path" << path << "contains multiple images with id" << i->baseName();
                } else {
                    imageResources.insert(i->baseName(), new PixmapImageResource(i->absoluteFilePath()));
                }
            } else if (i->suffix() == "svg") {
                svgFiles.insert(i->absoluteFilePath(), QSharedPointer<QSvgRenderer>());
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
        QHash< QString, QSharedPointer<QSvgRenderer> >::iterator i = svgFiles.begin();
        QHash< QString, QSharedPointer<QSvgRenderer> >::iterator end = svgFiles.end();

        for (; i != end; ++i) {
            QSvgRenderer* renderer = MThemeResourceManager::instance().svgRenderer(i.key());
            // does this svg contain the element we're looking for?
            if (renderer->elementExists(imageId)) {
                resource = new SvgImageResource(imageId, i.key());
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

