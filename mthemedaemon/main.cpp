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

#include "mthemedaemonserver.h"
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

MThemeDaemonServer *_server;

void sighup(int)
{
    _server->themeChanged(true);
}

int main(int argc, char **argv)
{
    signal(SIGTERM, sigclose);
    signal(SIGINT, sigclose);
    signal(SIGHUP, sighup);

    QApplication::setGraphicsSystem(QLatin1String("native"));

    QApplication app(argc, argv);

    // Apply custom server address, if the "-address" has been passed as argument
    QString serverAddress;
    const int index = app.arguments().indexOf("-address");
    if ((index >= 0) && (index + 1 < app.arguments().count())) {
        serverAddress = app.arguments().at(index + 1);
    }

    MThemeDaemonServer server(serverAddress);
    _server = &server;

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
