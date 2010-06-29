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

#ifndef MTHEMEDAEMONCLIENT_H
#define MTHEMEDAEMONCLIENT_H

#include <QString>
#include <QSize>
#include <QSet>
#include <QList>
#include <QDataStream>
#include <QColor>

#include "mthemedaemonprotocol.h"
#include "mnamespace.h"

class QLocalSocket;
struct ImageResource;
class MImageDirectory;
class MThemeImagesDirectory;

//! \internal
class MThemeDaemonClient
{
public:
    MThemeDaemonClient(QIODevice *socket, const QString &clientName, const QStringList &themes);
    ~MThemeDaemonClient();

    void reinit(const QString &newClientName, const QStringList &themes);

    QString name() const;
    QDataStream &stream();

    ImageResource *pixmapResource(const M::MThemeDaemonProtocol::PixmapIdentifier &id);

    // tries to find an image from this client specific image paths
    // returns NULL if not found.
    ImageResource *findImageResource(const QString &imageId);

    // this is called when theme has changed.
    void reloadImagePaths(const QStringList &themes);

    void addCustomImageDirectory(const QString &path, M::RecursionMode recursionMode);
    void removeAddedImageDirectories();

    QHash<M::MThemeDaemonProtocol::PixmapIdentifier, ImageResource *> pixmaps;

private:
    QString clientName;
    QDataStream dataStream;

    QList<MThemeImagesDirectory *> themeImageDirs;
    QList<MImageDirectory *> customImageDirs;
};
//! \internal_end

#endif
