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

#ifndef MEXPORT_H
#define MEXPORT_H

#include <QtCore/QtGlobal>

// the M_EXPORT define is deprecated. it does not
// make any sense to use it. we just keep it here
// so that projects that are using it continue to
// work as before.
#if defined(M_EXPORTS)
#  define M_EXPORT Q_DECL_EXPORT
#else
#  if defined (Q_OS_WIN)
#    define M_EXPORT Q_DECL_IMPORT
#  else
#    define M_EXPORT Q_DECL_EXPORT
#  endif
#endif

#ifdef M_BUILD_CORE
#  define M_CORE_EXPORT Q_DECL_EXPORT
#else
#  define M_CORE_EXPORT Q_DECL_IMPORT
#endif

#ifdef M_BUILD_VIEWS
#  define M_VIEWS_EXPORT Q_DECL_EXPORT
#else
#  define M_VIEWS_EXPORT Q_DECL_IMPORT
#endif

#ifdef M_BUILD_SETTINGS
#  define M_SETTINGS_EXPORT Q_DECL_EXPORT
#else
#  define M_SETTINGS_EXPORT Q_DECL_IMPORT
#endif

#ifdef M_BUILD_EXTENSIONS
#  define M_EXTENSIONS_EXPORT Q_DECL_EXPORT
#else
#  define M_EXTENSIONS_EXPORT Q_DECL_IMPORT
#endif

#if defined(Q_OS_WIN) && defined (Q_CC_MSVC)
# ifndef __func__
#  define __func__ __FUNCTION__
# endif
# ifndef __PRETTY_FUNCTION__
#  define __PRETTY_FUNCTION__ __FUNCTION__
# endif
#endif

#endif // Header guard
