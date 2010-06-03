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

#include "mdebug.h"
#include <QDir>
#include <QPainter>
#include <QPixmap>

#ifdef  Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif

uint qHash(const QSize &size)
{
    return (size.width() & 0x0000ffff) | (size.height() << 16);
}

Qt::HANDLE ImageResource::fetchPixmap(const QSize &size)
{
    PixmapCacheEntry &cacheEntry = cachedPixmaps[size];

    if (!cacheEntry.pixmap) {
        // we didn't have the correctly sized pixmap in cache, so we need to create one.
        cacheEntry.pixmap = createPixmap(size);

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
    Q_ASSERT(cachedPixmaps.contains(size));
#if defined(Q_WS_X11)
    return cachedPixmaps[size].pixmap->handle();
#else
    return &(cachedPixmaps[size].pixmap);
#endif
}

bool ImageResource::save(QIODevice* device, const QSize& size) const
{
    QHash<QSize, PixmapCacheEntry>::const_iterator iterator = cachedPixmaps.find(size);
    if(iterator == cachedPixmaps.constEnd())
        return false;

    return iterator.value().pixmap->save(device, "PNG");
}

bool ImageResource::load(QIODevice* device, const QSize& size)
{
    QPixmap* pixmap = new QPixmap();
    if(!pixmap->loadFromData(device->readAll(), "PNG")) {
        delete pixmap;
        return false;
    }

    PixmapCacheEntry &cacheEntry = cachedPixmaps[size];
    Q_ASSERT_X(!cacheEntry.pixmap, "ImageResource", "Tried to load pixmap that already exists.");
    cacheEntry.pixmap = pixmap;
    ++cacheEntry.refCount;

#ifdef  Q_WS_X11
    // Sync X-Server, without this the pixmap handle is still invalid in client-side
    XSync(QX11Info::display(), false);
#endif
    return true;
}

QPixmap *PixmapImageResource::createPixmap(const QSize &size)
{
    if (!image) {
        image = new QImage(absoluteFilePath);
    }

    if (size.isNull() || size == image->size()) {
        // the requested size is (0,0) or the original image is the same size as the requested one.
        // we can use the original image
        return new QPixmap(QPixmap::fromImage(*image));
    }

    // we need to resize the image.
    return new QPixmap(QPixmap::fromImage(image->scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));//Qt::FastTransformation)));
}

QPixmap *IconImageResource::createPixmap(const QSize &size)
{
    if (!renderer) {
        renderer = new QSvgRenderer(absoluteFilePath);
    }

    QSize svgImageSize = size;
    if (size.isNull()) {
        svgImageSize = renderer->defaultSize();
        if (svgImageSize.isNull()) {
            mWarning("IconImageResource") << "    Invalid svg, size of the document is (0,0)";
        }
    }

    QPixmap *pixmap = new QPixmap(svgImageSize);
    pixmap->fill(QColor(Qt::transparent));
    QPainter painter(pixmap);
    renderer->render(&painter);

    return pixmap;
}

QPixmap *SvgImageResource::createPixmap(const QSize &size)
{
    QSvgRenderer* renderer = MThemeResourceManager::instance().svgRenderer(absoluteFilePath);
    Q_ASSERT_X(renderer, "SvgImageResource", "SVG renderer not found");

    QSize svgImageSize = size;
    if (size.isNull()) {
        // the requested size is (0,0) so we need to fetch the default size from the svg.
        svgImageSize = renderer->boundsOnElement(imageId).size().toSize();
        if (svgImageSize.isNull()) {
            mWarning("SvgImageResource") << "    Invalid svg, size of id" << imageId << "is (0,0)";
        }
    }

    QPixmap *pixmap = new QPixmap(svgImageSize);
    pixmap->fill(QColor(Qt::transparent));
    QPainter painter(pixmap);
    renderer->render(&painter, imageId);

    return pixmap;
}

const QString MThemeImagesDirectory::pixmapsDir = "images";
const QString MThemeImagesDirectory::iconsDir = "icons";
const QString MThemeImagesDirectory::svgDir = "svg";


MThemeImagesDirectory::MThemeImagesDirectory(const QString &path, const QString &locale) :
    path(path)
{
    QList<QString> directories;
    // first go trough the pixmaps dir
    directories.append(path + QDir::separator() + pixmapsDir);
    while (!directories.isEmpty()) {
        QDir dir(directories.takeFirst());
        if (!dir.exists()) {
            continue;
        }
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();

        // go trough all files in this directory.
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == "png" || i->suffix() == "jpg") {
                if (imageIds.contains(i->baseName())) {
                    mDebug("MThemeDaemon") << "Path" << i->absolutePath() << "contains multiple images with id" << i->baseName();
                } else {
                    imageIds.insert(i->baseName(), new PixmapImageResource(i->absoluteFilePath()));
                }
            }
        }
    }

    // then go trough the icons dir
    directories.append(path + QDir::separator() + iconsDir);
    while (!directories.isEmpty()) {
        QDir dir(directories.takeFirst());
        if (!dir.exists()) {
            continue;
        }
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();

        // go trough all files in this directory.
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == "svg") {
                if (imageIds.contains(i->baseName())) {
                    mDebug("MThemeDaemon") << "Path" << path + QDir::separator() + "meegotouch" << "contains multiple images with id" << i->baseName();
                } else {
                    imageIds.insert(i->baseName(), new IconImageResource(i->absoluteFilePath()));
                }
            }
        }
    }

    // then go trough the svg dir
    directories.append(path + QDir::separator() + svgDir);
    while (!directories.isEmpty()) {
        QDir dir(directories.takeFirst());
        if (!dir.exists()) {
            continue;
        }
        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();

        // go trough all files in this directory.
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == "svg") {
                if(QFile::exists(i->absoluteFilePath() + ".ids")) {
                    // read file ids and create svg resources
                    QFile file(i->absoluteFilePath() + ".ids");
                    if(file.open(QIODevice::ReadOnly)) {
                        while(!file.atEnd()) {
                            QString id = file.readLine().trimmed();
                            if (imageIds.contains(id)) {
                                mDebug("MThemeDaemon") << "Path" << path << "contains multiple images with id" << id;
                            } else {
                                imageIds.insert(id, new SvgImageResource(id, i->absoluteFilePath()));
                            }
                        }
                    }
                } else {
                    mWarning("MThemeDaemon") << "SLOW: SVG file" << i->absoluteFilePath() << "doesn't have .ids file!";
                    // insert this to svg lookup list, this acts as a fallback to old implementation
                    svgFiles.insert(i->absoluteFilePath(), QSharedPointer<QSvgRenderer>());
                }
            }
        }
    }

    reloadLocaleSpecificImages(locale);
}

MThemeImagesDirectory::~MThemeImagesDirectory()
{
    qDeleteAll(imageIds);
    imageIds.clear();
    qDeleteAll(localeSpecificIcons);
    localeSpecificIcons.clear();
    svgFiles.clear();
    notFoundIds.clear();
}

ImageResource *MThemeImagesDirectory::findImage(const QString &imageId)
{
    // First check from locale specific icons
    ImageResource *resource = localeSpecificIcons.value(imageId, NULL);
    if (resource) {
        qDebug() << "Found icon" << imageId << "from locale";
    }

    if (!resource) {
        // Check if we have this id already resolved
        resource = imageIds.value(imageId, NULL);
    }

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
            Q_ASSERT_X(renderer, "SvgImageResource", "SVG renderer not found");

            // does this svg contain the element we're looking for?
            if (renderer->elementExists(imageId)) {
                resource = new SvgImageResource(imageId, i.key());
                imageIds.insert(imageId, resource);
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

bool MThemeImagesDirectory::isLocaleSpecificImage(const QString &imageId) const
{
    return localeSpecificIcons.contains(imageId);
}

void MThemeImagesDirectory::reloadLocaleSpecificImages(const QString &locale)
{
    qDeleteAll(localeSpecificIcons);
    localeSpecificIcons.clear();


    if (locale.isEmpty()) {
        return;
    }

    QString localeDir = path + QDir::separator() + "locale" + QDir::separator() + locale + QDir::separator() + iconsDir;
    if (!QFile::exists(localeDir)) {
        return;
    }

    QList<QString> directories;
    directories.append(localeDir);
    while (!directories.isEmpty()) {
        QDir dir(directories.takeFirst());

        QFileInfoList fileInfoList = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        QFileInfoList::const_iterator i = fileInfoList.constBegin();
        QFileInfoList::const_iterator end = fileInfoList.constEnd();
        // go trough all files in this directory.
        for (; i != end; ++i) {
            if (i->isDir()) {
                directories.append(i->absoluteFilePath());
            } else if (i->suffix() == "svg") {
                if (!imageIds.contains(i->baseName())) {
                    mWarning("MThemeDaemon") << "Path" << dir.absolutePath() << "contains imageId" << i->baseName() << "which was not found from the original theme!";
                } else {
                    localeSpecificIcons.insert(i->baseName(), new IconImageResource(i->absoluteFilePath()));
                }
            }
        }
    }
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
                if (imageIds.contains(i->baseName())) {
                    mDebug("MThemeDaemon") << "Path" << path << "contains multiple images with id" << i->baseName();
                } else {
                    imageIds.insert(i->baseName(), new PixmapImageResource(i->absoluteFilePath()));
                }
            } else if (i->suffix() == "svg") {
                svgFiles.insert(i->absoluteFilePath(), QSharedPointer<QSvgRenderer>());
            }
        }
    }
}

MImageDirectory::~MImageDirectory()
{
    qDeleteAll(imageIds);
    imageIds.clear();
    svgFiles.clear();
    notFoundIds.clear();
}

ImageResource *MImageDirectory::findImage(const QString &imageId)
{
    ImageResource *resource = imageIds.value(imageId, NULL);

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
            Q_ASSERT_X(renderer, "SvgImageResource", "SVG renderer not found");

            // does this svg contain the element we're looking for?
            if (renderer->elementExists(imageId)) {
                resource = new SvgImageResource(imageId, i.key());
                imageIds.insert(imageId, resource);
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

