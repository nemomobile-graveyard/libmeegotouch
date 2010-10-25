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

#ifndef MLIBRARY_H
#define MLIBRARY_H

#include <mexport.h>
#include <massembly.h>

// Defines M_LIBRARY macro, which creates a static MLibrary object.
// There should be always exactly one static MLibrary object per library
// which uses m.
#if defined M_LIBRARY_NAME
#define M_LIBRARY \
    static const MLibrary g_Library(M_LIBRARY_NAME);
#elif !defined M_APPLICATION_NAME
#error "You must have CONFIG += meegotouch in your .pro file in order to register a library!" \
"If the library you're developing doesn't provide any stylesheets then you shouldn't include MLibrary header!"
#endif


// forward declarations
class MLibraryPrivate;

/*!
   \class MLibrary
   \brief This class provides the library information to MTheme which uses it to load the correct css files.

    This class should not be used manually, but with M_LIBRARY macro. The macro will use
    M_LIBRARY_NAME as the name of the library. M_LIBRARY_NAME is automatically defined by m.prf when .pro file
    has CONFIG += m.

    You must use M_LIBRARY macro in one of your library source files to instantiate the static
    MLibrary object. This object will be instantiated while the library gets loaded, and destroyed when
    the library gets unloaded.
 */
class M_CORE_EXPORT MLibrary : public MAssembly
{
public:

    /*!
       \brief Constructs the MLibrary object and registers it to MTheme.
     */
    MLibrary(const QString &libraryName);

    /*!
       \brief Destructs the MLibrary object and unregisters it from MTheme.
     */
    virtual ~MLibrary();
};

#endif
