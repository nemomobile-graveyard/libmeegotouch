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


#include <sys/inotify.h>
#include <cstdio>

#include <QHash>

#include <QSocketNotifier>


#include "inotify.h"
#include "inotify.watch.h"

#include "utils.h"


  // size of the buffer that is used to read inotify events
#define INEVBUFSIZE 2048


class INotify::Private
{
public:
    Private();

    QSocketNotifier *notifier;

    int fd;

      // optimizing to find Watch for corresponding
      // - not too fast when removing watches
    QHash<int, Watch *> watchlist;

      // buffer for reading the file descriptor
    unsigned char buffer[INEVBUFSIZE];
    unsigned bufPos;
};


INotify::Private::Private() : notifier(0), fd(-1)
{
}


INotify INotify::g_inst;


INotify::INotify() : QObject(0), hidden(0)
{
    hidden = new Private;
}

INotify::~INotify()
{
    if (hidden) {
        if (hidden->fd != -1) {
            close(hidden->fd);
            hidden->fd = -1;
        }

        delete hidden;
        hidden = 0;
    }
}


INotify *INotify::instance()
{
    return &g_inst;
}


INotify::Watch *INotify::addWatch(const char *path, unsigned flags, QObject *data)
{
    if (!hidden)
        return 0;

        // not done under construction in order to avoid calling inotify_init
        // unless an application is actually using the interface
    if (hidden->fd == -1) {
        hidden->fd = inotify_init();

        if (hidden->fd == -1)
            return 0;
    }

    if (!path || !path[0])
        return 0;

        // can't be done on construction thanks to lack of event dispatcher at that time
    if (!hidden->notifier) {
        hidden->notifier = new QSocketNotifier(hidden->fd,
                                               QSocketNotifier::Read,
                                               this);
        QObject::connect(hidden->notifier, SIGNAL(activated(int)),
                         this, SLOT(flush()));
    }

    int wd = inotify_add_watch(hidden->fd, path, flags);
    if (wd == -1)
        return 0;

    Watch *ret = new Watch(path);
    ret->wd = wd;

    ret->d = data;

    hidden->watchlist.insert(wd, ret);

    return ret;
}

void INotify::removeWatch(INotify::Watch *watch)
{
    if (!hidden || hidden->fd == -1 || !watch || watch->wd == -1)
        return;

      // the expensive looking could be avoided by storing wd inside Watch
    QHash<int, Watch *>::iterator iter = hidden->watchlist.find(watch->wd);

    if (iter != hidden->watchlist.end()) {
        inotify_rm_watch(hidden->fd, iter.key());

        delete *iter;
        hidden->watchlist.erase(iter);
    }
}


void INotify::flush()
{
    if (!hidden || hidden->fd == -1)
        return;  // should never happen here ...


    int readCount = read(hidden->fd, hidden->buffer + hidden->bufPos, INEVBUFSIZE - hidden->bufPos);
    if (0 < readCount) {
        hidden->bufPos += readCount;
    }


    struct inotify_event *event = (struct inotify_event *)hidden->buffer;
    unsigned evlen;

    while (sizeof(struct inotify_event) <= hidden->bufPos &&
           (evlen = sizeof(struct inotify_event) + event->len) <= hidden->bufPos) {
        QHash<int, Watch *>::iterator iter = hidden->watchlist.find(event->wd);
        if (iter != hidden->watchlist.end())
            emit fileEvent(*iter, event->name, event->mask);

        util::rewindbuf(hidden->buffer, evlen, hidden->bufPos - evlen);
        hidden->bufPos -= evlen;
    }
}

