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

#ifndef __INOTIFY_HH__
#define __INOTIFY_HH__


  // Simplme inotify wrapper that spams with Qt signals on inotify events.
  // Watches descriptors are hidden behind INotify::Watch class that can be
  // used to store recursive directory information.


#include <QObject>


  // TODO: needs to be checked whether QObject based classes can be global
  // singletons like this ...
class INotify : public QObject
{
    Q_OBJECT

public:
    class Watch;

    ~INotify();

    static INotify *instance();

      // TODO: consider what would be most meaningful user data here ...
      // takes ownership of the data for now (in the case of data != 0)
    Watch *addWatch(const char *path, unsigned flags, QObject *data = 0);
    void removeWatch(Watch *watch);

signals:
    void fileEvent(INotify::Watch *watch, const char *name, unsigned eventmask);

private:
    INotify();

    class Private;
    Private *hidden;

    static INotify g_inst;

private slots:
    void flush();
};


#endif // __INOTIFY_HH__

