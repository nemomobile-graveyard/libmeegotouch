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

#include "mcomponentcache.h"
#include "mcomponentcache_p.h"
#include "mapplication.h"
#ifdef HAVE_DBUS
#include "mapplicationservice.h"
#endif //HAVE_DBUS
#include "mapplicationwindow.h"
#include "mcomponentdata.h"
#include "mgraphicssystemhelper.h"
#include "mthemedaemon.h"
#include "QFileInfo"
#ifndef UNIT_TEST
#include "mtheme_p.h"
#include "mlibrary.h"
#endif

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#endif

#include <unistd.h>


MComponentCachePrivate * const MComponentCache::d_ptr = new MComponentCachePrivate;
const int MComponentCachePrivate::ARGV_LIMIT = 32;

MComponentCachePrivate::MComponentCachePrivate() :
    mApplicationInstance(0),
    mApplicationWindowInstance(0),
    cacheBeingPopulated(false),
    initialArgc(ARGV_LIMIT),
    initialArgv(new char* [initialArgc])
{
}

MComponentCachePrivate::~MComponentCachePrivate()
{
    delete mApplicationWindowInstance;
    delete[] initialArgv;
}

bool MComponentCachePrivate::populating()
{
    return cacheBeingPopulated;
}

#ifdef Q_WS_X11
static int boostedapp_xio_errhandler(Display *)
{
    // The safest way to exit a boosted application is without calling
    // exit handlers of loaded libraries. A significant part of the
    // libraries have been loaded and are still used by the parent
    // process.
    _exit(EXIT_FAILURE);
}
#endif //Q_WS_X11


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
#ifndef UNIT_TEST
        // force libraries to load their stylesheets
        foreach (const MLibrary *lib, *MTheme::instance()->d_ptr->libraries) {
            lib->stylesheet();
        }
#endif
    } else {
        cacheBeingPopulated = false;
        qFatal("MComponentCache::populateForMApplication() - Cache is already populated.");
    }

    mApplicationWindowInstance = new MApplicationWindow();

 #ifdef Q_WS_X11
    XSetIOErrorHandler(boostedapp_xio_errhandler);
 #endif //Q_WS_X11

    cacheBeingPopulated = false;
}

void MComponentCachePrivate::populateForWRTApplication()
{
    MApplication::setGraphicsSystem("raster");
    populateForMApplication();
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
            
            // Set object name for MApplication
            QString appName = QFileInfo(argv[0]).fileName();    
            qApp->setObjectName(appName);

#ifdef Q_WS_X11
            // reinit WM_COMMAND X11 property
            if (mApplicationWindowInstance) {
                Display *display = QX11Info::display();
                if (display) {
                    XSetCommand(display, mApplicationWindowInstance->effectiveWinId(), argv, argc);

                    // set correct WM_CLASS properties
                    QString appClass = appName.left(1).toUpper();
                    if (appName.length() > 1)
                        appClass += appName.right(appName.length() - 1);
           
                    // reserve memory for C strings
                    QByteArray arrName(appName.toLatin1());
                    QByteArray arrClass(appClass.toLatin1());

                    XClassHint class_hint;
                    class_hint.res_name  = arrName.data();
                    class_hint.res_class = arrClass.data();

                    XSetClassHint(display, mApplicationWindowInstance->effectiveWinId(), &class_hint);
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
        mApplicationWindowInstance->reinit();
        returnValue = mApplicationWindowInstance;
        mApplicationWindowInstance = 0;
        MComponentData::registerWindow(returnValue);
        returnValue->prestartedInit();
    } else {
        returnValue = new MApplicationWindow();
    }
    return returnValue;
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

MApplication* MComponentCache::mApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{    
    return d_ptr->mApplication(argc, argv, appIdentifier, service);
}

MApplicationWindow* MComponentCache::mApplicationWindow()
{
    return d_ptr->mApplicationWindow();
}

