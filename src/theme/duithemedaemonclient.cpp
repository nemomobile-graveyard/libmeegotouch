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

#include "duithemedaemonclient.h"

#include "duiimagedirectory.h"
#include "duithemedaemon.h"

#include <QIODevice>
#include <QDir>
#include <QPixmap>

DuiThemeDaemonClient::DuiThemeDaemonClient(QIODevice *socket, const QString &clientName, const QStringList &themes) :
    clientName(clientName),
    dataStream(socket)
{
    reloadImagePaths(themes);
}

DuiThemeDaemonClient::~DuiThemeDaemonClient()
{
    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();
    qDeleteAll(customImageDirs);
    customImageDirs.clear();
}

QString DuiThemeDaemonClient::name() const
{
    return clientName;
}

QDataStream &DuiThemeDaemonClient::stream()
{
    return dataStream;
}

ImageResource *DuiThemeDaemonClient::findImageResource(const QString &imageId)
{
    ImageResource *resource = NULL;

    // search the image first from manualy added image directories
    foreach(DuiImageDirectory * imdir, customImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            return resource;
    }

    // not found, so search from all theme directory
    foreach(DuiThemeImagesDirectory * imdir, themeImageDirs) {
        resource = imdir->findImage(imageId);
        if (resource)
            return resource;
    }

    return NULL;
}

void DuiThemeDaemonClient::reloadImagePaths(const QStringList &themes)
{
    qDeleteAll(themeImageDirs);
    themeImageDirs.clear();

    foreach(const QString & theme, themes) {
        themeImageDirs.append(new DuiThemeImagesDirectory(theme + "dui" + QDir::separator() + clientName));
    }
}

void DuiThemeDaemonClient::addCustomImageDirectory(const QString &path, Dui::RecursionMode recursionMode)
{
    customImageDirs.insert(0, new DuiImageDirectory(path, recursionMode));
}

void DuiThemeDaemonClient::removeAddedImageDirectories()
{
    qDeleteAll(customImageDirs);
    customImageDirs.clear();
}
