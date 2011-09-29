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

#ifndef MIMAGEDIRECTORY_H
#define MIMAGEDIRECTORY_H


#include "mpixmaphandle.h"

#include <QString>
#include <QSharedPointer>
#include <QSvgRenderer>
#include <QHash>
#include <QLinkedList>
#include <QSet>
#include <QSize>
#include <QImage>
#include <QColor>
#include <mnamespace.h>
#include <QDebug>
#include <QObject>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <sys/mman.h>
#include <QtMeeGoGraphicsSystemHelper>
#endif

class QPixmap;
class QFileInfo;
class QFile;


#ifndef NOIMAGEDIRECTORIES
#include "inotifywrapper/inotify.h"
#endif


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
    friend class Ut_MImageDirectory;
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

    bool saveToFsCache(QImage &image, const QSize &size);
    bool saveToFsCache(QImage &image, const QSize &size, const QString &uniqueKey);

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

    void deleteCacheFile();

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


#ifndef NOIMAGEDIRECTORIES // omit the class from imgcachegen


  // a hack to cope with Qt moc failure - MThemeImagesDirectory slot needs to
  // have INotify::Watch declared which is done in inotify.hh that's used only
  // on linux
#ifndef __linux__
class INotify
{
public:
    class Watch;
};
#endif


class MThemeImagesDirectory : public QObject
{
    Q_OBJECT

    friend class Ut_MImageDirectory;
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
#ifndef __linux__  // stick with old Qt-based implementation
    bool loadIdsFromCache(const QFileInfo& svgFileInfo, bool localized = false);
    void saveIdsInCache(const QStringList& ids, const QFileInfo& svgFileInfo) const;
#else
    bool loadIdsFromCache(const char *path, bool localized = false);
    void saveIdsInCache(const QStringList& ids, const char *path) const;
#endif

    QString createIdCacheFilename(const QString &filePath) const;

#ifdef __linux__
    INotify::Watch *readImageResources(const QString& path, bool localized = false);
#else
    void readImageResources(const QString& path, bool localized = false);
#endif
    void readSvgResources(const QString& path, bool localized = false);

#ifdef __linux__
    void addImageResource(const char *path, const char *filename, bool localized, char *basename = 0, const char *ldot = 0);
    void addSvgResource(const char *path, bool localized);

      // params: self, path (filename with full path), filename (filename without path, in same buf as path), localized
    typedef void (*FileCrawlerHandler)(MThemeImagesDirectory *, const char *, const char *, bool);
    INotify::Watch *crawlImageDirectory(FileCrawlerHandler handler, const QString& path, bool localized);

    static void imageCrawlerHandler(MThemeImagesDirectory *self, const char *path, const char *filename, bool localized);
    static void svgCrawlerHandler(MThemeImagesDirectory *self, const char *path, const char *filename, bool localized);
#else
    void addImageResource(const QFileInfo& fileInfo, bool localized);
    void addSvgResource(const QFileInfo& fileInfo, bool localized);
#endif

    // image id => image resource
    QHash<QString, ImageResource *> imageResources;
    // image id => svg filename
    QMultiHash<QString, QString> idsInSvgImages;

    QHash<QString, ImageResource *> localizedImageResources;
    QMultiHash<QString, QString> idsInLocalizedSvgImages;

    QString m_path;
    QString m_locale;

#ifdef __linux__
    INotify::Watch *m_imageWatch;
    INotify::Watch *m_iconWatch;

    INotify::Watch *m_localizedImageWatch;
    INotify::Watch *m_localizedIconWatch;

    QLinkedList< QPair<INotify::Watch *, char *> > newDirQueue;

    static const unsigned inflags;
#endif

    // We have to expose the slots for all platforms as moc is not processing
    // anything inside #ifdef __linux__ blocks (unless someone adds
    // DEFINES += __linux__ to any of project (include) files which is quite
    // dirty hack). If this ever gets fixed on Qt side, move the private slots
    // inside previous #ifdef __linux__ block and remove hack INotify::Watch
    // declaration for non-__linux__ platforms.
private slots:
    void inEvent(INotify::Watch *w, const char *fn, unsigned evmask);

    void processNewDirQueue();
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


class ImageDirectoryData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool localized READ localized)

public:
    ImageDirectoryData(bool localized = false);

    bool localized() const;

private:
    bool m_localized;
};
//! \internal_end

#endif // NOIMAGEDIRECTORIES

#endif
