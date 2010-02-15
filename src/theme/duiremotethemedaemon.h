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

#ifndef DUIREMOTETHEMEDAEMON_H
#define DUIREMOTETHEMEDAEMON_H

#include <QLocalSocket>
#include "iduithemedaemon.h"

class DuiRemoteThemeDaemonPrivate;

class DuiRemoteThemeDaemon : public IDuiThemeDaemon
{
    Q_OBJECT
public:
    DuiRemoteThemeDaemon(const QString &applicationName, int timeout);
    virtual ~DuiRemoteThemeDaemon();
    bool connected() const;

public:
    // directory search list contains list of directories to search for pixmaps for application
    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, Dui::RecursionMode recursive);
    virtual void clearPixmapSearchList();

    virtual void pixmapHandleSync(const QString &imageId, const QSize &size);
    virtual void pixmapHandle(const QString &imageId, const QSize &size);
    virtual void releasePixmap(const QString &imageId, const QSize &size);

    virtual QString currentTheme();

    virtual QStringList themeInheritanceChain();
    virtual bool hasPendingRequests() const;

private:
    Q_DECLARE_PRIVATE(DuiRemoteThemeDaemon)
    DuiRemoteThemeDaemonPrivate *const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void connectionDataAvailable())
};

#endif
