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

#ifndef DUITHEME_P_H
#define DUITHEME_P_H

class IDuiThemeDaemon;
class DuiWidgetController;
class DuiStyleSheet;
class QPixmap;
class DuiTheme;
class DuiScalableImage;
class DuiLibrary;

#include <QSet>
#include <QSize>
#include <QHash>
#include <QLibrary>

#include "duiassembly.h"
#include "duilogicalvalues.h"
#include "duipalette.h"
#include "duidefaultfonts.h"

// TODO: remove this with style profiling
#ifndef Q_OS_WIN
#include <sys/time.h>
#endif

#ifdef HAVE_GCONF
#include "duigconfitem.h"
#endif

//! \cond
struct CachedStyle {
    CachedStyle(DuiStyle *d) : data(d), refcount(1) {};

    DuiStyle *data;
    int refcount;
};
//! \endcond

//! \cond
struct CachedPixmap {
    CachedPixmap(const QPixmap *p, const QString &imageId, const QSize &size) : pixmap(p), refcount(1), imageId(imageId), size(size) {};

    const QPixmap *pixmap;
    int refcount;
    QString imageId;
    QSize size;
};

struct CachedScalableImage {
    CachedScalableImage(const DuiScalableImage *p) : image(p), refcount(1) {};

    const DuiScalableImage *image;
    int refcount;
};
//! \endcond

class DuiThemePrivate
{
public:
    DuiThemePrivate(const QString &applicationName, DuiTheme::ThemeService themeService);
    virtual ~DuiThemePrivate();

    typedef QHash<DuiStyleContainer *, DuiStyleContainer *> RegisteredStyleContainers;
    static RegisteredStyleContainers styleContainers;

    //remote theme cache, not used by default
    IDuiThemeDaemon *themeDaemon;

    // Identifier that is used to separate applications on theme daemon
    QString applicationName;

    //stores pixmap identifier and corresponding daemon handle
    QHash<QString, CachedPixmap> pixmapIdentifiers;

    //stores pixmap identifier and corresponding daemon handle
    QHash<QString, CachedScalableImage> scalableImageIdentifiers;

    DuiStyleSheet *customStylesheet;

    QPixmap *invalidPixmap;

    DuiTheme *q_ptr;

    static QHash<QString, DuiLibrary *>* libraries;

    DuiAssembly application;

    DuiLogicalValues logicalValues;

    DuiPalette palette;
    DuiDefaultFonts fonts;

    QSet<QLibrary*> openedThemeLibraries;

#ifdef HAVE_GCONF
    DuiGConfItem locale;
#endif

public:
    /*!
     * Check if the theme has defined a view for this controller, if not, go towards base class
     * to find one.
     */
    QString determineViewClassForController(const DuiWidgetController *controller);

    /*!
     * Tries to find pixmap from cache with given identifier
     * if found return the pixmap otherwise return NULL
     */
    const QPixmap *fetchPixmapFromCache(const QString &identifier);

    /*!
     * Theme daemon notifies DuiTheme of a theme change. In this method DuiTheme
     * will change the views and styles of all widgets.
     */
    void themeChangedSlot(const QStringList &themeInheritance, const QStringList& libraryNames);

    /*!
     * Theme daemon notifies pixmap updates to to this slot
     * In here DuiTheme will fetch the new pixmap data from X-Server and provide it to the widgets.
     */
    void pixmapChangedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle);
    void pixmapCreatedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle);

    /*!
     * GConf item change signals that locale is changed. Logical values & icons are reloaded.
     */
    void localeChangedSlot();

    bool loadCSS(const QString &filename, DuiTheme::InsertMode mode = DuiTheme::Append);

    void refreshLocalThemeConfiguration(const QStringList &themeInheritance);
    void reloadThemeLibraries(const QStringList& libraryNames);

    static void registerStyleContainer(DuiStyleContainer *container);
    static void unregisterStyleContainer(DuiStyleContainer *container);

    static void registerLibrary(DuiLibrary *library);
    static void unregisterLibrary(DuiLibrary *library);
};

#endif

