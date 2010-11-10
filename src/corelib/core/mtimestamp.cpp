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

#include "mtimestamp.h"

#include "mdebug.h"
#ifdef unix
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#else
#include <QtCore/QTime>
#endif

void M_CORE_EXPORT mTimestampStream(const QString &module, const QString &file, const QString &scope, const QString &msg)
{
#ifdef unix
    static const QString format("%1.%2 MARK|%3|%4|%5");
    static const QChar fill('0');
    QString output;
    struct timeval tv;

    gettimeofday(&tv, 0);

    output = format.arg(tv.tv_sec).arg(tv.tv_usec, 6, 10, fill).arg(file).arg(scope).arg(msg);
    mWarning(module) << output;
#else
    static const QString format("MARK|%3|%4|%5");
    QString output = format.arg(file).arg(scope).arg(msg);
    mWarning(module) << QTime::currentTime() << output;
#endif
}
