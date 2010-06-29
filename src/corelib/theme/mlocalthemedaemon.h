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

#ifndef MLOCALTHEMEDAEMON_H
#define MLOCALTHEMEDAEMON_H

#include "imthemedaemon.h"
#include "mthemedaemon.h"
#include "mthemedaemonclient.h"
#include <QHash>
#include <QBuffer>

#ifdef HAVE_GCONF
#include "mgconfitem.h"
#endif

class MLocalThemeDaemon : public IMThemeDaemon
{
    Q_OBJECT
public:
    MLocalThemeDaemon(const QString &applicationName);
    ~MLocalThemeDaemon();

    virtual void registerApplicationName(const QString &applicationName);
    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, M::RecursionMode recursive);
    virtual void clearPixmapSearchList();

    virtual void pixmapHandleSync(const QString &imageId, const QSize &size);
    virtual void pixmapHandle(const QString &imageId, const QSize &size);
    virtual void releasePixmap(const QString &imageId, const QSize &size);

    virtual QString currentTheme();

    virtual QStringList themeInheritanceChain();
    virtual QStringList themeLibraryNames();

    virtual bool hasPendingRequests() const;

private slots:
    void themeChangedSlot();
    void localeChanged();

private:
    MThemeDaemonClient *client;
    MThemeDaemon daemon;
#ifdef HAVE_GCONF
    MGConfItem themeItem;
    MGConfItem locale;
#endif
};
#endif
