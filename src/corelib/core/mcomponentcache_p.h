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

#ifndef MCOMPONENTCACHE_P_H
#define MCOMPONENTCACHE_P_H

#include <QList>
#include <QPair>

class MComponentCache;
class MApplication;
class MApplicationWindow;

class MComponentCachePrivate
{
public:
    MComponentCachePrivate();
    virtual ~MComponentCachePrivate();
    void populateForMApplication();
    void populateForWRTApplication();
    bool populating();
    MApplication* mApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);
    MApplicationWindow* mApplicationWindow();

    bool canUseCachedApp(int &argc, char **argv, const QString &appIdentifier);
    bool hasExtraParams(int &argc, char **argv, const QString &appIdentifier);

    static const int ARGV_LIMIT;
    MApplication *mApplicationInstance;
    MApplicationWindow *mApplicationWindowInstance;
    bool cacheBeingPopulated;
    int initialArgc;
    char **initialArgv;

#ifdef UNIT_TEST
    friend class Ut_MComponentCache;
#endif
};

#endif // MCOMPONENTCACHE_P_H
