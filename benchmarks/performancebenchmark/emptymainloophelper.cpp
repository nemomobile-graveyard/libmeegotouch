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

#include "emptymainloophelper.h"

#include <MTheme>
#include <MComponentData>

#include <QApplication>
#include <QTimer>

#include <MDebug>
#include <QString>

#include <cstdlib>

#ifdef Q_OS_LINUX
#include <sys/time.h>
#endif


EmptyMainLoopHelper::EmptyMainLoopHelper() :
    terminationType( QuitOnEmpty )
{
}

void EmptyMainLoopHelper::triggerTermination(TerminationType type)
{
    terminationType = type;
    QTimer::singleShot(0, this, SLOT(terminateOnEmptyMainLoop()));
}

void EmptyMainLoopHelper::terminateOnEmptyMainLoop()
{
    if (qApp->hasPendingEvents() || MTheme::hasPendingRequests()) {
        QTimer::singleShot(0, this, SLOT(terminateOnEmptyMainLoop()));
    } else {
        QTimer::singleShot(20, this, SLOT(terminateOnEmptyMainLoop2()));
    }
}

void EmptyMainLoopHelper::terminateOnEmptyMainLoop2()
{
    if (qApp->hasPendingEvents()) {
        QTimer::singleShot(0, this, SLOT(terminateOnEmptyMainLoop()));
    } else {
        if (terminationType == ExitOnEmpty) {
            exit(0);
        } else {
#ifdef Q_OS_LINUX
            struct timeval t;
            gettimeofday(&t, 0);

            mDebug("EmptyMainLoopHelper") << QString("%1[%2%3]: quitting")
                   .arg(MComponentData::instance()->appName())
                   .arg((int)t.tv_sec)
                   .arg((int)(t.tv_usec % 1000000), 6, 10, (const QChar)'0');
#endif

            qApp->quit();
        }
    }
}

