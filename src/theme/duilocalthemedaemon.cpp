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

#include "duilocalthemedaemon.h"
#include "duithemedaemonprotocol.h"
#include "duidebug.h"

using namespace Dui::DuiThemeDaemonProtocol;


#ifndef HAVE_GCONF
#include <QTimer>
#include "duiexport.h"
QString Dui_themeName;
DuiLocalThemeDaemon *Dui_localDaemon;

void DUI_EXPORT Dui_changeTheme(const QString &theme)
{
    Dui_themeName = theme;
    QTimer::singleShot(0, Dui_localDaemon, SLOT(themeChangedSlot()));
}
#endif

DuiLocalThemeDaemon::DuiLocalThemeDaemon(const QString &applicationName)
#ifdef HAVE_GCONF
    : themeItem("/Dui/theme/name"),
      locale("/Dui/i18n/Language")
#endif
{
    QString theme = "devel";
    QString language = "";
#if HAVE_GCONF
    theme = themeItem.value("devel").toString();
    language = locale.value().toString();
#else
    Dui_themeName = "devel";
    Dui_localDaemon = this;
#endif

    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    daemon.activateTheme(theme, language, QList<DuiThemeDaemonClient *>(), pixmapsToReload);
    Q_ASSERT(pixmapsToReload.isEmpty());

    client = new DuiThemeDaemonClient(NULL, applicationName, daemon.themeInheritanceChain());
    daemon.addClient(client);

#ifdef HAVE_GCONF
    connect(&themeItem, SIGNAL(valueChanged()), SLOT(themeChangedSlot()));
    connect(&locale, SIGNAL(valueChanged()), SLOT(localeChanged()));
#endif
}

DuiLocalThemeDaemon::~DuiLocalThemeDaemon()
{
    daemon.removeClient(client);
    delete client;
    client = NULL;
}

void DuiLocalThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName, Dui::RecursionMode recursive)
{
    client->addCustomImageDirectory(directoryName, recursive);
}

void DuiLocalThemeDaemon::clearPixmapSearchList()
{
    client->removeAddedImageDirectories();
}

void DuiLocalThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    // request
    Qt::HANDLE handle;
    if (daemon.pixmap(client, PixmapIdentifier(imageId, size), handle)) {
        emit pixmapCreated(imageId, size, handle);
    } else {
        qDebug() << "error";
    }
}

void DuiLocalThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    pixmapHandleSync(imageId, size);
}

void DuiLocalThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    if (!daemon.releasePixmap(client, PixmapIdentifier(imageId, size))) {
        qDebug() << "error";
    }
}

QString DuiLocalThemeDaemon::currentTheme()
{
    return daemon.currentTheme();
}

QStringList DuiLocalThemeDaemon::themeInheritanceChain()
{
    return daemon.themeInheritanceChain();
}

QStringList DuiLocalThemeDaemon::themeLibraryNames()
{
    return daemon.themeLibraryNames();
}

bool DuiLocalThemeDaemon::hasPendingRequests() const
{
    return false;
}

void DuiLocalThemeDaemon::themeChangedSlot()
{
    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    QList<DuiThemeDaemonClient *> list;
    list.append(client);

#ifdef HAVE_GCONF
    if (daemon.activateTheme(themeItem.value().toString(), locale.value().toString(), list, pixmapsToReload)) {
#else
    if (daemon.activateTheme(Dui_themeName, "", list, pixmapsToReload)) {
#endif

        QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator i = pixmapsToReload.begin();
        QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >::iterator end = pixmapsToReload.end();
        for (; i != end; ++i) {
            const QList<PixmapIdentifier>& ids = i.value();

            foreach(const PixmapIdentifier & id, ids) {
                pixmapHandle(id.imageId, id.size);
            }
        }
        // theme change succeeded, let's inform all clients + add the pixmaps to load-list
        emit themeChanged(themeInheritanceChain(), themeLibraryNames());
    } else {
        // theme change failed, so change the theme back also in gconf.
#ifdef HAVE_GCONF
        duiWarning("DuiLocalThemeDaemon") << "Could not change theme to" << themeItem.value().toString();
        themeItem.set(currentTheme());
#else
        duiWarning("DuiLocalThemeDaemon") << "Could not change theme to" << Dui_themeName;
        Dui_themeName = currentTheme();
#endif
    }
}

void DuiLocalThemeDaemon::localeChanged()
{
#ifdef HAVE_GCONF
    QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> > pixmapsToReload;
    QList<DuiThemeDaemonClient *> clients;
    clients.append(client);
    daemon.changeLocale(locale.value().toString(), clients, pixmapsToReload);

    foreach(const PixmapIdentifier & id, pixmapsToReload[client]) {
        Qt::HANDLE handle;
        if (daemon.pixmap(client, id, handle)) {
            emit pixmapChanged(id.imageId, id.size, handle);
        } else {
            qDebug() << "error";
        }
    }
#endif
}

