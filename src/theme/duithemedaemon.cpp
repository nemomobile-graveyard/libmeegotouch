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

#include "duithemedaemon.h"

#include <QSvgRenderer>
#include <DuiDebug>
#include <QPixmap>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <QColor>
#include <QCoreApplication>
#include <QStringList>
#include <DuiGConfItem>
#include <QPainter>
#include "duithemedaemonclient.h"
#include "duiimagedirectory.h"
#include "duithemedaemonprotocol.h"
#include "duithemeresourcemanager.h"

using namespace Dui::DuiThemeDaemonProtocol;

DuiThemeDaemon::DuiThemeDaemon() :
    mostUsedPixmaps(this)
{
    // check that the base theme ("fallback") directory exists.
    QDir baseThemeDir(systemThemeDirectory() + QDir::separator() + "base" + QDir::separator() +
                      "dui" + QDir::separator() + "libdui" + QDir::separator() + "style");
    if (!baseThemeDir.exists()) {
        // we don't have the base theme ("fallback") directory, so exit!
        qFatal("DuiThemeDaemon - base theme directory not found: %s. Please (re)install duitheme package.",
               qPrintable(baseThemeDir.absolutePath()));
    }
}


DuiThemeDaemon::~DuiThemeDaemon()
{
    if (!mostUsedPixmaps.save(systemThemeCacheDirectory() + QDir::separator() + currentThemeName + QDir::separator() + "preload.list")) {
        // TODO: print out warning
    }
    mostUsedPixmaps.clear();

    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();
}

QString DuiThemeDaemon::systemThemeDirectory()
{
#ifdef Q_OS_WIN
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    appDir.cd("themes");
    return appDir.absolutePath();
#else
    return THEMEDIR;
#endif
}

QString DuiThemeDaemon::systemThemeCacheDirectory()
{
#ifdef Q_WS_X11
    return "/var/cache/dui/themedaemon";
#else
    return QDir::tempPath();
#endif
}

void DuiThemeDaemon::addClient(DuiThemeDaemonClient *client)
{
    Q_UNUSED(client);
}

void DuiThemeDaemon::removeClient(DuiThemeDaemonClient *client)
{
    // release all pixmaps
    while (client->pixmaps.count() > 0) {
        PixmapIdentifier id = client->pixmaps.begin().key();
        releasePixmap(client, id);
    }
}


bool DuiThemeDaemon::pixmap(DuiThemeDaemonClient *client, const PixmapIdentifier &id, Qt::HANDLE &handle)
{
    // has the client already requested this pixmap?
    if (client->pixmaps.contains(id)) {
        duiWarning("DuiThemeDaemon") << "    The requested pixmap" << id.imageId << "is already loaded for the client" << client->name();
        return false;
    }

    // try to find the image resource from the client specific paths
    ImageResource *resource = client->findImageResource(id.imageId);
    if (!resource) {
        // application paths didn't contain the image resource, now check from theme
        resource = findImageResource(id.imageId);
        if (!resource) {
            duiWarning("DuiThemeDaemon") << "    The requested pixmap" << id.imageId << "was not found for client" << client->name();
            client->pixmaps.insert(id, NULL);
            handle = 0;
            return true;
        } else {
            mostUsedPixmaps.increaseRequestCount(id, resource);
        }
    }

    // save the information that this client has a reference of this pixmap
    client->pixmaps.insert(id, resource);

    // we have found the correct image resource.
    handle = resource->fetchPixmap(id.size);
    return true;
}

bool DuiThemeDaemon::releasePixmap(DuiThemeDaemonClient *client, const PixmapIdentifier &id)
{
    QHash<PixmapIdentifier, ImageResource *>::iterator i = client->pixmaps.find(id);
    if (i == client->pixmaps.end()) {
        // the client has no reference to this pixmap
        duiWarning("DuiThemeDaemon") << "    Pixmap" << id.imageId << "was not loaded by the client" << client->name();
        return false;
    }

    ImageResource *resource = i.value();
    client->pixmaps.erase(i);
    if (resource == NULL) {
        // the pixmap was an invalid pixmap, no need to release anything
        return true;
    }

    // find a cache entry for the pixmap to be released.
    resource->releasePixmap(id.size);
    return true;
}


QString DuiThemeDaemon::currentTheme() const
{
    return currentThemeName;
}

QStringList DuiThemeDaemon::themeInheritanceChain() const
{
    return themeInheritance;
}

QStringList DuiThemeDaemon::themeLibraryNames() const
{
    return themeLibraries;
}

ImageResource *DuiThemeDaemon::findImageResource(const QString &imageId)
{
    ImageResource *resource = NULL;
    // try to find the image first from current theme,
    // then from theme that the current one is inherited from, and so on.
    foreach(DuiThemeImagesDirectory * imdir, themeImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            break;
    }

    return resource;
}

void DuiThemeDaemon::reloadImagePaths(const QString &locale)
{
    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();

    foreach(const QString & theme, themeInheritance) {
        themeImageDirs.append(new DuiThemeImagesDirectory(theme + "dui", locale));
    }
}





const QSettings *themeFile(const QString &theme)
{
    // Determine whether this is a dui theme:
    // step 1: we need to have index.theme file
    const QString themeIndexFileName = DuiThemeDaemon::systemThemeDirectory() + QDir::separator() + theme + QDir::separator() + "index.theme";
    if (!QFile::exists(themeIndexFileName))
        return NULL;

    // step 2: it needs to be a valid ini file
    const QSettings *themeIndexFile = new QSettings(themeIndexFileName, QSettings::IniFormat);
    if (themeIndexFile->status() != QSettings::NoError) {
        delete themeIndexFile;
        return NULL;
    }

    // step 3: we need to have X-DUI-Metatheme group in index.theme
    if (!themeIndexFile->childGroups().contains(QString("X-DUI-Metatheme"))) {
        delete themeIndexFile;
        return NULL;
    }
    return themeIndexFile;
}


bool DuiThemeDaemon::activateTheme(const QString &newTheme, const QString &locale, const QList<DuiThemeDaemonClient *>& clientList, QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >& pixmapsToReload)
{
    if (newTheme == currentThemeName) {
        // TODO: check need for warning
        return false;
    }

    QDir dir(systemThemeDirectory() + QDir::separator() + newTheme);
    if (!dir.exists()) {
        duiWarning("DuiThemeDaemon") << "Theme" << newTheme << "does not exist! Not changing theme";
        return false;
    }

#ifdef DUITHEME_PRINT_DEBUG
    duiDebug("DuiThemeDaemon") << "Changing theme from" << currentThemeName << "to" << newTheme;
#endif

    // Change the theme

    // 1: find out the inheritance chain for the new theme
    QString tmpTheme = newTheme;
    QStringList newThemeInheritanceChain;
    QSet<QString> themeLibraryNames;

    while (true) {
        const QSettings *themeIndexFile = themeFile(tmpTheme);
        if (!themeIndexFile) {
            // TODO: print out warning, theme inheritance contains invalid theme
            return false;
        }

        newThemeInheritanceChain.append(tmpTheme);
        QString parentTheme = themeIndexFile->value("X-DUI-Metatheme/X-Inherits", "").toString();

        QStringList libraryNames = themeIndexFile->value("X-DUI-Metatheme/X-Libraries", "").toString().split(",", QString::SkipEmptyParts);
        foreach(const QString& libname, libraryNames) {
            themeLibraryNames.insert(libname.trimmed());
        }

        delete themeIndexFile;

        if (tmpTheme == "base")
            break;
        tmpTheme = parentTheme;
        if (tmpTheme.isEmpty()) {
            // TODO: print out warning, no base theme for this theme.
            return false;
        }

        // check that there is no cyclic dependencies
        foreach(const QString & themeName, newThemeInheritanceChain) {
            if (tmpTheme == themeName) {
                duiWarning("DuiThemeDaemon") << "Cyclic dependency in theme:" << newTheme;
                return false;
            }
        }
    }

    themeLibraries = themeLibraryNames.toList();
    themeInheritance = newThemeInheritanceChain;

#ifdef DUITHEME_PRINT_DEBUG
    duiDebug("DuiThemeDaemon") << "    New theme inheritance chain is" << themeInheritance;
#endif

    // include the path to theme inheritance chain
    for (int i = 0; i != themeInheritance.count(); ++i) {
        themeInheritance[i] = systemThemeDirectory() + QDir::separator() + themeInheritance[i] + QDir::separator();
    }

    // 2. save the most used list for the old theme
    if (!currentThemeName.isEmpty()) {
        if (!mostUsedPixmaps.save(systemThemeCacheDirectory() + QDir::separator() + currentThemeName + QDir::separator() + "preload.list")) {
            // TODO: print out warning
        }
    }

    // 3. release all most used pixmaps
    mostUsedPixmaps.clear();

    currentThemeName = newTheme;

    // 4: go trough all clients and release all pixmaps + put them into load queue
    // plus send a packet for all clients, so they can start releasing and requesting new pixmaps, (so the daemon wouldn't need to reload all pixmaps that aren't needed anymore)

    foreach(DuiThemeDaemonClient * client, clientList) {

        QList<PixmapIdentifier> pixmapsToBeReloaded = client->pixmaps.keys();

        foreach(const PixmapIdentifier & id, pixmapsToBeReloaded) {
            releasePixmap(client, id);
        }

        pixmapsToReload.insert(client, pixmapsToBeReloaded);

        client->reloadImagePaths(themeInheritance);
    }

    DuiThemeResourceManager::instance().themeChanged();

    // 5: release rest of the old theme resources and load the new ones
    reloadImagePaths(locale);

    // 6. load the "preload" list
    if (!mostUsedPixmaps.load(systemThemeCacheDirectory() + QDir::separator() + currentThemeName + QDir::separator() + "preload.list")) {
        // TODO: print out warning
    }

    return true;
}

void DuiThemeDaemon::changeLocale(const QString &newLocale, const QList<DuiThemeDaemonClient *>& clientList, QHash<DuiThemeDaemonClient *, QList<PixmapIdentifier> >& pixmapsToReload)
{
    QList<QPair<DuiThemeDaemonClient *, QList<QPair<PixmapIdentifier, ImageResource *> > > > potentialIconsFromClients;

    foreach(DuiThemeDaemonClient * client, clientList) {

        QHash<PixmapIdentifier, ImageResource *>::iterator i = client->pixmaps.begin();
        QHash<PixmapIdentifier, ImageResource *>::iterator end = client->pixmaps.end();

        QList<QPair<PixmapIdentifier, ImageResource *> > potentialReloadlist;

        QList<PixmapIdentifier> pixmapsToBeReloaded;

        // get all icons from this client
        for (; i != end; ++i) {
            // is this image an icon?
            if (dynamic_cast<IconImageResource *>(i.value())) {

                bool reload = false;
                foreach(DuiThemeImagesDirectory * imdir, themeImageDirs) {
                    if (imdir->isLocaleSpecificImage(i.key().imageId)) {
                        reload = true;
                        break;
                    }
                }

                if (reload) {
                    // this image was locale specific icon so mark it to be reloaded
                    pixmapsToBeReloaded.append(i.key());
                    mostUsedPixmaps.reload(i.key(), i.value());
                } else {
                    // this image was icon but not locale specific so mark it as potential
                    potentialReloadlist.append(qMakePair(i.key(), i.value()));
                }
            }
        }

        // release all old locale specific icons
        foreach(const PixmapIdentifier & id, pixmapsToBeReloaded) {
            releasePixmap(client, id);
        }

        pixmapsToReload.insert(client, pixmapsToBeReloaded);
        potentialIconsFromClients.append(qMakePair(client, potentialReloadlist));
    }

    // load the new locale specific images
    foreach(DuiThemeImagesDirectory * imdir, themeImageDirs) {
        imdir->reloadLocaleSpecificImages(newLocale);
    }

    typedef QPair<PixmapIdentifier, ImageResource *> pi_ir;
    typedef QPair<DuiThemeDaemonClient *, QList<pi_ir> > c_pi_ir;

    foreach(const c_pi_ir & potentialIconsFromClient, potentialIconsFromClients) {
        foreach(const pi_ir & potentialIcon, potentialIconsFromClient.second) {

            foreach(DuiThemeImagesDirectory * imdir, themeImageDirs) {
                if (imdir->isLocaleSpecificImage(potentialIcon.first.imageId)) {
                    releasePixmap(potentialIconsFromClient.first, potentialIcon.first);
                    mostUsedPixmaps.reload(potentialIcon.first, potentialIcon.second);
                    pixmapsToReload[potentialIconsFromClient.first].append(potentialIcon.first);
                    break;
                }
            }
        }
    }
}
