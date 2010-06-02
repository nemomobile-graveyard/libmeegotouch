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

#ifndef MTHEME_P_H
#define MTHEME_P_H

class IMThemeDaemon;
class MWidgetController;
class MStyleSheet;
class QPixmap;
class MTheme;
class MScalableImage;
class MLibrary;

#include <QSet>
#include <QSize>
#include <QHash>
#include <QLibrary>

#include "massembly.h"
#include "mlogicalvalues.h"
#include "mpalette.h"
#include "mdefaultfonts.h"
#include "mtheme.h"

// TODO: remove this with style profiling
#ifndef Q_OS_WIN
#include <sys/time.h>
#endif

#ifdef HAVE_GCONF
#include "mgconfitem.h"
#endif

//! \cond
struct CachedStyle {
    CachedStyle(MStyle *d) : data(d), refcount(1) {};

    MStyle *data;
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
    CachedScalableImage(const MScalableImage *p) : image(p), refcount(1) {};

    const MScalableImage *image;
    int refcount;
};
//! \endcond

class MThemePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MTheme)
public:
    MThemePrivate(const QString &applicationName, MTheme::ThemeService themeService);
    virtual ~MThemePrivate();

    typedef QHash<MStyleContainer *, MStyleContainer *> RegisteredStyleContainers;
    static RegisteredStyleContainers styleContainers;

    //remote theme cache, not used by default
    IMThemeDaemon *themeDaemon;

    // Identifier that is used to separate applications on theme daemon
    QString applicationName;

    //stores pixmap identifier and corresponding daemon handle
    QHash<QString, CachedPixmap> pixmapIdentifiers;

    //stores pixmap identifier and corresponding daemon handle
    QHash<QString, CachedScalableImage> scalableImageIdentifiers;

    MStyleSheet *customStylesheet;

    QPixmap *invalidPixmap;

    MTheme *q_ptr;

    static QHash<QString, MLibrary *>* libraries;

    MAssembly *application;

    MLogicalValues logicalValues;

    MPalette palette;
    MDefaultFonts fonts;

    QSet<QLibrary*> openedThemeLibraries;

#ifdef HAVE_GCONF
    MGConfItem locale;
    MGConfItem showAsyncRequestsItem;
#endif
    bool showAsyncRequests;

public:
    /*!
     * Check if the theme has defined a view for this controller, if not, go towards base class
     * to find one.
     */
    QString determineViewClassForController(const MWidgetController *controller);

    /*!
     * Tries to find pixmap from cache with given identifier
     * if found return the pixmap otherwise return NULL
     */
    const QPixmap *fetchPixmapFromCache(const QString &identifier);

    /*!
     * Theme daemon notifies MTheme of a theme change. In this method MTheme
     * will change the views and styles of all widgets.
     */
    void themeChangedSlot(const QStringList &themeInheritance, const QStringList& libraryNames);

    /*!
     * Finalize pixmap change or creation by updating the scene and/or emitting a signal
     */
    void pixmapRequestFinished();

    /*!
     * Theme daemon notifies pixmap updates to to this slot
     * In here MTheme will fetch the new pixmap data from X-Server and provide it to the widgets.
     */
    void pixmapChangedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle);
    void pixmapCreatedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle);

    /*!
     * GConf item change signals that locale is changed. Logical values & icons are reloaded.
     */
    void localeChangedSlot();

    bool loadCSS(const QString &filename, MTheme::InsertMode mode = MTheme::Append);

    void refreshLocalThemeConfiguration(const QStringList &themeInheritance);
    void reloadThemeLibraries(const QStringList& libraryNames);

    /*!
     * Reinitializes the theme to correspond to the new application name
     */
    void reinit(const QString &applicationName, const QString &imglistFilename = QString(), MTheme::ThemeService themeService = MTheme::AnyTheme);

    static void registerStyleContainer(MStyleContainer *container);
    static void unregisterStyleContainer(MStyleContainer *container);

    static void registerLibrary(MLibrary *library);
    static void unregisterLibrary(MLibrary *library);

#ifdef HAVE_GCONF
public slots:
    void updateShowAsyncRequests();
#endif
};

#endif

