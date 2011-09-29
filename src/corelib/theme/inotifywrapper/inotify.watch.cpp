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


#include "inotify.watch.h"


INotify::Watch::Watch(const char *path) : FSTreeNode(path),
                                          wd(-1),
                                          d(0)
{
}

INotify::Watch::~Watch()
{
    if (d) {
        delete d;
        d = 0;
    }
}


void INotify::Watch::recursiveDestruction()
{
    QLinkedList<FSTreeNode *> oldchildren = children;

      // suicide first, children later
    INotify::instance()->removeWatch(this);

    for (QLinkedList<FSTreeNode *>::iterator i = oldchildren.begin();
        i != oldchildren.end(); i++) {
        Watch *w = (Watch *)*i;  // FIXME: blind cast ...
        w->recursiveDestruction();
    }
}


const QObject *INotify::Watch::data()
{
    return d;
}

