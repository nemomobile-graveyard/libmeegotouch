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

#ifndef MCOMMONPIXMAPS_H
#define MCOMMONPIXMAPS_H

#include <QObject>
#include <QSet>
#include <QElapsedTimer>

#include "mcpumonitor.h"
#include "mimagedirectory.h"
#include "mthemedaemonprotocol.h"

class MThemeDaemon;

//! \internal
class MCommonPixmaps : public QObject
{
    Q_OBJECT
public:
    MCommonPixmaps(MThemeDaemon *daemon, bool loadMostUsed);
    ~MCommonPixmaps();
    void clear();
    void load();
    void save() const;

    void increaseRequestCount(const M::MThemeDaemonProtocol::PixmapIdentifier &id, ImageResource *resource);

    void reload(const M::MThemeDaemonProtocol::PixmapIdentifier &id, ImageResource *oldResource);

    QList<M::MThemeDaemonProtocol::PixmapHandle> mostUsedPixmapHandles();

Q_SIGNALS:
    void mostUsedPixmapsChanged(const M::MThemeDaemonProtocol::MostUsedPixmaps& mostUsed);

private slots:
    void loadOne();
    void updateClientsAboutMostUsed();
    void considerSaving();
private:
    QString cacheFilename() const;

    /*!
        \brief Number of pixmaps to keep in most used pixmaps cache
     */
    static const qint32 CacheSize = 1024;

    /*!
        \brief Current minimum amount of requests for a pixmap to end up in cache
     */
    quint32 minRequestsForCache;

    /*!
        \brief Counters for pixmaps
     */
    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, quint32> requestCounts;

    /*!
        \brief A list of pixmaps which are currently cached
     */
    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> mostUsedPixmaps;

    /*!
        \brief A list which contains most used pixmaps, which are not yet loaded
     */
    QSet<M::MThemeDaemonProtocol::PixmapIdentifier> toLoadList;

    /*!
        \brief A pointer to MThemeDaemonPrivate, so this class can find image resources from theme
     */
    MThemeDaemon *daemon;

    /*!
        \brief Monitors the cpu usage. Is used for loading pixmaps when the cpu has free time
     */
    MCpuMonitor cpuMonitor;

    QElapsedTimer timerSinceLastSave;
};
//! \internal_end

#endif
