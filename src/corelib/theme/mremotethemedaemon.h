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

#ifndef MREMOTETHEMEDAEMON_H
#define MREMOTETHEMEDAEMON_H

#include <QLocalSocket>
#include "imthemedaemon.h"

class MRemoteThemeDaemonPrivate;

class MRemoteThemeDaemon : public IMThemeDaemon
{
    Q_OBJECT
public:
    MRemoteThemeDaemon(const QString &applicationName, int timeout);
    virtual ~MRemoteThemeDaemon();
    bool connected() const;

public:
    virtual void registerApplicationName(const QString &applicationName);
    // directory search list contains list of directories to search for pixmaps for application
    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, M::RecursionMode recursive);
    virtual void clearPixmapSearchList();

    virtual void pixmapHandleSync(const QString &imageId, const QSize &size);
    virtual void pixmapHandle(const QString &imageId, const QSize &size);
    virtual void releasePixmap(const QString &imageId, const QSize &size);

    virtual QString currentTheme();

    virtual QStringList themeInheritanceChain();
    virtual QStringList themeLibraryNames();
    virtual bool hasPendingRequests() const;

private:
    Q_DECLARE_PRIVATE(MRemoteThemeDaemon)
    MRemoteThemeDaemonPrivate *const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void connectionDataAvailable())
};

#endif
