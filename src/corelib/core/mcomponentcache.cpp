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

#include "mthemedaemon.h"

#include <QGLFormat>
#include <QGLWidget>

MComponentCachePrivate * const MComponentCache::d_ptr = new MComponentCachePrivate;

MComponentCachePrivate::MComponentCachePrivate() :
    mApplicationInstance(0),
    mApplicationWindowInstance(0),
    glWidgetOfmApplicationWindowInstance(0),
    glWidgetOfOtherWindow(0),
    cacheBeingPopulated(false)
{
}

MComponentCachePrivate::~MComponentCachePrivate()
{
    if (glWidgetOfmApplicationWindowInstance != 0)
        delete glWidgetOfmApplicationWindowInstance;
    if (glWidgetOfOtherWindow != 0)
        delete glWidgetOfOtherWindow;
    if (mApplicationWindowInstance != 0)
        delete mApplicationWindowInstance;
}

bool MComponentCachePrivate::populating()
{
    return cacheBeingPopulated;
}

void MComponentCachePrivate::populateForMApplication()
{
    static int argc = 1;
    static char *argv0 = (char *)"generic";

    cacheBeingPopulated = true;

    if (mApplicationInstance == 0) {
        mApplicationInstance = new MApplication(argc, &argv0, 0);
    } else {
        cacheBeingPopulated = false;
        qFatal("MComponentCache::populateForMApplication() - Cache is already populated.");
    }

    glWidgetOfmApplicationWindowInstance = createNewGlWidget();
    glWidgetOfOtherWindow = createNewGlWidget();

    mApplicationWindowInstance = new MApplicationWindow();

    cacheBeingPopulated = false;
}

QGLWidget* MComponentCachePrivate::createNewGlWidget()
{
    QGLFormat fmt;
    // disable multisampling, is enabled by default in Qt                                                                                          
    fmt.setSampleBuffers(false);
    fmt.setSamples(0);
    return new QGLWidget(fmt);
}

MApplication* MComponentCachePrivate::mApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service)
{
    if (mApplicationInstance == 0) {
        mApplicationInstance = new MApplication(argc, argv, appIdentifier, service);
    } else {
        if (canUseCachedApp(argc, argv, appIdentifier)) {
            MComponentData::instance()->reinit(argc, argv, appIdentifier, service);
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
    if ( hasCustomTheme(argc, argv, appIdentifier) || 
         hasExtraParams(argc, argv, appIdentifier) )
        return false;

    return true;
}

bool MComponentCachePrivate::hasCustomTheme(int &argc, char **argv, const QString &appIdentifier)
{
    Q_UNUSED(argc);

    QFileInfo fileInfo(argv[0]);
    QString themeIdentifier = fileInfo.fileName();
    if (!appIdentifier.isEmpty()) {
        QRegExp regExp("[0-9a-zA-Z_-]*");
        if (regExp.exactMatch(appIdentifier)) {
            themeIdentifier = appIdentifier;
        }
    }

    QString sysThemePath(MThemeDaemon::systemThemeDirectory());

    QDir themesDir(sysThemePath);
    QStringList themes = themesDir.entryList(QDir::AllDirs | 
                                             QDir::NoDotAndDotDot);

    QString meegotouch("meegotouch");
    foreach(const QString & theme, themes) {
        QString fullPath(sysThemePath + QDir::separator() + theme);
        fullPath.append(QDir::separator() + meegotouch + QDir::separator() + themeIdentifier);
 
        if ( QDir(fullPath).exists() )
            return true;
    }
    return false;
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
            (s.startsWith("--help"))              ||

            /* theme related options */
            (s == "-genimglist")                  ||
            (s == "-remote-theme")                ||
            
            /* other options */
            (s == "-fullscreen")                  ||
            (s == "-output-level")                ||
            (s == "-output-file")                 ||
            (s == "-disable-m-input-context")     ||
            (s == "-output-prefix")               || 
            (s == "-no-output-prefix")            
           )
            return true;
    }
    return false;
}

MApplicationWindow* MComponentCachePrivate::mApplicationWindow()
{
    if (mApplicationWindowInstance == 0) {
        mApplicationWindowInstance = new MApplicationWindow();
    }

    return mApplicationWindowInstance;
}

QGLWidget* MComponentCachePrivate::glWidget()
{
    QGLWidget *returnValue;
    if (cacheBeingPopulated && glWidgetOfmApplicationWindowInstance != 0) {
        returnValue = glWidgetOfmApplicationWindowInstance;
        glWidgetOfmApplicationWindowInstance = 0;
    } else if (glWidgetOfOtherWindow != 0) {
        returnValue = glWidgetOfOtherWindow;
        glWidgetOfOtherWindow = 0;
    } else {
        returnValue = createNewGlWidget();
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

QGLWidget* MComponentCache::glWidget()
{
    return d_ptr->glWidget();
}
