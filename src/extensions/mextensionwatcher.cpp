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

#include "mextensionwatcher.h"
#include "mdesktopentry.h"
#include "mapplicationextensionmetadata.h"
#include <QFile>
#include <QStringList>


MExtensionWatcher::MExtensionWatcher()
{
    connect(&watcher, SIGNAL(fileChanged(const QString &)), this, SLOT(notifyDataChanged(const QString &)));
}

MExtensionWatcher::~MExtensionWatcher()
{
}

void MExtensionWatcher::addExtension(
    QSharedPointer<const MDesktopEntry> extension)
{
    QString fileName = extension->fileName();
    if (!extensionsToWatch.contains(fileName)) {
        extensionsToWatch.insert(fileName, extension);
        watcher.addPath(fileName);
    }
}

void MExtensionWatcher::removeExtension(const QString &fileName)
{
    extensionsToWatch.remove(fileName);
}

void MExtensionWatcher::notifyDataChanged(const QString &path) const
{
    // If the file does not exist anymore no need to signal an update for it
    // observation of the file removal is handled by the instance managers
    if (!QFile::exists(path)) {
        return;
    }
    QHash<QString, QSharedPointer<const MDesktopEntry> >::const_iterator
        extension =  extensionsToWatch.find(path);
    if (extension != extensionsToWatch.constEnd()) {
        emit extensionChanged(*extension.value());
    }
    if (!watcher.files().contains(path)) {
        // if the file is removed and recreated, we need to re-add
        // the path to the watcher
        watcher.addPath(path);
    }
}
