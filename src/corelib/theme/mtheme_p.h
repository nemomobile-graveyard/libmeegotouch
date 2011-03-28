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
#include <QAtomicInt>
#include <QSystemSemaphore>

#include "massembly.h"
#include "mlogicalvalues.h"
#include "mpalette.h"
#include "mdefaultfonts.h"
#include "mtheme.h"
#include "mpixmaphandle.h"

// TODO: remove this with style profiling
#ifndef Q_OS_WIN
#include <sys/time.h>
#endif

#ifdef HAVE_GCONF
#include "mgconfitem.h"
#endif

//! \cond
struct CachedStyle {
    CachedStyle(MStyle *d) : data(d), refcount(1) {}

    MStyle *data;
    int refcount;
};
//! \endcond

//! \cond
struct CachedPixmap {
    CachedPixmap(const QPixmap *p, const QString &imageId, const QSize &size) :
            pixmap(p), refcount(1), imageId(imageId), size(size), addr(0), numBytes(0)
    {}

    const QPixmap *pixmap;
    QAtomicInt refcount;
    QString imageId;
    QSize size;
    void *addr;
    int numBytes;
};

struct CachedScalableImage {
    CachedScalableImage(const MScalableImage *p) : image(p), refcount(1) {}

    const MScalableImage *image;
    QAtomicInt refcount;
};
//! \endcond

class MThemePrivate
{
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
    QVector<const QPixmap *> releasedPixmaps;

    //stores pixmap identifier and corresponding daemon handle
    QHash<QString, CachedScalableImage> scalableImageIdentifiers;

    MStyleSheet *customStylesheet;

    QPixmap *invalidPixmapPtr;

    MTheme *q_ptr;

    static QHash<QString, MLibrary *>* libraries;

    MAssembly *application;

    MLogicalValues logicalValues;

    MPalette palette;
    MDefaultFonts fonts;

    QSet<QLibrary*> openedThemeLibraries;

#ifdef HAVE_GCONF
    MGConfItem locale;
#endif

    typedef QHash<QString, QString> MStringHash;
    QHash<const char*, MStringHash> controllerViewCache;

    QTimer *changeThemeTimer;
    QSystemSemaphore changeThemeSemaphore;

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
     * Creates or returns already created invalid pixmap.
     */
    QPixmap *invalidPixmap();

    /*!
     * Theme daemon notifies pixmap updates to to this slot
     * In here MTheme will fetch the new pixmap data from X-Server and provide it to the widgets.
     */
    void pixmapCreatedOrChangedSlot(const QString &imageId, const QSize &size, const MPixmapHandle& pixmapHandle);

    const QPixmap * pixmap(const QString &id, bool async, const QSize &size = QSize(0, 0));

    /*!
     * GConf item change signals that locale is changed. Logical values & icons are reloaded.
     */
    void localeChangedSlot();

    /*!
     * Invokes themeChangedSlot() with the current theme inheritence chain and library names.
     */
    void changeTheme();

    bool loadCSS(const QString &filename, MTheme::InsertMode mode = MTheme::Append);

    void refreshLocalThemeConfiguration(const QStringList &themeInheritance);
    void reloadThemeLibraries(const QStringList& libraryNames);

    QHash<QString, CachedPixmap>::iterator findCachedPixmap(const QPixmap *pixmap);
    bool releasePixmapNow(const QPixmap *pixmap);
    bool releasePixmapNow(QHash<QString, CachedPixmap>::iterator i);
    bool releasePixmap(const QPixmap *pixmap);

    void cleanupGarbage();

    /*!
     * Reinitializes the theme to correspond to the new application name
     */
    void reinit(const QString &applicationName);

    static bool extractDataForStyleClass(const QByteArray &styleClassName,
                                         QList<const MStyleSheet *> &sheets,
                                         QList<QByteArray> &styleMetaObjectHierarchy);
    static QList<const MStyleSheet *> extractSheetsForClassHierarchy(const QList<const MStyleSheet *> &sheets,
                                                                     const QList<QByteArray> &parentHierarchy);

    static void appendAllLibraryStyleSheets(QList<const MStyleSheet *> &sheets);
    static bool appendLibraryStyleSheet(QList<const MStyleSheet *> &sheets, const QString &assemblyName);

    static void registerStyleContainer(MStyleContainer *container);
    static void unregisterStyleContainer(MStyleContainer *container);

    static void registerLibrary(MLibrary *library);
    static void unregisterLibrary(MLibrary *library);

private:
    typedef QPair<QList<const MStyleSheet *>, QList<QByteArray> > SheetsAndHierarchy;
    static QHash<QByteArray, SheetsAndHierarchy> hierarchyCache;
};

#endif

