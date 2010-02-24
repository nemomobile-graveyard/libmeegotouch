/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duicommonpixmaps.h"
#include "duithemedaemon.h"
#include <QFile>
#include <QDir>

using namespace Dui::DuiThemeDaemonProtocol;


#define VERSION(major, minor) ((major << 16) | minor)
const unsigned int PRELOAD_FILE_VERSION = VERSION(0, 1);

DuiCommonPixmaps::DuiCommonPixmaps(DuiThemeDaemon *daemon) :
    minRequestsForCache(0),
    daemon(daemon)
{
    connect(&cpuMonitor, SIGNAL(newCpuFrameAvailable()), SLOT(loadOne()));
}

DuiCommonPixmaps::~DuiCommonPixmaps()
{
    // please call clear before destroying this object
    Q_ASSERT(mostUsedPixmaps.count() == 0);
}

void DuiCommonPixmaps::clear()
{
    // release all most used pixmaps
    foreach(const PixmapIdentifier & id, mostUsedPixmaps) {
        if (toLoadList.contains(id))
            continue;

        ImageResource *resource = daemon->findImageResource(id.imageId);
        resource->releasePixmap(id.size);
    }

    cpuMonitor.stop();
    mostUsedPixmaps.clear();
    toLoadList.clear();
    requestCounts.clear();
    minRequestsForCache = 0;
}

bool DuiCommonPixmaps::load(const QString &filename)
{
    // clear the old ones.
    clear();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream stream(&file);

    unsigned int version;
    stream >> version;
    if (version != PRELOAD_FILE_VERSION)
        return false;

    QString path = cachePath();

    while (file.bytesAvailable()) {
        QString imageId;
        QSize size;
        quint32 requestCount;
        bool isMostUsed;
        stream >> imageId >> size >> requestCount >> isMostUsed;

        PixmapIdentifier id(imageId, size);
        requestCounts.insert(id, requestCount);
        if (isMostUsed) {
            bool resourceLoaded = false;
            QFile pixmapFile(path + id.imageId + '(' + QString::number(id.size.width()) + ',' + QString::number(id.size.height()) + ')');
            if(pixmapFile.open(QIODevice::ReadOnly)) {
                // find this resource
                ImageResource* resource = daemon->findImageResource(imageId);
                if(resource) {
                    // try to load pre-rasterized pixmap from file
                    resourceLoaded = resource->load(&pixmapFile, size);
                }
                pixmapFile.close();
            }
            // if there was no pre-rasterized pixmap for this resource, 
            // it will be added to load list.
            if(!resourceLoaded) {
                toLoadList.insert(PixmapIdentifier(imageId, size));
            }
            mostUsedPixmaps.insert(PixmapIdentifier(imageId, size));
        }
    }

    if (!toLoadList.isEmpty()) {
        cpuMonitor.start(2000);
    }

    file.close();
    return true;
}

bool DuiCommonPixmaps::save(const QString &filename) const
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream << PRELOAD_FILE_VERSION;

    QHash<PixmapIdentifier, quint32>::const_iterator i = requestCounts.begin();

    QString path = cachePath();

    for (; i != requestCounts.end(); ++i) {
        const PixmapIdentifier& id = i.key();

        bool isMostUsed = mostUsedPixmaps.contains(id);
        stream << id.imageId << id.size << i.value() << isMostUsed;
        if(isMostUsed) {
            QFile pixmapFile(path + id.imageId + '(' + QString::number(id.size.width()) + ',' + QString::number(id.size.height()) + ')');
            if(!pixmapFile.exists()) {
                if(pixmapFile.open(QIODevice::WriteOnly)) {
                    ImageResource* resource = daemon->findImageResource(id.imageId);
                    if(resource && resource->save(&pixmapFile, id.size)) {
                        pixmapFile.close();
                    } else {
                        pixmapFile.remove();
                    }
                }
            }
        }
    }

    file.close();
    return true;
}

void DuiCommonPixmaps::loadOne()
{
    // stop the timer, so we can adjust the frequency depending on the usage
    cpuMonitor.stop();

    if (0 <= cpuMonitor.usage() || cpuMonitor.usage() < 10) {

        PixmapIdentifier id = *toLoadList.begin();
        toLoadList.erase(toLoadList.begin());
        if (!toLoadList.isEmpty()) {
            // there's still items in the list, so start the timer with small delay
            cpuMonitor.start(250);
        }

        ImageResource *resource = daemon->findImageResource(id.imageId);
        Q_ASSERT_X(resource, "DuiCommonPixmaps", "Theme daemon could not find resource while loading most used pixmaps! Please re-install current theme or clear the preload.list file");
        resource->fetchPixmap(id.size);

    } else {
        // the cpu usage was too high, so start start the timer with longer delay
        cpuMonitor.start(2000);
    }
}

void DuiCommonPixmaps::increaseRequestCount(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id, ImageResource *resource)
{
    QHash<PixmapIdentifier, quint32>::iterator requestCount = requestCounts.find(id);
    if (requestCount == requestCounts.end()) {
        requestCount = requestCounts.insert(id, 0);
    }
    ++requestCount.value();

    // does this pixmap has higher request count value than the current minimum for cache?
    if (requestCount.value() > minRequestsForCache && !mostUsedPixmaps.contains(id)) {

        // this pixmap might end up to mostUsedPixmaps list

        // check if there's still room for this pixmap
        if (mostUsedPixmaps.count() < DuiCommonPixmaps::CacheSize) {
            // yep, just add this pixmap and return
            resource->fetchPixmap(id.size);
            mostUsedPixmaps.insert(id);
            return;
        }

        // there was no room, so we'll check if we can make it
        QSet<PixmapIdentifier>::iterator i = mostUsedPixmaps.begin();
        QSet<PixmapIdentifier>::iterator leastUsed = i;
        quint32 leastUsedRequests = requestCounts[*leastUsed];
        quint32 secondlyLeastUsedRequests = leastUsedRequests;
        ++i;

        // find the least used pixmap from most used list
        for (; i != mostUsedPixmaps.end(); ++i) {
            const PixmapIdentifier &curId = *i;
            quint32 count = requestCounts[curId];
            if (count < leastUsedRequests) {
                secondlyLeastUsedRequests = leastUsedRequests;
                leastUsedRequests = count;
                leastUsed = i;
            }
        }

        // if the least used is still above the current, we'll just update the limit
        if (leastUsedRequests >= requestCount.value()) {
            minRequestsForCache = leastUsedRequests;
            return;
        }

        // otherwise we have a new pixmap for the list

        // update the limit, there may be duplicate request counts in the most used list
        minRequestsForCache = (secondlyLeastUsedRequests > requestCount.value()) ? requestCount.value() : secondlyLeastUsedRequests;

        // allocate one pixmap for the list
        resource->fetchPixmap(id.size);

        // release the old one from the list
        if (!toLoadList.remove(*leastUsed)) {
            // resource was loaded
            resource = daemon->findImageResource(leastUsed->imageId);
            resource->releasePixmap(leastUsed->size);
        }

        remove(*leastUsed);
        mostUsedPixmaps.insert(id);
    }
}

void DuiCommonPixmaps::reload(const PixmapIdentifier &id, ImageResource *oldResource)
{
    if (toLoadList.contains(id) || !mostUsedPixmaps.contains(id)) {
        // no need to do anything
        return;
    }

    oldResource->releasePixmap(id.size);
    toLoadList.insert(id);
}

void DuiCommonPixmaps::remove(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id)
{
    // path to theme-specific cache
    QString path = cachePath() +
                   id.imageId + '(' + QString::number(id.size.width()) + ',' + QString::number(id.size.height()) + ')';

    // remove pixmap file from disk if there is one
    if(QFile::exists(path)) {
        QFile::remove(path);
    }

    mostUsedPixmaps.remove(id);
}

QString DuiCommonPixmaps::cachePath() const
{
    return DuiThemeDaemon::systemThemeCacheDirectory() + QDir::separator() +
           daemon->currentTheme() + QDir::separator();
}

