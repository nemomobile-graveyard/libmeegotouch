/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __INOTIFY_WATCH_HH__
#define __INOTIFY_WATCH_HH__


#include "inotify.h"

#include "fstreenode.h"


class INotify::Watch : public FSTreeNode
{
friend INotify::Watch *INotify::addWatch(const char *path, unsigned flags, QObject *data);
friend void INotify::removeWatch(INotify::Watch *watch);
friend void INotify::flush();
public:  // suicides and kills children
    void recursiveDestruction();

    const QObject *data();

private:
    Watch(const char *path);
    virtual ~Watch();

    int wd;

    QObject *d;
};


#endif // __INOTIFY_WATCH_HH__

