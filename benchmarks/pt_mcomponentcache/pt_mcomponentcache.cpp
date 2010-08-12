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

#include "pt_mcomponentcache.h"

#include <mbenchmark.h>
#include <MApplication>
#include <MComponentCache>

void Pt_MComponentCache::reinit()
{
    int argc = 1;
    char appName[] = "./pt_mcomponentcache";
    char *argv[] = { appName };

    // QBENCHMARK_ONCE invokes reinit() two times
    // (see <http://bugreports.qt.nokia.com/browse/QTBUG-12689>),
    // so firstRun is used to workaround this problem.
    static bool firstRun = true;
    if (firstRun) {
        firstRun = false;
    } else {
        // Trigger creating an internal MApplication instance
        MComponentCache::populateForMApplication();
    }

    MApplication *app = 0;
    MBENCHMARK_ONCE(
        // Trigger a MComponentData::reinit()
        app = MComponentCache::mApplication(argc, argv);
    )
    delete app;
}

QTEST_APPLESS_MAIN(Pt_MComponentCache)
