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

#ifndef DUICOMMONPIXMAPS_H
#define DUICOMMONPIXMAPS_H

#include <QObject>
#include <QSet>
#include "duicpumonitor.h"
#include "duiimagedirectory.h"
#include "duithemedaemonprotocol.h"

class DuiThemeDaemon;

//! \internal
class DuiCommonPixmaps : public QObject
{
    Q_OBJECT
public:
    DuiCommonPixmaps(DuiThemeDaemon *daemon);
    ~DuiCommonPixmaps();
    void clear();
    bool load(const QString &filename);
    bool save(const QString &filename) const;

    void increaseRequestCount(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id, ImageResource *resource);

    void reload(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id, ImageResource *oldResource);

private slots:
    void loadOne();
private:
    /*!
        \brief Number of pixmaps to keep in most used pixmaps cache
     */
    static const qint32 CacheSize = 128;

    void remove(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id);
    QString cachePath() const;

    /*!
        \brief Current minimum amount of requests for a pixmap to end up in cache
     */
    quint32 minRequestsForCache;

    /*!
        \brief Counters for pixmaps
     */
    QHash<Dui::DuiThemeDaemonProtocol::PixmapIdentifier, quint32> requestCounts;

    /*!
        \brief A list of pixmaps which are currently cached
     */
    QSet<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> mostUsedPixmaps;

    /*!
        \brief A list which contains most used pixmaps, which are not yet loaded
     */
    QSet<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> toLoadList;

    /*!
        \brief A pointer to DuiThemeDaemonPrivate, so this class can find image resources from theme
     */
    DuiThemeDaemon *daemon;

    /*!
        \brief Monitors the cpu usage. Is used for loading pixmaps when the cpu has free time
     */
    DuiCpuMonitor cpuMonitor;
};
//! \internal_end

#endif
