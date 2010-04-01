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

#ifndef DUITHEMEDAEMONCLIENT_H
#define DUITHEMEDAEMONCLIENT_H

#include <QString>
#include <QSize>
#include <QSet>
#include <QList>
#include <QDataStream>
#include <QColor>

#include "duithemedaemonprotocol.h"
#include "duinamespace.h"

class QLocalSocket;
struct ImageResource;
class DuiImageDirectory;
class DuiThemeImagesDirectory;

//! \internal
class DuiThemeDaemonClient
{
public:
    DuiThemeDaemonClient(QIODevice *socket, const QString &clientName, const QStringList &themes);
    ~DuiThemeDaemonClient();

    QString name() const;
    QDataStream &stream();

    ImageResource *pixmapResource(const Dui::DuiThemeDaemonProtocol::PixmapIdentifier &id);

    // tries to find an image from this client specific image paths
    // returns NULL if not found.
    ImageResource *findImageResource(const QString &imageId);

    // this is called when theme has changed.
    void reloadImagePaths(const QStringList &themes);

    void addCustomImageDirectory(const QString &path, Dui::RecursionMode recursionMode);
    void removeAddedImageDirectories();

    QHash<Dui::DuiThemeDaemonProtocol::PixmapIdentifier, ImageResource *> pixmaps;

private:
    QString clientName;
    QDataStream dataStream;

    QList<DuiThemeImagesDirectory *> themeImageDirs;
    QList<DuiImageDirectory *> customImageDirs;
};
//! \internal_end

#endif
