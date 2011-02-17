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

#ifndef MSYSTEMDIRECTORIES_H
#define MSYSTEMDIRECTORIES_H

class QString;

class MSystemDirectories
{
public:
    /**
      * Returns a cache directory which can be used to store library
      * specific cache files.
      */
    static QString cacheDirectory();

    /**
      * Sets the prefix for cache directory. This is useful for example
      * when building a dpkg-package and it should be avoided to generate
      * the caches in system root directory.
      */
    static void setPrefix(const QString &prefix);
};

#endif
