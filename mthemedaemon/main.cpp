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

namespace
{
    QtMsgType outputLevel = QtDebugMsg;
}

static int setup_unix_signal_handlers()
{
    struct sigaction sighup, sigterm, sigint, sigusr1;

    sighup.sa_handler = MThemeDaemonServer::hupSignalHandler;
    sigemptyset(&sighup.sa_mask);
    sighup.sa_flags = SA_RESTART;

    if (sigaction(SIGHUP, &sighup, 0) > 0)
        return 1;

    sigterm.sa_handler = MThemeDaemonServer::termSignalHandler;
    sigemptyset(&sigterm.sa_mask);
    sigterm.sa_flags = SA_RESTART;

    if (sigaction(SIGTERM, &sigterm, 0) > 0)
        return 2;

    sigint.sa_handler = MThemeDaemonServer::intSignalHandler;
    sigemptyset(&sigint.sa_mask);
    sigint.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sigint, 0) > 0)
        return 3;

    sigusr1.sa_handler = MThemeDaemonServer::usr1SignalHandler;
    sigemptyset(&sigusr1.sa_mask);
    sigusr1.sa_flags = SA_RESTART;

    if (sigaction(SIGUSR1, &sigusr1, 0) > 0)
        return 4;

     return 0;
 }

static QtMsgType getOutputLevelFromArgs(QStringList arguments)
{
    QString argLevel;
    int index = arguments.indexOf("-output-level") + 1;
    if (index > 0 && index < arguments.length()) {
        argLevel = arguments[index];
    }

    if (argLevel == "debug")
        return QtDebugMsg;
    if (argLevel == "warning")
        return QtWarningMsg;
    if (argLevel == "critical")
        return QtCriticalMsg;

#ifdef __arm__
    return QtCriticalMsg;
#else
    return QtWarningMsg;
#endif
}

static void mMessageHandler(QtMsgType type, const char *msg)
{
    if (type < outputLevel)
        return;

   fprintf(stderr, "%s\n", msg);

   if (type == QtFatalMsg)
       abort();
}

int main(int argc, char **argv)
{
    setup_unix_signal_handlers();

    // make sure we are not loading the maemo6 qt style which
    // could interfere with us
    QApplication::setStyle("windows");
#ifndef HAVE_MEEGOGRAPHICSSYSTEM
    QApplication::setGraphicsSystem("native");
#endif // HAVE_MEEGOGRAPHICSSYSTEM

    QApplication app(argc, argv);

    outputLevel = getOutputLevelFromArgs(app.arguments());
    qInstallMsgHandler(mMessageHandler);

    // Apply custom server address, if the "-address" has been passed as argument
    QString serverAddress;
    int index = app.arguments().indexOf("-address");
    if ((index >= 0) && (index + 1 < app.arguments().count())) {
        serverAddress = app.arguments().at(index + 1);
    }

    MThemeDaemonServer server(serverAddress);

#ifdef CLOSE_ON_ENTER
    KeyPressWaiter keyWaiter;
    QObject::connect(&keyWaiter, SIGNAL(finished()), qApp, SLOT(quit()));
    keyWaiter.start();
#endif

    if (app.arguments().indexOf("-quitimmediately") >= 0) {
        QTimer::singleShot(0, &app, SLOT(quit()));
    }

    index = app.arguments().indexOf("-slowdown");
    if ((index >= 0) && (index + 1 < app.arguments().count())) {
        bool conversionOk;
        int slowDown = app.arguments().at(index + 1).toInt(&conversionOk);
        if (conversionOk) {
            server.setSlowDown(slowDown);
            qDebug() << "Slowing down theme asset requests by" << slowDown << "microseconds";
        }
    }

    return app.exec();
}
