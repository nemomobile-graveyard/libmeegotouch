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

#ifndef MTHEMEDAEMON_H
#define MTHEMEDAEMON_H
//This is setuping base as the default theme for themedaemon if a wrong/emtpy value is at gconf key
#define M_THEME_DEFAULT "base"

#include <QObject>
#include "mthemedaemonprotocol.h"
#include "mcommonpixmaps.h"

class MThemeDaemonClient;

//! \internal
class MThemeDaemon
{
public:
    enum Type {
        LocalDaemon = 0,
        RemoteDaemon = 1
    };

    MThemeDaemon(Type type);
    virtual ~MThemeDaemon();

    static QString systemThemeDirectory();
    static QString systemThemeCacheDirectory();

    void addClient(MThemeDaemonClient *client);
    void removeClient(MThemeDaemonClient *client);

    bool pixmap(MThemeDaemonClient *client, const M::MThemeDaemonProtocol::PixmapIdentifier &id, MPixmapHandle *handle);
    bool releasePixmap(MThemeDaemonClient *client, const M::MThemeDaemonProtocol::PixmapIdentifier &id);

    bool activateTheme(const QString &newTheme, const QString &locale, const QList<MThemeDaemonClient *>& clientList, QList<PixmapCacheEntry*>& pixmapsToDelete, bool forceReload = false);

    void changeLocale(const QString &newLocale, const QList<MThemeDaemonClient *>& clientList, QHash<MThemeDaemonClient *, QList<M::MThemeDaemonProtocol::PixmapIdentifier> >& pixmapsToReload);

    QStringList themeInheritanceChain() const;
    QStringList themeLibraryNames() const;

    QString currentTheme() const;

    ImageResource *findImageResource(const QString &imageId);

    MCommonPixmaps mostUsedPixmaps;

private:

    void reloadImagePaths(const QString &locale, bool forceReload);
private:
    QString currentThemeName;
    QStringList themeInheritance;
    QStringList themeLibraries;
    QList<MThemeImagesDirectory *> themeImageDirs;
    Type type;
};
//! \internal_end
#endif

