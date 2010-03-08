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

#ifndef DUILOCALTHEMEDAEMON_H
#define DUILOCALTHEMEDAEMON_H

#include "iduithemedaemon.h"
#include "duithemedaemon.h"
#include "duithemedaemonclient.h"
#include <QHash>
#include <QBuffer>

#ifdef HAVE_GCONF
#include "duigconfitem.h"
#endif

class DuiLocalThemeDaemon : public IDuiThemeDaemon
{
    Q_OBJECT
public:
    DuiLocalThemeDaemon(const QString &applicationName);
    ~DuiLocalThemeDaemon();

    virtual void addDirectoryToPixmapSearchList(const QString &directoryName, Dui::RecursionMode recursive);
    virtual void clearPixmapSearchList();

    virtual void pixmapHandleSync(const QString &imageId, const QSize &size);
    virtual void pixmapHandle(const QString &imageId, const QSize &size);
    virtual void releasePixmap(const QString &imageId, const QSize &size);

    virtual QString currentTheme();

    virtual QStringList themeInheritanceChain();
    virtual QStringList themeLibraryNames();

    virtual bool hasPendingRequests() const;

private slots:
    void themeChangedSlot();
    void localeChanged();

private:
    DuiThemeDaemonClient *client;
    DuiThemeDaemon daemon;
#ifdef HAVE_GCONF
    DuiGConfItem themeItem;
    DuiGConfItem locale;
#endif
};
#endif
