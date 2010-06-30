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

#ifndef TESTTHEMEDAEMON_H
#define TESTTHEMEDAEMON_H

#include <QMultiMap>
#include "../../src/corelib/theme/imthemedaemon.h"

class QPixmap;

class TestThemeDaemon : public IMThemeDaemon
{
    Q_OBJECT
public:

    TestThemeDaemon();

    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, M::RecursionMode recursive);
    virtual void clearPixmapSearchList();
    virtual void pixmapHandleSync(const QString &imageId, const QSize &size);
    virtual void pixmapHandle(const QString &imageId, const QSize &size);
    virtual void releasePixmap(const QString &imageId, const QSize &size);
    virtual void registerApplicationName(const QString &applicationName);
    virtual QString currentTheme();
    virtual QStringList findAvailableThemes();
    virtual void changeTheme(const QString &theme_id);
    void emitThemeChange();
    virtual QStringList themeInheritanceChain();
    virtual QStringList themeLibraryNames();
    virtual bool hasPendingRequests() const;

    int pixmapCount() const;
    void reset();

signals:
    void themeReinited();

private:
    QMultiMap<QString, QPixmap *> pixmaps;
    QString current;
};

#endif
