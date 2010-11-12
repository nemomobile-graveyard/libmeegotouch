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

#include "ut_mcomponentcache.h"
#include "../../src/corelib/core/mcomponentcache_p.h"
#include "../../src/corelib/core/mcpumonitor.h"
#include <MComponentData>
#include <MApplication>
#include <MApplicationService>
#include <MApplicationService>

class MThemeDaemon
{
public:
    MThemeDaemon();
    virtual ~MThemeDaemon();

    static QString systemThemeDirectory();
};

QString MThemeDaemon::systemThemeDirectory()
{
    QString dir("/usr/share/theme/");
    return dir;
}


void Ut_MComponentCache::init()
{
}


void Ut_MComponentCache::cleanup()
{
}


void Ut_MComponentCache::testDBusService()
{
    /* We must test many things in the same function because
       populateForMApplication can be run only once. */
    
    QVERIFY2( MComponentCache::populating() == false, "Failure");	
    QVERIFY2( MComponentCache::d_ptr->cacheBeingPopulated == false, "Failure");
    MComponentCache::d_ptr->cacheBeingPopulated = true;
    QVERIFY2( MComponentCache::populating() == true, "Failure");	
    MComponentCache::d_ptr->cacheBeingPopulated = false;
    QVERIFY2( MComponentCache::populating() == false, "Failure");	

    MComponentCache::populateForMApplication();

    /* Pick up the application from the cache and register a special D-Bus service */
    static char ** argv  = packTwoArgs("testDBusRegistration", "argument1");
    static int argc = 2;
    MApplicationService *service = new MApplicationService("com.nokia.appName");

    MApplication *app = MComponentCache::mApplication(argc, argv, "doesnt_matter", service);

    QVERIFY2( app != NULL, "Failure");
    QVERIFY2( MComponentCache::populating() == false, "Failure");

    QString appService = MComponentData::instance()->serviceName();
    QString defaultService("com.nokia.appName");
    QVERIFY2( appService == defaultService, "Failure");

    MApplicationWindow* win = MComponentCache::mApplicationWindow();

    QVERIFY2( win != NULL, "Failure");
    QVERIFY2( MComponentCache::populating() == false, "Failure");

    cleanupCache();
}

char ** Ut_MComponentCache::packTwoArgs(const char * arg0, const char * arg1)
{
    char ** argv  = new char * [2];
    char * result = new char[strlen(arg0) + strlen(arg1) + 2];
    memset(result, '\0', strlen(arg0) + strlen(arg1) + 2);

    strcat(result, arg0);
    strcat(result, " ");
    strcat(result, arg1);

    // Arguments are allocated consecutively in Linux
    argv[0] = result;
    argv[1] = argv[0] + strlen(arg0) + 1;
    argv[0][strlen(arg0)] = '\0';

    return argv;
}

void  Ut_MComponentCache::cleanupCache()
{
    delete (MComponentCache::d_ptr->mApplicationInstance);
    MComponentCache::d_ptr->mApplicationInstance = 0;
    MComponentCache::d_ptr->mApplicationWindowInstance = 0;
}

QTEST_APPLESS_MAIN(Ut_MComponentCache);
