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

#include "utils.h"

#include <QCoreApplication>
#include <QDir>

#include <QDesktopServices>

namespace Utils
{

    QString contactsDir()
    {
#ifdef Q_OS_WIN
        QDir appDir(QCoreApplication::applicationDirPath());
        appDir.cdUp();
        appDir.cd("share");
        appDir.cd("themes");
        appDir.cd("base");
        appDir.cd("meegotouch");
        appDir.cd("widgetsgallery");
        appDir.cd("images");
        appDir.cd("contacts");
        return appDir.path().append("/");
#else
        return QString(CONTACTS_DIR);
#endif
    }

    QString imagesDir()
    {
#ifdef Q_OS_WIN
        QDir appDir(QCoreApplication::applicationDirPath());
        appDir.cdUp();
        appDir.cd("share");
        appDir.cd("themes");
        appDir.cd("base");
        appDir.cd("meegotouch");
        appDir.cd("widgetsgallery");
        appDir.cd("images");
        return appDir.path().append("/");
#else
        return QString(IMAGES_DIR);
#endif
    }

    QString mediaArtDir()
    {
#ifdef Q_OS_WIN
        QDir appDir(QCoreApplication::applicationDirPath());
        appDir.cdUp();
        appDir.cd("share");
        appDir.cd("themes");
        appDir.cd("base");
        appDir.cd("meegotouch");
        appDir.cd("widgetsgallery");
        appDir.cd("images");
        appDir.cd("media-art");
        return appDir.path().append("/");
#else
        return QString(MEDIA_DIR);
#endif
    }

    QString picturesDir()
    {
        return QDesktopServices::storageLocation(QDesktopServices::PicturesLocation);
    }

}
