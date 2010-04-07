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

#include "duithemedaemonserver.h"
#include <QDebug>
#include <QApplication>
#include <signal.h>

#ifdef CLOSE_ON_ENTER
#include "keypresswaiter.h"
#endif

void sigclose(int)
{
    // kill the daemon so that it can save it's current state (caches, refcounts, etc)
    qApp->quit();
}

int main(int argc, char **argv)
{
    signal(SIGTERM, sigclose);
    signal(SIGINT, sigclose);

    QApplication::setGraphicsSystem(QLatin1String("native"));

    QApplication app(argc, argv);

    DuiThemeDaemonServer server;

#ifdef CLOSE_ON_ENTER
    KeyPressWaiter keyWaiter;
    QObject::connect(&keyWaiter, SIGNAL(finished()), qApp, SLOT(quit()));
    keyWaiter.start();
#endif

    if (app.arguments().indexOf("-quitimmediately") >= 0) {
        QTimer::singleShot(0, &app, SLOT(quit()));
    }

    return app.exec();
}
