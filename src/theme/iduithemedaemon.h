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

#ifndef IDUITHEMEDAEMON_H
#define IDUITHEMEDAEMON_H

#include <QObject>
#include <QSize>
#include <QColor>
#include "duinamespace.h"

class IDuiThemeDaemon : public QObject
{
    Q_OBJECT

public:
    virtual ~IDuiThemeDaemon() {}

    // directory search list contains list of directories to search for pixmaps for application
    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, Dui::RecursionMode recursive) = 0;
    virtual void clearPixmapSearchList() = 0;

    // this method is only for copying pixmaps for user, this is really slow
    virtual void pixmapHandleSync(const QString &imageId, const QSize &size) = 0;
    // returns valid handle to a pixmap, which is not yet loaded
    virtual void pixmapHandle(const QString &imageId, const QSize &size) = 0;

    virtual void releasePixmap(const QString &imageId, const QSize &size) = 0;

    // ask theme daemon to change the theme or get a list of available themes
    virtual QString currentTheme() = 0;

    virtual QStringList themeInheritanceChain() = 0;

    virtual bool hasPendingRequests() const = 0;

signals:
    void pixmapChanged(const QString &imageId, const QSize &size, Qt::HANDLE newHandle);

    void pixmapCreated(const QString &imageId, const QSize &size, Qt::HANDLE handle);

    // emitted when theme has been changed
    void themeChanged(const QStringList &themeInheritance);

};

#endif
