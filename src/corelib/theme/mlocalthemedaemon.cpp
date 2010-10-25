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

#include "mlocalthemedaemon.h"
#include "mthemedaemonprotocol.h"
#include "mdebug.h"
#include "mpixmaphandle.h"

using namespace M::MThemeDaemonProtocol;


#ifndef HAVE_GCONF
#include <QTimer>
#include "mexport.h"
QString M_themeName;
MLocalThemeDaemon *M_localDaemon;

void M_CORE_EXPORT M_changeTheme(const QString &theme)
{
    M_themeName = theme;
    QTimer::singleShot(0, M_localDaemon, SLOT(themeChangedSlot()));
}
#endif

MLocalThemeDaemon::MLocalThemeDaemon(const QString &applicationName) :
    daemon(MThemeDaemon::LocalDaemon)
#ifdef HAVE_GCONF
    ,
    themeItem("/meegotouch/theme/name"),
    locale("/meegotouch/i18n/language")
#endif
{
    QString theme =  M_THEME_DEFAULT;
    QString language = "";
#if HAVE_GCONF
    theme = themeItem.value(M_THEME_DEFAULT).toString();
    language = locale.value("en_GB").toString();
#else
    M_themeName =  M_THEME_DEFAULT;
    M_localDaemon = this;
#endif

    QList<PixmapCacheEntry*> pixmapsToDelete;
    if ( daemon.activateTheme(theme, language, QList<MThemeDaemonClient *>(), pixmapsToDelete) == false ) {
        mWarning("MThemeDaemon") << "Could not activate the theme:" << theme;
    }
    Q_ASSERT(pixmapsToDelete.isEmpty());

    client = new MThemeDaemonClient(NULL, applicationName, daemon.themeInheritanceChain());
    daemon.addClient(client);

#ifdef HAVE_GCONF
    connect(&themeItem, SIGNAL(valueChanged()), SLOT(themeChangedSlot()));
    connect(&locale, SIGNAL(valueChanged()), SLOT(localeChanged()));
#endif
}

void MLocalThemeDaemon::registerApplicationName(const QString &applicationName)
{
    client->reinit(applicationName, daemon.themeInheritanceChain());
}

MLocalThemeDaemon::~MLocalThemeDaemon()
{
    daemon.removeClient(client);
    delete client;
    client = NULL;
}

void MLocalThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName, M::RecursionMode recursive)
{
    client->addCustomImageDirectory(directoryName, recursive);
}

void MLocalThemeDaemon::clearPixmapSearchList()
{
    client->removeAddedImageDirectories();
}

void MLocalThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    // request
    MPixmapHandle handle;
    if (daemon.pixmap(client, PixmapIdentifier(imageId, size), &handle)) {
        emit pixmapCreatedOrChanged(imageId, size, handle);
    } else {
        qDebug() << "error";
    }
}

void MLocalThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    pixmapHandleSync(imageId, size);
}

void MLocalThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    if (!daemon.releasePixmap(client, PixmapIdentifier(imageId, size))) {
        qDebug() << "error";
    }
}

QString MLocalThemeDaemon::currentTheme()
{
    return daemon.currentTheme();
}

QStringList MLocalThemeDaemon::themeInheritanceChain()
{
    return daemon.themeInheritanceChain();
}

QStringList MLocalThemeDaemon::themeLibraryNames()
{
    return daemon.themeLibraryNames();
}

bool MLocalThemeDaemon::hasPendingRequests() const
{
    return false;
}

void MLocalThemeDaemon::themeChangedSlot()
{
    QHash<MThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    QList<MThemeDaemonClient *> list;
    list.append(client);

    QList<PixmapCacheEntry*> pixmapsToDelete;
#ifdef HAVE_GCONF
    if (daemon.activateTheme(themeItem.value().toString(), locale.value().toString(), list, pixmapsToDelete)) {
#else
    if (daemon.activateTheme(M_themeName, "", list, pixmapsToDelete)) {
#endif
        while (!client->pixmapsToReload.isEmpty()) {
            PixmapIdentifier id = client->pixmapsToReload.takeLast();
            pixmapHandle(id.imageId, id.size);
        }
        // theme change succeeded, let's inform all clients + add the pixmaps to load-list
        emit themeChanged(themeInheritanceChain(), themeLibraryNames());
        qDeleteAll(pixmapsToDelete);
    } else {
        // theme change failed, so change the theme back also in gconf.
#ifdef HAVE_GCONF
        mWarning("MLocalThemeDaemon") << "Could not change theme to" << themeItem.value().toString();
        themeItem.set(currentTheme());
#else
        mWarning("MLocalThemeDaemon") << "Could not change theme to" << M_themeName;
        M_themeName = currentTheme();
#endif
    }
    emit themeChangeCompleted();
}

void MLocalThemeDaemon::localeChanged()
{
#ifdef HAVE_GCONF
    QHash<MThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    QList<MThemeDaemonClient *> clients;
    clients.append(client);
    daemon.changeLocale(locale.value().toString(), clients, pixmapsToReload);

    foreach(const PixmapIdentifier & id, pixmapsToReload[client]) {
        MPixmapHandle handle;
        if (daemon.pixmap(client, id, &handle)) {
            emit pixmapCreatedOrChanged(id.imageId, id.size, handle);
        } else {
            qDebug() << "error";
        }
    }
#endif
}

