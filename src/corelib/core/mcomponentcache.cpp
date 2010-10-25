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

#ifdef QT_OPENGL_LIB
#include "mgles2renderer.h"
#endif

#include "mcomponentcache.h"
#include "mcomponentcache_p.h"
#include "mapplication.h"
#include "mapplicationservice.h"
#include "mapplicationwindow.h"
#include "mcomponentdata.h"
#include "mgraphicssystemhelper.h"

#include "mthemedaemon.h"

#ifdef QT_OPENGL_LIB
#include <QGLFormat>
#include <QGLWidget>
#endif

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

MComponentCachePrivate * const MComponentCache::d_ptr = new MComponentCachePrivate;
const int MComponentCachePrivate::ARGV_LIMIT = 32;

MComponentCachePrivate::MComponentCachePrivate() :
    mApplicationInstance(0),
    mApplicationWindowInstance(0),
    glWidgetOfmApplicationWindowInstance(0),
    cacheBeingPopulated(false),
    initialArgc(ARGV_LIMIT),
    initialArgv(new char* [initialArgc])
{
}

MComponentCachePrivate::~MComponentCachePrivate()
{
#ifdef QT_OPENGL_LIB
    delete glWidgetOfmApplicationWindowInstance;
#endif
    delete mApplicationWindowInstance;
    delete[] initialArgv;

    MComponentCache::cleanupCache();
}

bool MComponentCachePrivate::populating()
{
    return cacheBeingPopulated;
}

void MComponentCachePrivate::populateForMApplication()
{
    static const char *const appName = "componentcache_pre_initialized_mapplication";
    static const char *const emptyString = "";

    cacheBeingPopulated = true;

    // We support at most ARGV_LIMIT arguments in QCoreApplication. These will be set when real
    // arguments are known (in MComponentCachePrivate::mApplication). 
    initialArgv[0] = const_cast<char *>(appName);
    for (int i = 1; i < initialArgc; i++) {
        initialArgv[i] = const_cast<char *>(emptyString);
    }

    if (mApplicationInstance == 0) {
        mApplicationInstance = new MApplication(initialArgc, initialArgv, 0);
    } else {
        cacheBeingPopulated = false;
        qFatal("MComponentCache::populateForMApplication() - Cache is already populated.");
    }

    if (MGraphicsSystemHelper::isRunningNativeGraphicssystem()) {
        glWidgetOfmApplicationWindowInstance = createNewGlWidget();
    }

    mApplicationWindowInstance = new MApplicationWindow();

    cacheBeingPopulated = false;
}

void MComponentCachePrivate::populateForWRTApplication()
{
    cacheBeingPopulated = true; // reset and unset in populateForMApplication
    MApplication::setGraphicsSystem("raster");
    populateForMApplication();
}

QGLWidget* MComponentCachePrivate::createNewGlWidget(const QGLFormat* format)
{
#ifdef QT_OPENGL_LIB
    QGLFormat fmt;

    if (!format) {
        // disable multisampling, is enabled by default in Qt
        fmt.setSampleBuffers(false);
        fmt.setSamples(0);
    }
    else {
        fmt = *format;
    }

    QGLWidget* shareWidget = NULL;
    foreach(FormatWidgetPair pair, shareWidgetsCache) {
        if (pair.first == fmt)  {
            shareWidget = pair.second;
            break;
        }
    }

    if (!shareWidget) {
       shareWidget = new QGLWidget(fmt);
       shareWidgetsCache.append(qMakePair(fmt, shareWidget));
    }

    return new QGLWidget(fmt, NULL, shareWidget);
#else
    Q_UNUSED(format);
    return 0;
#endif
}

MApplication* MComponentCachePrivate::mApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{
    if (mApplicationInstance == 0) {
        mApplicationInstance = new MApplication(argc, argv, appIdentifier, service);
    } else {
        if (canUseCachedApp(argc, argv, appIdentifier)) {
            if(argc > ARGV_LIMIT) {
                qWarning("MComponentCache: QCoreApplication::arguments() will not contain all arguments.");
            }

            // Copy arguments to QCoreApplication 
            for (int i = 0; i < qMin(argc, ARGV_LIMIT); i++) {
                qApp->argv()[i] = argv[i];
            }

            // This changes argc in QCoreApplication
            initialArgc = qMin(argc, ARGV_LIMIT);

            MComponentData::instance()->reinit(argc, argv, appIdentifier, service);

#ifdef Q_WS_X11
            // reinit WM_COMMAND X11 property
            if (mApplicationWindowInstance) {
                Display *display = QX11Info::display();
                if (display) {
                    XSetCommand(display, mApplicationWindowInstance->effectiveWinId(), argv, argc);
                }
            }
#endif
            /* Unregister MApplicationWindow for the case that boosted
               application instantiates and uses an MApplicationWindow
               of its own. In that case application's window must be
               MApplication::activeWindow() as well. On the other
               hand, if application takes mApplicationWindowInstance
               from the cache, it will be re-registered then.
             */
            MComponentData::unregisterWindow(mApplicationWindowInstance);

        } else {
            /* Clean up cache.
               
               Deleting mApplicationInstance without deleting
               mApplicationWindowInstance first causes a
               crash. Therefore we cannot save the existing
               mApplicationWindowInstance to be used in new
               mApplication. 
            */
            if (mApplicationWindowInstance) {
                delete mApplicationWindowInstance;
                mApplicationWindowInstance = 0;
            }

            delete mApplicationInstance;
            mApplicationInstance = new MApplication(argc, argv, appIdentifier, service);
        }
    }
    return mApplicationInstance;
}

bool MComponentCachePrivate::canUseCachedApp(int &argc, char **argv, const QString &appIdentifier)
{
    if ( hasExtraParams(argc, argv, appIdentifier) )
        return false;

    return true;
}

bool MComponentCachePrivate::hasExtraParams(int &argc, char **argv, const QString &appIdentifier)
{
    Q_UNUSED(appIdentifier);

    for (int i = 1; i < argc; ++i) 
    {
        QString s(argv[i]);

        if (
            /* QApplication command options */
            (s == "-style")                       ||
            (s == "-stylesheet")                  ||
            (s == "-session")                     ||
            (s == "-widgetcount")                 ||
            (s == "-reverse")                     ||
            (s == "-graphicssystem")              ||

            /* X11 options */
            (s == "-display")                     ||
            (s == "-geometry" )                   ||
            (s == "-fn" )                         ||
            (s == "-font")                        ||
            (s == "-bg")                          ||
            (s == "-background")                  ||
            (s == "-fg")                          ||
            (s == "-foreground")                  ||
            (s == "-btn")                         ||
            (s == "-button")                      ||
            (s == "-name" )                       ||
            (s == "-title" )                      ||
            (s == "-visual" )                     ||
            (s == "-ncols")                       ||
            (s == "-cmap")                        ||
            (s == "-im")                          ||
            (s == "-inputstyle" )                 ||

            /* help options, application will not be started */
            (s == "-v")                           ||
            (s.startsWith("-version"))            ||
            (s.startsWith("--version"))           ||
            (s == "-h")                           ||
            (s.startsWith("-help"))               ||
            (s.startsWith("--help"))
           )
            return true;
    }
    return false;
}

MApplicationWindow* MComponentCachePrivate::mApplicationWindow()
{
    MApplicationWindow *returnValue;
    if (mApplicationWindowInstance != 0) {
        returnValue = mApplicationWindowInstance;
        mApplicationWindowInstance = 0;
        MComponentData::registerWindow(returnValue);
    } else {
        returnValue = new MApplicationWindow();
    }
    return returnValue;
}

QGLWidget* MComponentCachePrivate::glWidget(const QGLFormat* format)
{
#ifdef QT_OPENGL_LIB
    QGLWidget *returnValue;
    if (glWidgetOfmApplicationWindowInstance != 0
        && (!format || glWidgetOfmApplicationWindowInstance->format() == *format))
    {
        returnValue = glWidgetOfmApplicationWindowInstance;
        glWidgetOfmApplicationWindowInstance = 0;
    } else {
        returnValue = createNewGlWidget(format);
    }
    return returnValue;
#else
    Q_UNUSED(format);
    return 0;
#endif
}

MComponentCache::MComponentCache()
{
}

void MComponentCache::populateForMApplication()
{
    d_ptr->populateForMApplication();
}

void MComponentCache::populateForWRTApplication()
{
    d_ptr->populateForWRTApplication();
}

bool MComponentCache::populating()
{
    return d_ptr->populating();
}

void MComponentCache::cleanupCache()
{
#ifdef QT_OPENGL_LIB
    while (!d_ptr->shareWidgetsCache.isEmpty()) {
        MComponentCachePrivate::FormatWidgetPair pair = d_ptr->shareWidgetsCache.takeFirst();
        delete pair.second;
    }
#endif
}

MApplication* MComponentCache::mApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{    
    return d_ptr->mApplication(argc, argv, appIdentifier, service);
}

MApplicationWindow* MComponentCache::mApplicationWindow()
{
    return d_ptr->mApplicationWindow();
}

QGLWidget* MComponentCache::glWidget()
{
    return d_ptr->glWidget();
}

QGLWidget* MComponentCache::glWidget(const QGLFormat& format)
{
    return d_ptr->glWidget(&format);
}
