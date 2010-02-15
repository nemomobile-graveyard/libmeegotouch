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

#ifndef DUILIBRARY_H
#define DUILIBRARY_H

#include <duiexport.h>
#include <duiassembly.h>

// Defines DUI_LIBRARY macro, which creates a static DuiLibrary object.
// There should be always exactly one static DuiLibrary object per library
// which uses dui.
#if defined DUI_LIBRARY_NAME
#define DUI_LIBRARY \
    static const DuiLibrary g_Library(DUI_LIBRARY_NAME);
#elif !defined DUI_APPLICATION_NAME
#error "You must have CONFIG += dui in your .pro file in order to register a library!" \
"If the library you're developing doesn't provide any stylesheets then you shouldn't include DuiLibrary header!"
#endif


// forward declarations
class DuiLibraryPrivate;

/*!
   \class DuiLibrary
   \brief This class provides the library information to DuiTheme which uses it to load the correct css files.

    This class should not be used manually, but with DUI_LIBRARY macro. The macro will use
    DUI_LIBRARY_NAME as the name of the library. DUI_LIBRARY_NAME is automatically defined by dui.prf when .pro file
    has CONFIG += dui.

    You must use DUI_LIBRARY macro in one of your library source files to instantiate the static
    DuiLibrary object. This object will be instantiated while the library gets loaded, and destroyed when
    the library gets unloaded.
 */
class DUI_EXPORT DuiLibrary : public DuiAssembly
{
public:

    /*!
       \brief Constructs the DuiLibrary object and registers it to DuiTheme.
     */
    DuiLibrary(const QString &libraryName);

    /*!
       \brief Destructs the DuiLibrary object and unregisters it from DuiTheme.
     */
    virtual ~DuiLibrary();
};

#endif
