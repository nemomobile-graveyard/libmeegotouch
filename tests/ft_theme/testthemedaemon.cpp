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

#include <QStringList>
#include <QPixmap>
#include <QDir>
#include "testthemedaemon.h"
#include <QDebug>

TestThemeDaemon::TestThemeDaemon()
{
    current = "";
}

void TestThemeDaemon::addDirectoryToPixmapSearchList(const QString &directoryName, M::RecursionMode recursive)
{
    Q_UNUSED(directoryName);
    Q_UNUSED(recursive);
}

void TestThemeDaemon::registerApplicationName(const QString &applicationName)
{
    Q_UNUSED(applicationName);
    emit themeReinited();
}

void TestThemeDaemon::clearPixmapSearchList()
{
}

void TestThemeDaemon::pixmapHandleSync(const QString &imageId, const QSize &size)
{
    QString identifier = imageId + ',' + QString::number(size.width()) + ',' + QString::number(size.height());
    // store
    int pixmapWidth = size.width() < 0 ? 128 : size.width();
    int pixmapHeight = size.height() < 0 ? 128 : size.height();

    QPixmap *pixmap = new QPixmap(pixmapWidth, pixmapHeight);
    pixmaps.insert(identifier, pixmap);

    // emit signal
    MPixmapHandle handle;
    handle.xHandle = pixmap->handle();
    emit pixmapCreatedOrChanged(imageId, size, handle);
}

void TestThemeDaemon::pixmapHandle(const QString &imageId, const QSize &size)
{
    // use sync loading always
    pixmapHandleSync(imageId, size);
}

void TestThemeDaemon::releasePixmap(const QString &imageId, const QSize &size)
{
    QString identifier = imageId + ',' + QString::number(size.width()) + ',' + QString::number(size.height());

    // find from list
    QPixmap *pixmap = pixmaps.value(identifier, NULL);
    if (pixmap) {
        // remove & delete
        pixmaps.remove(identifier);
        delete pixmap;
    }
}

QString TestThemeDaemon::currentTheme()
{
    return current;
}

QStringList TestThemeDaemon::findAvailableThemes()
{
    QStringList list;
    list << "theme 1";
    list << "theme 2";
    return list;
}

void TestThemeDaemon::changeTheme(const QString &theme_id)
{
    if (theme_id == "theme 1") {
        current = theme_id;
    } else if (theme_id == "theme 2") {
        current = theme_id;
    }
}

void TestThemeDaemon::emitThemeChange()
{
    emit themeChanged(themeInheritanceChain(), QStringList());
}

QStringList TestThemeDaemon::themeInheritanceChain()
{
    QString themeDirectory = QDir::tempPath() + QDir::separator() + QString("Ft_Theme") + QDir::separator();

    QStringList themeInheritance;
    if (current == "theme 2") {
        themeInheritance << themeDirectory + "theme2" + QDir::separator();
    }
    themeInheritance << themeDirectory + "theme1" + QDir::separator();
    return themeInheritance;
}

QStringList TestThemeDaemon::themeLibraryNames()
{
    return QStringList();
}

bool TestThemeDaemon::hasPendingRequests() const
{
    return false;
}

int TestThemeDaemon::pixmapCount() const
{
    return pixmaps.count();
}

void TestThemeDaemon::reset()
{
    pixmaps.clear();
}


