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

#include "mthemedaemonclient.h"

#include "mimagedirectory.h"
#include "mthemedaemon.h"

#include <QIODevice>
#include <QDir>
#include <QPixmap>

MThemeDaemonClient::MThemeDaemonClient(QIODevice *socket, const QString &clientName, const QStringList &themes):
    dataStream(socket)
{
    reinit(clientName, themes);
}

MThemeDaemonClient::~MThemeDaemonClient()
{
    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();
    qDeleteAll(customImageDirs);
    customImageDirs.clear();
}

QString MThemeDaemonClient::name() const
{
    return clientName;
}

bool MThemeDaemonClient::reinit(const QString &clientName, const QStringList &themes)
{
    if (this->clientName == clientName && this->themes == themes) {
        return false;
    }
    this->clientName = clientName;
    this->themes = themes;

    reloadImagePaths(themes);

    return true;
}

QDataStream &MThemeDaemonClient::stream()
{
    return dataStream;
}

ImageResource *MThemeDaemonClient::findImageResource(const QString &imageId)
{
    ImageResource *resource = NULL;

    // search the image first from manualy added image directories
    foreach(MImageDirectory * imdir, customImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            return resource;
    }

    // not found, so search from all theme directory
    foreach(MThemeImagesDirectory * imdir, themeImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            return resource;
    }

    return NULL;
}

void MThemeDaemonClient::reloadImagePaths(const QStringList &themes)
{
    toBeDeletedThemeImageDirs.append(themeImageDirs);
    themeImageDirs.clear();

    foreach(const QString & theme, themes) {
        themeImageDirs.append(new MThemeImagesDirectory(theme + "meegotouch" + QDir::separator() + clientName));
    }
}

void MThemeDaemonClient::addCustomImageDirectory(const QString &path, M::RecursionMode recursionMode)
{
    customImageDirs.insert(0, new MImageDirectory(path, recursionMode));
}

QStringList MThemeDaemonClient::removeAddedImageDirectories()
{
    QStringList stillActiveIds;
    foreach(MImageDirectory * imdir, customImageDirs) {
	stillActiveIds.append(imdir->activeImageIds());
    }
    if(stillActiveIds.isEmpty()) {
	qDeleteAll(customImageDirs);
	customImageDirs.clear();
    }
    return stillActiveIds;
}

void MThemeDaemonClient::themeChangeApplied()
{
    qDeleteAll(toBeDeletedThemeImageDirs);
    toBeDeletedThemeImageDirs.clear();
}
