/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "timedemobenchmark.h"
#include "timedemo.h"

#include <QTimer>
#include <MApplicationPage>

TimedemoBenchmark::TimedemoBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo)
    : QObject(),
    verifyAppearanceTimer(0),
    applicationPage(applicationPage),
    timedemo(timedemo),
    _type("common")
{
    verifyAppearanceTimer = new QTimer(this);
    connect(verifyAppearanceTimer, SIGNAL(timeout()), this, SLOT(terminateIfNotAppeared()));
}

QString TimedemoBenchmark::type() {
    return _type;
}

void TimedemoBenchmark::setType(const QString& type) {
    _type = type;
}

void TimedemoBenchmark::terminateIfNotAppeared() {
    verifyAppearanceTimer->stop();
    if (applicationPage->isOnDisplay()) {
        return;
    } else {
        timedemo->displayBenchmarkResults();
        qCritical("Could not activate application page. Either the application page is not the toplevel window or the window manager is not running.\nAborting.");
        exit(EXIT_FAILURE);
    }
}
