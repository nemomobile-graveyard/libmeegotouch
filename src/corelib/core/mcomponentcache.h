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

#ifndef MCOMPONENTCACHE_H
#define MCOMPONENTCACHE_H

#include "mexport.h"

#include <QString>

class MApplication;
class MApplicationWindow;
class MApplicationService;
class MComponentCachePrivate;

/*!
 * \class MComponentCache
 * \brief MComponentCache contains objects instantiated before running main().
 *
 * MComponentCache enables performance boost on application start-up
 * times by offering a storage for objects instantiated and
 * initialized before launching the application.
 *
 * Launchers like applauncherd populate this cache before loading the
 * application binary. The application should pick up the objects
 * fromt the cache instead of creating new ones. In particular, the
 * MApplication instance must be read from the cache as follows. For
 * instance, replace
 *
 * MApplication myApp(argc, argv);
 *
 * by
 *
 * MApplication *myApp = MComponentCache::mApplication(argc, argv);
 *
 * As a consequence, this prevents inheriting MApplication in
 * applications that use MComponentCache.
 *
 * If the launcher has not populated the cache, this will create a new
 * MApplication instance. Thus the same application work with and
 * without the launcher.
 *
 * The ownership of the objects returned from the cache is passed to
 * the caller.
 */
class M_CORE_EXPORT MComponentCache
{
public:
    //! Returns MApplication instance from the cache or creates a new one.
    /*!
     * Parameters are the same as in MApplication::MApplication().
     * Ownership of the returned object is passed to the caller.
     */
    static MApplication* mApplication(int &argc, char **argv, const QString &appIdentifier = QString(), MApplicationService *service = 0);

    //! Returns MApplicationWindow instance from the cache or creates a new one.
    /*!
     * Ownership of the returned object is passed to the caller.
     */
    static MApplicationWindow* mApplicationWindow();

    //! Populates cache for the use of an ordinary Meegotouch application, called by the launcher.
    static void populateForMApplication();

    //! Populates cache for the use of a web runtime application, called by the launcher.
    static void populateForWRTApplication();

    //! Returns true if cache is currently being populated, otherwise false.
    static bool populating();

protected:

    static MComponentCachePrivate* const d_ptr;

private:

    Q_DISABLE_COPY(MComponentCache)

    MComponentCache();

    friend class MApplicationWindow;
    friend class MGraphicsSystemHelper;

#ifdef UNIT_TEST
    friend class Ut_MComponentCache;
#endif
};

#endif // MCOMPONENTCACHE_H
