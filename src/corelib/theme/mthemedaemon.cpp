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

#include "mthemedaemon.h"

#include <QSvgRenderer>
#include <MDebug>
#include <QPixmap>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <QColor>
#include <QCoreApplication>
#include <QStringList>
#include <MGConfItem>
#include <QPainter>
#include "mthemedaemonclient.h"
#include "mimagedirectory.h"
#include "mthemedaemonprotocol.h"
#include "mthemeresourcemanager.h"

using namespace M::MThemeDaemonProtocol;

MThemeDaemon::MThemeDaemon() :
    mostUsedPixmaps(this)
{
    // check that the base theme ("fallback") directory exists.
    QDir baseThemeDir(systemThemeDirectory() + QDir::separator() + "base" + QDir::separator() +
                      "meegotouch" + QDir::separator() + "libmeegotouchcore" + QDir::separator() + "style");
    if (!baseThemeDir.exists()) {
        // we don't have the base theme ("fallback") directory, so exit!
        qFatal("MThemeDaemon - base theme directory not found: %s. Please (re)install meegotouchtheme package.",
               qPrintable(baseThemeDir.absolutePath()));
    }
}


MThemeDaemon::~MThemeDaemon()
{
    if (!mostUsedPixmaps.save(systemThemeCacheDirectory() + QDir::separator() + currentThemeName + QDir::separator() + "preload.list")) {
        // TODO: print out warning
    }
    mostUsedPixmaps.clear();

    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();
}

QString MThemeDaemon::systemThemeDirectory()
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

QString MThemeDaemon::systemThemeCacheDirectory()
{
#ifdef Q_WS_X11
    return QString(CACHEDIR) + "themedaemon/";
#else
    return QDir::tempPath();
#endif
}

void MThemeDaemon::addClient(MThemeDaemonClient *client)
{
    Q_UNUSED(client);
}

void MThemeDaemon::removeClient(MThemeDaemonClient *client)
{
    // release all pixmaps
    while (client->pixmaps.count() > 0) {
        PixmapIdentifier id = client->pixmaps.begin().key();
        releasePixmap(client, id);
    }
}


bool MThemeDaemon::pixmap(MThemeDaemonClient *client, const PixmapIdentifier &id, Qt::HANDLE &handle)
{
    // has the client already requested this pixmap?
    if (client->pixmaps.contains(id)) {
        mWarning("MThemeDaemon") << "    The requested pixmap" << id.imageId << "is already loaded for the client" << client->name();
        return false;
    }

    // try to find the image resource from the client specific paths
    ImageResource *resource = client->findImageResource(id.imageId);
    if (!resource) {
        // application paths didn't contain the image resource, now check from theme
        resource = findImageResource(id.imageId);
        if (!resource) {
            mWarning("MThemeDaemon") << "    The requested pixmap" << id.imageId << "was not found for client" << client->name();
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

bool MThemeDaemon::releasePixmap(MThemeDaemonClient *client, const PixmapIdentifier &id)
{
    QHash<PixmapIdentifier, ImageResource *>::iterator i = client->pixmaps.find(id);
    if (i == client->pixmaps.end()) {
        // the client has no reference to this pixmap
        mWarning("MThemeDaemon") << "    Pixmap" << id.imageId << "was not loaded by the client" << client->name();
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


QString MThemeDaemon::currentTheme() const
{
    return currentThemeName;
}

QStringList MThemeDaemon::themeInheritanceChain() const
{
    return themeInheritance;
}

QStringList MThemeDaemon::themeLibraryNames() const
{
    return themeLibraries;
}

ImageResource *MThemeDaemon::findImageResource(const QString &imageId)
{
    ImageResource *resource = NULL;
    // try to find the image first from current theme,
    // then from theme that the current one is inherited from, and so on.
    foreach(MThemeImagesDirectory * imdir, themeImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            break;
    }

    return resource;
}

void MThemeDaemon::reloadImagePaths(const QString &locale)
{
    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();

    foreach(const QString & theme, themeInheritance) {
        themeImageDirs.append(new MThemeImagesDirectory(theme + "meegotouch", locale));
    }
}





const QSettings *themeFile(const QString &theme)
{
    // Determine whether this is a m theme:
    // step 1: we need to have index.theme file
    const QString themeIndexFileName = MThemeDaemon::systemThemeDirectory() + QDir::separator() + theme + QDir::separator() + "index.theme";
    if (!QFile::exists(themeIndexFileName))
        return NULL;

    // step 2: it needs to be a valid ini file
    const QSettings *themeIndexFile = new QSettings(themeIndexFileName, QSettings::IniFormat);
    if (themeIndexFile->status() != QSettings::NoError) {
        delete themeIndexFile;
        return NULL;
    }

    // step 3: we need to have X-MeeGoTouch-Metatheme group in index.theme

    // here we accept also the old X-DUI-Metatheme, because the old and new files
    // are in the same directory. remove "X-DUI-Metatheme" when libdui 0.19 and
    // duitheme with it is phased out.

    if ( (!themeIndexFile->childGroups().contains(QString("X-DUI-Metatheme")))
         && (!themeIndexFile->childGroups().contains(QString("X-MeeGoTouch-Metatheme"))) )
    {
        delete themeIndexFile;
        return NULL;
    }
    return themeIndexFile;
}


bool MThemeDaemon::activateTheme(const QString &newTheme, const QString &locale, const QList<MThemeDaemonClient *>& clientList, QHash<MThemeDaemonClient *, QList<PixmapIdentifier> >& pixmapsToReload, QList<QPixmap*>& pixmapsToDelete)
{
    if (newTheme == currentThemeName) {
        // TODO: check need for warning
        return false;
    }

    QDir dir(systemThemeDirectory() + QDir::separator() + newTheme);
    if (!dir.exists()) {
        mWarning("MThemeDaemon") << "Theme" << newTheme << "does not exist! Not changing theme";
        return false;
    }

#ifdef MTHEME_PRINT_DEBUG
    mDebug("MThemeDaemon") << "Changing theme from" << currentThemeName << "to" << newTheme;
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
        QString parentTheme = themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Inherits", "").toString();

        // remove when duitheme is phased out
        if ( parentTheme.isEmpty() )
        {
            parentTheme = themeIndexFile->value("X-DUI-Metatheme/X-Inherits", "").toString();
        }
        // end remove

        QStringList libraryNames = themeIndexFile->value("X-MeeGoTouch-Metatheme/X-Libraries", "").toString().split(",", QString::SkipEmptyParts);

        // remove when duitheme is phased out
        if ( libraryNames.isEmpty() )
        {
            libraryNames = themeIndexFile->value("X-DUI-Metatheme/X-Libraries", "")
                .toString().split(",", QString::SkipEmptyParts);
        }
        // end remove

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
                mWarning("MThemeDaemon") << "Cyclic dependency in theme:" << newTheme;
                return false;
            }
        }
    }

    themeLibraries = themeLibraryNames.toList();
    themeInheritance = newThemeInheritanceChain;

#ifdef MTHEME_PRINT_DEBUG
    mDebug("MThemeDaemon") << "    New theme inheritance chain is" << themeInheritance;
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

    foreach(MThemeDaemonClient * client, clientList) {

        QList<PixmapIdentifier> pixmapsToBeReloaded;
        QHash<PixmapIdentifier, ImageResource *>::iterator i;
        while(!client->pixmaps.isEmpty()) {
            i = client->pixmaps.begin();
            ImageResource* resource = i.value();
            PixmapIdentifier id = i.key();
            pixmapsToBeReloaded.append(id);
            client->pixmaps.erase(i);
            if(resource != NULL) {
                QPixmap* pixmap = resource->releaseWithoutDelete(id.size);
                if(pixmap != NULL) {
                    pixmapsToDelete.append(pixmap);
                }
            }
        }

        pixmapsToReload.insert(client, pixmapsToBeReloaded);

        client->reloadImagePaths(themeInheritance);
    }

    MThemeResourceManager::instance().themeChanged();

    // 5: release rest of the old theme resources and load the new ones
    reloadImagePaths(locale);

    // 6. load the "preload" list
    if (!mostUsedPixmaps.load(systemThemeCacheDirectory() + QDir::separator() + currentThemeName + QDir::separator() + "preload.list")) {
        // TODO: print out warning
    }

    return true;
}

void MThemeDaemon::changeLocale(const QString &newLocale, const QList<MThemeDaemonClient *>& clientList, QHash<MThemeDaemonClient *, QList<PixmapIdentifier> >& pixmapsToReload)
{
    QList<QPair<MThemeDaemonClient *, QList<QPair<PixmapIdentifier, ImageResource *> > > > potentialIconsFromClients;

    foreach(MThemeDaemonClient * client, clientList) {

        QHash<PixmapIdentifier, ImageResource *>::iterator i = client->pixmaps.begin();
        QHash<PixmapIdentifier, ImageResource *>::iterator end = client->pixmaps.end();

        QList<QPair<PixmapIdentifier, ImageResource *> > potentialReloadlist;

        QList<PixmapIdentifier> pixmapsToBeReloaded;

        // get all icons from this client
        for (; i != end; ++i) {
            // is this image an icon?
            if (dynamic_cast<IconImageResource *>(i.value())) {

                bool reload = false;
                foreach(MThemeImagesDirectory * imdir, themeImageDirs) {
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
    foreach(MThemeImagesDirectory * imdir, themeImageDirs) {
        imdir->reloadLocaleSpecificImages(newLocale);
    }

    typedef QPair<PixmapIdentifier, ImageResource *> pi_ir;
    typedef QPair<MThemeDaemonClient *, QList<pi_ir> > c_pi_ir;

    foreach(const c_pi_ir & potentialIconsFromClient, potentialIconsFromClients) {
        foreach(const pi_ir & potentialIcon, potentialIconsFromClient.second) {

            foreach(MThemeImagesDirectory * imdir, themeImageDirs) {
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
