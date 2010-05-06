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

#ifndef MFILEDATASTORE_P_H
#define MFILEDATASTORE_P_H

#include <QSettings>
#include <QScopedPointer>
#include <QFileSystemWatcher>
#include <QMap>

class MFileDataStorePrivate
{
public:
    MFileDataStorePrivate(const QString &filePath);

    //! The used data storing backend
    QSettings settings;

    //! Snapshot of the settings, used for observing external file changes
    QMap<QString, QVariant> settingsSnapshot;

    //! File system watcher wrapped with QScopedPointer to monitor changes in the settings file
    QScopedPointer<QFileSystemWatcher> watcher;
};

#endif // MFILEDATASTORE_P_H
