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

#ifndef DUITHEMEDAEMON_H
#define DUITHEMEDAEMON_H

#include <QObject>
#include "duithemedaemonprotocol.h"
#include "duicommonpixmaps.h"

class DuiThemeDaemonClient;

//! \internal
class DuiThemeDaemon
{
public:
    DuiThemeDaemon();
    virtual ~DuiThemeDaemon();

    static QString systemThemeDirectory();
    static QString systemThemeCacheDirectory();

    void addClient(DuiThemeDaemonClient *client);
    void removeClient(DuiThemeDaemonClient *client);

    bool pixmap(DuiThemeDaemonClient *client, const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id, Qt::HANDLE &handle);
    bool releasePixmap(DuiThemeDaemonClient *client, const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id);

    bool activateTheme(const QString &newTheme, const QString &locale, const QList<DuiThemeDaemonClient *>& clientList, QHash<DuiThemeDaemonClient *, QList<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> >& pixmapsToReload);

    void changeLocale(const QString &newLocale, const QList<DuiThemeDaemonClient *>& clientList, QHash<DuiThemeDaemonClient *, QList<Dui::DuiThemeDaemonProtocol::PixmapIdentifier> >& pixmapsToReload);

    QStringList themeInheritanceChain() const;
    QStringList themeLibraryNames() const;

    QString currentTheme() const;

    ImageResource *findImageResource(const QString &imageId);
private:

    void reloadImagePaths(const QString &locale);
private:
    DuiCommonPixmaps mostUsedPixmaps;
    QString currentThemeName;
    QStringList themeInheritance;
    QStringList themeLibraries;
    QList<DuiThemeImagesDirectory *> themeImageDirs;
};
//! \internal_end
#endif

