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

#ifndef MIMAGEDIRECTORY_H
#define MIMAGEDIRECTORY_H

#include "mpixmaphandle.h"

#include <QString>
#include <QSharedPointer>
#include <QSvgRenderer>
#include <QHash>
#include <QSet>
#include <QSize>
#include <QImage>
#include <QColor>
#include <mnamespace.h>
#include <QDebug>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <sys/mman.h>
#include <QtMeeGoGraphicsSystemHelper>
#endif

class QPixmap;
class QFileInfo;
class QFile;


typedef QPixmap RenderSurface;
uint qHash(const QSize &size);

//! \internal
struct PixmapCacheEntry {
    PixmapCacheEntry() :
        pixmap(NULL),
        refCount(0) {}

    ~PixmapCacheEntry() {
        if (refCount != 0) {
            qCritical("Freeing PixmapCacheEntry with a non-zero reference count. This very likely leads to a crash.");
        }

        if (!handle.shmHandle.isEmpty()) {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
            if (!handle.directMap)
                shm_unlink(handle.shmHandle.constData());
#endif
        }

        if (handle.eglHandle) {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
            QMeeGoGraphicsSystemHelper::destroyEGLSharedImage(handle.eglHandle);
#endif
        }

        if (pixmap) {
            delete pixmap;
        }
    }

    QPixmap *pixmap;
    MPixmapHandle handle;
    unsigned int refCount;
};

class ImageResource
{
public:
    ImageResource(const QString& absoluteFilePath);
    virtual ~ImageResource();

    // creates a cache entry if there's no such yet, otherwise just increase the refCount
    MPixmapHandle fetchPixmap(const QSize &size);
    // decreases refCount and releases the pixmap if refCount dropped to zero
    void releasePixmap(const QSize &size);

    PixmapCacheEntry* releaseWithoutDelete(const QSize &size);

    // returns a handle to pixmap, without increasing the refCount
    MPixmapHandle pixmapHandle(const QSize &size);
    
    QString absoluteFilePath()
    { return filePath; }

    QHash<QSize, const PixmapCacheEntry*> pixmapCacheEntries() const;

    void applyDebugColors(QImage *image);

    void saveToFsCache(QImage &image, const QSize &size);
    void saveToFsCache(QImage &image, const QSize &size, const QString &uniqueKey);

protected:
    virtual QImage createPixmap(const QSize &size) = 0;
    // must not return an empty string
    virtual QString uniqueKey() = 0;
    virtual bool shouldBeCached();

private:
    void pixmapFromImage(PixmapCacheEntry *cacheEntry ,const QImage image);
    QImage loadFromFsCache(const QSize& size, PixmapCacheEntry *cacheEntry);
    static QString createCacheFilename(const QSize& size, const QString &cacheKey);

    void fillCacheEntry(PixmapCacheEntry *cacheEntry, QImage &image, const QSize &size);

#ifdef  Q_WS_X11
    bool convertToX11;
#endif
    QString filePath;
    // pixmaps created from this image resource
    QHash<QSize, PixmapCacheEntry*> cachedPixmaps;

    uchar *buffer;
    QFile *cacheFile;
};

class IconImageResource : public ImageResource
{
public:
    IconImageResource(const QString &absoluteFilePath) :
        ImageResource(absoluteFilePath) {}
    virtual ~IconImageResource() {
    }

protected:
    virtual QImage createPixmap(const QSize &size);
    virtual QString uniqueKey();
};

class PixmapImageResource : public ImageResource
{
public:
    PixmapImageResource(const QString &absoluteFilePath) :
        ImageResource(absoluteFilePath) {}
    virtual ~PixmapImageResource() {
    }

protected:
    virtual QImage createPixmap(const QSize &size);
    virtual QString uniqueKey();
    virtual bool shouldBeCached();
};

class SvgImageResource : public ImageResource
{
public:
    SvgImageResource(const QString &imageId, const QString& absoluteFilePath) :
        ImageResource(absoluteFilePath), imageId(imageId) {}

    virtual ~SvgImageResource() {}

protected:
    virtual QImage createPixmap(const QSize &size);
    virtual QString uniqueKey();
private:
    QString imageId;
};

class MThemeImagesDirectory
{
public:
    static const QString pixmapsDir;
    static const QString iconsDir;
    static const QString svgDir;

    MThemeImagesDirectory(const QString &path, const QString &locale = QString());
    ~MThemeImagesDirectory();

    ImageResource *findImage(const QString &imageId);

    bool isLocalizedResource(const QString &imageId) const;

    void reloadLocalizedResources(const QString &locale);
    
    QString path() const;
    QString locale() const;
    
private:
    bool loadIdsFromCache(const QFileInfo& svgFileInfo, bool localized = false);
    void saveIdsInCache(const QStringList& ids, const QFileInfo& svgFileInfo) const;
    QString createIdCacheFilename(const QString &filePath) const;

    void readImageResources(const QString& path, bool localized = false);
    void readSvgResources(const QString& path, bool localized = false);
    void addImageResource(const QFileInfo& fileInfo, bool localized);
    void addSvgResource(const QFileInfo& fileInfo, bool localized);

    // image id => image resource
    QHash<QString, ImageResource *> imageResources;
    // image id => svg filename
    QMultiHash<QString, QString> idsInSvgImages;

    QHash<QString, ImageResource *> localizedImageResources;
    QMultiHash<QString, QString> idsInLocalizedSvgImages;

    QString m_path;
    QString m_locale;
};

class MImageDirectory
{
public:

    MImageDirectory(const QString &path, M::RecursionMode recursionMode);

    ~MImageDirectory();

    ImageResource *findImage(const QString &imageId);

    QStringList activeImageIds() const;
private:

    // image id => image resource
    QHash<QString, ImageResource *> imageResources;

    QSet<QString> svgFiles;

    QSet<QString> notFoundIds;
};
//! \internal_end

#endif
