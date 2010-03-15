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

#include "duitheme.h"
#include "duitheme_p.h"

#include "duilibrary.h"
DUI_LIBRARY

#ifdef Q_WS_X11
#include <QX11Info>
#endif
#include <QPixmap>
#include <DuiDebug>
#include <QCoreApplication>
#include <QFileInfo>
#include <QSettings>
#include <QDir>

#include "private/duiwidgetcontroller_p.h"

#ifdef HAVE_GCONF
#include "duigconfitem.h"
#endif

#include "duiclassfactory.h"
#include "duistyle.h"
#include "duiremotethemedaemon.h"
#include "duilocalthemedaemon.h"
#include "duistylesheet.h"
#include "duiwidgetcontroller.h"
#include "duiwidgetview.h"
#include "duiscenemanager.h"
#include "duiscene.h"
#include "duiscalableimage.h"

#include "private/duiwidgetcontroller_p.h"

#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duicomponentdata.h"
#include "duicomponentdata_p.h"

// Must be last as it conflicts with some Qt defined types
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

DuiTheme *gTheme = 0;

QHash<QString, DuiLibrary *>* DuiThemePrivate::libraries = NULL;
DuiThemePrivate::RegisteredStyleContainers DuiThemePrivate::styleContainers;

namespace
{

// "default_pixmap_MyPixmap_47_47"
static QString defaultPixmapCacheId(const QString &name, int width, int height)
{
    return QString::fromLatin1("default_pixmap_") + name
        + QChar::fromLatin1('_') + QString::number(width)
        + QChar::fromLatin1('_') + QString::number(height);
}

// "scalable_image_myscalable_5_5_5_5
static QString scalableImageCacheId(const QString &name, int left, int top, int right, int bottom)
{
    return QString::fromLatin1("scalable_image_") + name
        + QChar::fromLatin1('_') + QString::number(left)
        + QChar::fromLatin1('_') + QString::number(top)
        + QChar::fromLatin1('_') + QString::number(right)
        + QChar::fromLatin1('_') + QString::number(bottom);
}

} // anonymous namespace

DuiTheme::DuiTheme(const QString &applicationName, const QString &, ThemeService themeService) :
    d_ptr(new DuiThemePrivate(applicationName, themeService))
{
    if (gTheme || (DuiComponentData::instance() && DuiComponentData::instance()->d_ptr->theme))
        qFatal("There cannot be multiple instances of DuiTheme, use DuiTheme::instance() instead of constructing a new one");

    Q_D(DuiTheme);

    d->q_ptr = this;

    connect(d->themeDaemon, SIGNAL(themeChanged(QStringList, QStringList)),
            SLOT(themeChangedSlot(QStringList, QStringList)));

    connect(d->themeDaemon, SIGNAL(pixmapChanged(QString, QSize, Qt::HANDLE)),
            SLOT(pixmapChangedSlot(QString, QSize, Qt::HANDLE)));

    connect(d->themeDaemon, SIGNAL(pixmapCreated(QString, QSize, Qt::HANDLE)),
            SLOT(pixmapCreatedSlot(QString, QSize, Qt::HANDLE)));

#ifdef HAVE_GCONF
    connect(&d->locale, SIGNAL(valueChanged()), SLOT(localeChangedSlot()));
#endif

    gTheme = this;
}

DuiTheme::~DuiTheme()
{
    DuiStyleSheet::cleanup(true);

    QHash<QString, CachedScalableImage>::iterator i2 = d_ptr->scalableImageIdentifiers.begin();
    QHash<QString, CachedScalableImage>::iterator end2 = d_ptr->scalableImageIdentifiers.end();
    for (; i2 != end2; ++i2) {
        qWarning() << "DuiTheme - DuiScalableImage" << i2.key() << "not released!" << "refcount:" << i2.value().refcount;
        releasePixmap(i2.value().image->pixmap());
        delete i2.value().image;
    }


    // print identifiers from all pixmaps which were not released
    QHash<QString, CachedPixmap>::iterator i = d_ptr->pixmapIdentifiers.begin();
    QHash<QString, CachedPixmap>::iterator end = d_ptr->pixmapIdentifiers.end();
    for (; i != end; ++i) {
        qWarning() << "DuiTheme - pixmap" << i.key() << "not released!" << "refcount:" << i.value().refcount;
    }

    // unload all theme libraries
    d_ptr->reloadThemeLibraries(QStringList());

    gTheme = NULL;
    delete d_ptr;
}


bool DuiTheme::addPixmapDirectory(const QString &directoryName, Dui::RecursionMode recursive)
{
    QDir dir(directoryName);
    if (!dir.exists())
        return false;

    instance()->d_ptr->themeDaemon->addDirectoryToPixmapSearchList(dir.absolutePath(), recursive);
    return true;
}

void DuiTheme::clearPixmapDirectories()
{
    instance()->d_ptr->themeDaemon->clearPixmapSearchList();
}

DuiTheme *DuiTheme::instance()
{
    if (DuiComponentData::instance())
        return DuiComponentData::instance()->d_ptr->theme;

    if (!gTheme) {
        // This allows DuiTheme to be independent from DuiApplication.
        // Uses this process' name as the theme identifier by default
        QFileInfo fileInfo(QCoreApplication::instance()->applicationName());
        QString applicationName = fileInfo.fileName();
        gTheme = new DuiTheme(applicationName);
    }

    return gTheme;
}

const QPixmap *DuiTheme::pixmap(const QString &id, const QSize &size)
{
    if(id.isEmpty()) {
        duiWarning("DuiTheme") << "requested pixmap without id";
        return instance()->d_ptr->invalidPixmap;
    }

    // TODO: check if needed
    QSize realSize = size;
    if (realSize.width() < 1)
        realSize.rwidth() = 0;
    if (realSize.height() < 1)
        realSize.rheight() = 0;

    QString identifier = defaultPixmapCacheId(id, realSize.width(), realSize.height());
    const QPixmap *p = instance()->d_ptr->fetchPixmapFromCache(identifier);

    // check if we found the pixmap from cache?
    if (p)
        return p;

    QPixmap *result;
    // we have to create temporary pixmap
    if (realSize.isValid()) {
        result = new QPixmap(realSize);
        //TODO: activate the debug guards once the release is close
        // fill with an ugly color -- this pixmap should never be visible
//#ifndef NDEBUG
        result->fill(QColor(0, 255, 0, 255));
//#endif
    } else {
        result = new QPixmap();
    }

    instance()->d_ptr->pixmapIdentifiers.insert(identifier, CachedPixmap(result, id, realSize));
    instance()->d_ptr->themeDaemon->pixmapHandle(id, realSize);

    return result;
}

QPixmap *DuiTheme::pixmapCopy(const QString &id, const QSize &size)
{
    // TODO: check if needed
    QSize realSize = size;
    if (realSize.width() < 1)
        realSize.rwidth() = 0;
    if (realSize.height() < 1)
        realSize.rheight() = 0;

    QString identifier = defaultPixmapCacheId(id, realSize.width(), realSize.height());
    const QPixmap *p = instance()->d_ptr->fetchPixmapFromCache(identifier);

    // check if we found the pixmap from cache?
    if (p) {
        return new QPixmap(p->copy());
    }

    QPixmap *result;
    // we have to create temporary pixmap
    if (realSize.width() < 1 || realSize.height() < 1) {
        result = new QPixmap(1, 1);
    } else {
        result = new QPixmap(realSize);
    }
    result->fill(QColor(128, 128, 128, 32));

    instance()->d_ptr->pixmapIdentifiers.insert(identifier, CachedPixmap(result, id, realSize));
    instance()->d_ptr->themeDaemon->pixmapHandleSync(id, realSize);

    QPixmap *copy = new QPixmap(result->copy());
    releasePixmap(result);
    return copy;
}

const DuiScalableImage *DuiTheme::scalableImage(const QString &id, int left, int right, int top, int bottom)
{
    // check if we already have this scalable image in the cache
    QString scalableidentifier = scalableImageCacheId(id, left, top, right, bottom);
    QHash<QString, CachedScalableImage>::iterator i = instance()->d_ptr->scalableImageIdentifiers.find(scalableidentifier);
    if (i != instance()->d_ptr->scalableImageIdentifiers.end()) {
        //image found, increase refcount and return it
        ++i.value().refcount;
        return i.value().image;
    }

    //first try to fetch the used pixmap from the cache
    QString pixmapidentifier = defaultPixmapCacheId(id, 0, 0);
    const QPixmap *p = instance()->d_ptr->fetchPixmapFromCache(pixmapidentifier);
    if (!p) {
        // we have to create temporary pixmap
        QPixmap *result = new QPixmap(1, 1);
        result->fill(QColor(128, 128, 128, 32));

        instance()->d_ptr->pixmapIdentifiers.insert(pixmapidentifier, CachedPixmap(result, id, QSize(0, 0)));
        instance()->d_ptr->themeDaemon->pixmapHandle(id, QSize(0, 0));

        p = result;
    }

    //create the actual scalable image and cache it
    DuiScalableImage *image = new DuiScalableImage(p, left, right, top, bottom, id);
    instance()->d_ptr->scalableImageIdentifiers.insert(scalableidentifier, CachedScalableImage(image));

    return image;
}

void DuiTheme::releaseScalableImage(const DuiScalableImage *image)
{
    // find the image from cache and decrease refcount + release if refcount = 0
    // TODO: this could be optimized
    QHash<QString, CachedScalableImage>::iterator i = instance()->d_ptr->scalableImageIdentifiers.begin();
    QHash<QString, CachedScalableImage>::iterator end = instance()->d_ptr->scalableImageIdentifiers.end();
    for (; i != end; ++i) {
        // is this the image which we should release?
        if (i.value().image == image) {
            --i.value().refcount;
            if (i.value().refcount == 0) {
                releasePixmap(i.value().image->pixmap());
                delete i.value().image;
                instance()->d_ptr->scalableImageIdentifiers.erase(i);
            }
            break;
        }
    }
}


void DuiTheme::releasePixmap(const QPixmap *pixmap)
{
    // NULL pixmap, do nothing
    if (!pixmap)
        return;

    // invalidPixmap, no need to release it
    if (pixmap == instance()->d_ptr->invalidPixmap) {
        return;
    }

    // find the pixmap from cache and decrease refcount + release if refcount = 0
    // TODO: this could be optimized
    QHash<QString, CachedPixmap>::iterator i = instance()->d_ptr->pixmapIdentifiers.begin();
    QHash<QString, CachedPixmap>::iterator end = instance()->d_ptr->pixmapIdentifiers.end();
    for (; i != end; ++i) {       // is this the pixmap which we should release?
        if (i.value().pixmap == pixmap) {
            --i.value().refcount;
            if (i.value().refcount == 0) {
                instance()->d_ptr->themeDaemon->releasePixmap(i.value().imageId, i.value().size);
                delete i.value().pixmap;
                instance()->d_ptr->pixmapIdentifiers.erase(i);
            }
            return;
        }
    }

    // check if we didn't find the pixmap from our cache
    Q_ASSERT_X(false, "DuiTheme::releasePixmap", "Pixmap not found from the cache!");
}

const DuiStyle *DuiTheme::style(const char *styleClassName,
                                const QString &objectName)
{
    return DuiTheme::style(styleClassName, objectName, 0, 0, Dui::Landscape, NULL);
}

const DuiStyle *DuiTheme::style(const char *styleClassName,
                                const QString &objectName,
                                const QString &mode,
                                const QString &type,
                                Dui::Orientation orientation,
                                const DuiWidgetController *parent)
{
    // The style type should never be "default" - that would probably be a view type
    // that's mistakenly being used as a style type.
    // The caller probably means "" instead.
    Q_ASSERT(type != "default");

    DuiThemePrivate *d = DuiTheme::instance()->d_func();

    // list containing all stylesheets from all assemblies from which this style is/inherits + app css
    QList<const DuiStyleSheet *> sheets;

    // go trough the inheritance chain and add stylesheets from each assembly.
    const QMetaObject *mobj = DuiClassFactory::instance()->styleMetaObject(styleClassName);
    do {
        QString assemblyName = DuiClassFactory::instance()->styleAssemblyName(mobj->className());
        Dui::AssemblyType assemblyType = DuiClassFactory::instance()->styleAssemblyType(mobj->className());
        if (assemblyType == Dui::Application) {
            mobj = mobj->superClass();
            continue;
        }

        // find proper library
        DuiLibrary *library = d->libraries->value(assemblyName, NULL);
        if (!library) {
            duiWarning("DuiTheme") << "Cannot find library. You must register your library to theming using DUI_LIBRARY macro." << '(' << assemblyName << ')';
            Q_ASSERT_X(library, "DuiTheme", "Failed to find library");
        }
        // use stylesheet from this library if there is one
        if (library->stylesheet()) {
            if (!sheets.contains(library->stylesheet())) {
                sheets.insert(0, library->stylesheet());
            }
        }
        mobj = mobj->superClass();
    } while (mobj->className() != QObject::staticMetaObject.className());

    // add application css
    if (d->application.stylesheet())
        sheets.append(d->application.stylesheet());

    // add custom stylesheet
    if (d->customStylesheet)
        sheets.append(d->customStylesheet);

    return DuiStyleSheet::style(sheets, styleClassName, objectName, mode, type, orientation, parent);
}

void DuiTheme::releaseStyle(const DuiStyle *style)
{
    if (!style)
        return;

    DuiStyleSheet::releaseStyle(style);
}

DuiWidgetView *DuiTheme::view(const DuiWidgetController *controller)
{
    // Best matching view class name
    QString viewClassName = instance()->d_ptr->determineViewClassForController(controller);

    if (viewClassName.isEmpty()) {
        qWarning() << "Could not find view class for:" << controller->metaObject()->className() << "/" << controller->viewType();
        return NULL;
    }

    DuiWidgetView *v = DuiClassFactory::instance()->createView(viewClassName.toStdString().c_str(), controller);
    if (!v) {
        qWarning() << "Failed to create view for:" << controller->metaObject()->className() << "/" << controller->viewType() << ".  Class name found was: " << viewClassName;
        return NULL;
    }

    return v;
}

const DuiPalette &DuiTheme::palette()
{
    return instance()->d_ptr->palette;
}

const DuiDefaultFonts &DuiTheme::fonts()
{
    return instance()->d_ptr->fonts;
}

bool DuiTheme::loadCSS(const QString &filename, InsertMode mode)
{
    if (instance()->d_ptr->loadCSS(filename, mode)) {
        // Re-populate all the styles, custom stylesheet may have overridden something
        for (DuiThemePrivate::RegisteredStyleContainers::iterator iterator = DuiThemePrivate::styleContainers.begin(); iterator != DuiThemePrivate::styleContainers.end(); ++iterator) {
            iterator.value()->reloadStyles();
        }
        // notify all widgets that style needs to be applied
        QSet<DuiWidgetController *>::iterator end = DuiWidgetControllerPrivate::allSystemWidgets.end();
        for (QSet<DuiWidgetController *>::iterator i = DuiWidgetControllerPrivate::allSystemWidgets.begin();
                i != end; ++i) {
            // get view ask it to apply the new style
            const DuiWidgetView *view = (*i)->view();
            if (view) {
                const_cast<DuiWidgetView *>(view)->applyStyle();
            }
        }
        return true;
    }

    return false;
}

bool DuiThemePrivate::loadCSS(const QString &filename, DuiTheme::InsertMode mode)
{
    DuiStyleSheet *newStylesheet = new DuiStyleSheet(&logicalValues);
    bool result = newStylesheet->load(filename);
    if (result) {
        // loading ok, check what to do.. overwrite or append?
        if ((mode == DuiTheme::Append) && customStylesheet) {
            // append the loaded style data into the existing stylesheet
            *customStylesheet += *newStylesheet;
            delete newStylesheet;
            newStylesheet = NULL;
        } else {
            // no existing stylesheet or the stylesheet needs to be overwritten
            delete customStylesheet;
            customStylesheet = newStylesheet;
        }

        // Cached entries are not valid any more
        DuiStyleSheet::cleanup(false);
        return true;
    }

    delete newStylesheet;
    return false;
}

QString DuiTheme::currentTheme()
{
    DuiThemePrivate *d = DuiTheme::instance()->d_func();
    return d->themeDaemon->currentTheme();
}

QStringList DuiTheme::findAvailableThemes()
{
    return QStringList();
}

void DuiTheme::changeTheme(const QString &theme_id)
{
    Q_UNUSED(theme_id);
}

bool DuiTheme::hasPendingRequests()
{
    return instance()->d_ptr->themeDaemon->hasPendingRequests();
}

DuiThemePrivate::DuiThemePrivate(const QString &applicationName, DuiTheme::ThemeService themeService) :
    applicationName(applicationName),
    customStylesheet(NULL),
    application(applicationName),
    palette(logicalValues),
    fonts(logicalValues)
#ifdef HAVE_GCONF
    , locale("/Dui/i18n/Language")
#endif
{
    switch (themeService) {
    case DuiTheme::LocalTheme:
        themeDaemon = new DuiLocalThemeDaemon(applicationName);
        break;

    case DuiTheme::RemoteTheme:
        themeDaemon = new DuiRemoteThemeDaemon(applicationName, -1);
        break;

    case DuiTheme::AnyTheme: {
        DuiRemoteThemeDaemon *tds = new DuiRemoteThemeDaemon(applicationName, 0);
        if (tds->connected()) {
            themeDaemon = tds;
        } else {
            delete tds;
            themeDaemon = new DuiLocalThemeDaemon(applicationName);
        }
    } break;
    }

    invalidPixmap = new QPixmap(50, 50);
    invalidPixmap->fill(QColor(255, 64, 64, 255));

    // this loads the current theme
    reloadThemeLibraries(themeDaemon->themeLibraryNames());
    refreshLocalThemeConfiguration(themeDaemon->themeInheritanceChain());
}

DuiThemePrivate::~DuiThemePrivate()
{
    delete themeDaemon;
    delete invalidPixmap;
    delete customStylesheet;

    // TODO: check if we need to release QPixmaps from pixmapHandles
}

QString DuiThemePrivate::determineViewClassForController(const DuiWidgetController *controller)
{
    bool exactMatch = false;

    // first search from application view configuration
    QString bestMatch = application.viewType(controller, exactMatch);
    if (exactMatch)
        return bestMatch;

    for (const QMetaObject *metaObject = controller->metaObject(); metaObject != &DuiWidget::staticMetaObject; metaObject = metaObject->superClass()) {

        // check if this widget is declared inside some library
        Dui::AssemblyType type = DuiClassFactory::instance()->widgetAssemblyType(metaObject->className());
        if (type == Dui::Application)
            continue;

        // get name of the library where this widget was declared
        QString assemblyName = DuiClassFactory::instance()->widgetAssemblyName(metaObject->className());
        if (assemblyName.isEmpty())
            continue;

        // find proper library
        DuiLibrary *library = libraries->value(assemblyName, NULL);
        Q_ASSERT_X(library, "DuiTheme", "Failed to find library");

        // try to get view type for the widget
        QString viewClassName = library->viewType(controller, exactMatch);
        if (exactMatch) {
            return viewClassName;
        }

        if (bestMatch.isEmpty()) {
            bestMatch = viewClassName;
        }
    }

    return bestMatch;
}

const QPixmap *DuiThemePrivate::fetchPixmapFromCache(const QString &identifier)
{
    QHash<QString, CachedPixmap>::iterator i = pixmapIdentifiers.find(identifier);

    // check if we already have this pixmap in cache
    if (i != pixmapIdentifiers.end()) {
        ++i.value().refcount;
        return i.value().pixmap;
    }

    // if not return null
    return NULL;
}

void DuiThemePrivate::themeChangedSlot(const QStringList &themeInheritance, const QStringList& libraryNames)
{
    refreshLocalThemeConfiguration(themeInheritance);
    q_ptr->rebuildViewsForWidgets();
    reloadThemeLibraries(libraryNames);
}

void DuiThemePrivate::refreshLocalThemeConfiguration(const QStringList &themeInheritance)
{
    QString language;

#ifdef HAVE_GCONF
    // determine current language
    language = locale.value().toString();
#endif

    // Load logical values from ini file
    logicalValues.load(themeInheritance, language);

    // refresh default fonts & palette
    palette.refresh();
    fonts.refresh();

    // load all css-files from all libraries from all themes.
    foreach(DuiLibrary * lib, *libraries) {
        lib->themeChanged(themeInheritance, logicalValues);
    }

    // refresh application theme data
    application.themeChanged(themeInheritance, logicalValues);

    // cached data is no more valid
    DuiStyleSheet::cleanup(false);
}

void DuiThemePrivate::reloadThemeLibraries(const QStringList& libraryNames)
{
    // store list of libraries that needs to be unloaded
    QSet<QLibrary*> toUnload = openedThemeLibraries;

    // load all new libraries (if the library is already loaded, it will ref the loaded one)
    openedThemeLibraries.clear();
    foreach(const QString& libname, libraryNames) {
        QLibrary* library = new QLibrary(libname);
        if(library->load()) {
            openedThemeLibraries.insert(library);
        } else {
            duiWarning("DuiTheme") << "Failed to open theme library:" << libname;
            delete library;
        }
    }

    // unload old themelibraries
    foreach(QLibrary* library, toUnload) {
        library->unload();
        delete library;
    }
}

void DuiThemePrivate::registerLibrary(DuiLibrary *library)
{
    if (!DuiThemePrivate::libraries)
        DuiThemePrivate::libraries = new QHash<QString, DuiLibrary *>();

    DuiThemePrivate::libraries->insert(library->name(), library);

    // Load theme-specific content of this library (in case the lib was loaded after startup).
    // TODO: load/unload the stylesheets of this library, this only handles view configuration atm.
    if (gTheme) {
        DuiThemePrivate *d = DuiTheme::instance()->d_ptr;
        library->themeChanged(d->themeDaemon->themeInheritanceChain(), d->logicalValues);
    }
}

void DuiThemePrivate::unregisterLibrary(DuiLibrary *library)
{
    DuiThemePrivate::libraries->remove(library->name());

    if (DuiThemePrivate::libraries->count() == 0) {
        delete DuiThemePrivate::libraries;
        DuiThemePrivate::libraries = NULL;
    }
}

void DuiThemePrivate::pixmapCreatedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle)
{
    QString identifier = defaultPixmapCacheId(imageId, size.width(), size.height());
    QHash<QString, CachedPixmap>::iterator iterator = pixmapIdentifiers.find(identifier);
    Q_ASSERT_X(iterator != pixmapIdentifiers.end(), "DuiThemePrivate::pixmapCreatedSlot", "Unknown pixmap");

    QPixmap *pixmap = (QPixmap *) iterator.value().pixmap;

    if (pixmapHandle == 0) {
        duiWarning("DuiThemePrivate") << "pixmapCreatedSlot - pixmap creation failed (null handle):" << identifier;
        *pixmap = *invalidPixmap;
        if (!DuiTheme::hasPendingRequests() && DuiApplication::activeWindow() && DuiApplication::activeWindow()->viewport()) {
            DuiApplication::activeWindow()->viewport()->update();
        }
        return;
    }

#ifdef Q_WS_X11
    // we can create the real pixmap right now
    *pixmap = QPixmap::fromX11Pixmap(pixmapHandle, QPixmap::ImplicitlyShared);
#else
    QPixmap *pixmapPointer = (QPixmap *)(pixmapHandle);
    *pixmap = *pixmapPointer;
#endif

    if (!DuiTheme::hasPendingRequests() && DuiApplication::activeWindow() && DuiApplication::activeWindow()->viewport()) {
        DuiApplication::activeWindow()->viewport()->update();
    }
}

void DuiThemePrivate::pixmapChangedSlot(const QString &imageId, const QSize &size, Qt::HANDLE pixmapHandle)
{
    QString identifier = defaultPixmapCacheId(imageId, size.width(), size.height());
    QHash<QString, CachedPixmap>::iterator iterator = pixmapIdentifiers.find(identifier);

    if (iterator == pixmapIdentifiers.end()) {
        // the updated pixmap has already been released, but the daemon didn't get the message early enough
        return;
    }

    QPixmap *pixmap = (QPixmap *) iterator.value().pixmap;

    if (pixmapHandle == 0) {
        // the daemon couldn't reload the pixmap, so it sent a 0 as handle
        qDebug() << "daemon failed to reload pixmap" << imageId << size;
        if (!DuiTheme::hasPendingRequests() && DuiApplication::activeWindow() && DuiApplication::activeWindow()->viewport()) {
            DuiApplication::activeWindow()->viewport()->update();
        }
        *pixmap = *invalidPixmap;
        return;
    }

#ifdef Q_WS_X11
    // we can create the real pixmap right now
    *pixmap = QPixmap::fromX11Pixmap(pixmapHandle, QPixmap::ImplicitlyShared);
#else
    QPixmap *pixmapPointer = (QPixmap *)(pixmapHandle);
    *pixmap = *pixmapPointer;
#endif

    if (!DuiTheme::hasPendingRequests() && DuiApplication::activeWindow() && DuiApplication::activeWindow()->viewport()) {
        DuiApplication::activeWindow()->viewport()->update();
    }
}


void DuiTheme::rebuildViewsForWidgets()
{
    Q_D(DuiTheme);

    // Re-populate all the styles
    // TODO: This could be probably optimized somehow
    for (DuiThemePrivate::RegisteredStyleContainers::iterator iterator = d->styleContainers.begin(); iterator != d->styleContainers.end(); ++iterator) {
        iterator.value()->reloadStyles();
    }

    // go trough all widgets, replace views
    QSet<DuiWidgetController *>::iterator end = DuiWidgetControllerPrivate::allSystemWidgets.end();
    for (QSet<DuiWidgetController *>::iterator i = DuiWidgetControllerPrivate::allSystemWidgets.begin();
            i != end; ++i) {

        DuiWidgetController *controller = (*i);

        // figure out the view class name
        QString className = d->determineViewClassForController(controller);

        const DuiWidgetView *view = controller->view();
        if (view) {
            // check if it's completely new view class for this widget?
            if (className != view->metaObject()->className()) {
                controller->d_func()->deprecateView();
            } else {
                const_cast<DuiWidgetView *>(view)->applyStyle();
            }
        }
    }
}

void DuiThemePrivate::registerStyleContainer(DuiStyleContainer *container)
{
    styleContainers[container] = container;
}

void DuiThemePrivate::unregisterStyleContainer(DuiStyleContainer *container)
{
    RegisteredStyleContainers::iterator iterator = styleContainers.find(container);
    if (iterator != styleContainers.end()) {
        styleContainers.erase(iterator);
    }
}

void DuiThemePrivate::localeChangedSlot()
{
    themeChangedSlot(themeDaemon->themeInheritanceChain(), themeDaemon->themeLibraryNames());
}

#include "moc_duitheme.cpp"
