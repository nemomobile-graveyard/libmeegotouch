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

#include <QProcess>
#include <QStringList>

#include <MApplication>
#include <MApplicationService>
#include <MApplicationIfAdaptor>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MAction>
#include <MLocale>
#include <signal.h>

#include "mainpage.h"
#include "lifecycleapplication.h"

void emitMemorySignal(int sig);

MApplication *App;
int requestCounter = 0;

class MyApplicationService: public MApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        MApplicationService("com.nokia.lifecycle", parent)
    {}

    void handleServiceRegistrationFailure() {
        qDebug() << "MyApplicationService::handleServiceRegistrationFailure()";

        incrementAndRegister();
    }
};


int main(int argc, char **argv)
{
    LifeCycleApplication app(argc, argv, "lifecycle", new MyApplicationService());
    LifeCycleApplication::setPrestartMode(M::LazyShutdown);

    MApplicationWindow window;
    window.show();

    MainPage mainPage;
    mainPage.appear();

    (void) signal(SIGINT, emitMemorySignal);
    App = &app;

    // Run activateWidgets() here to setup things if app is NOT prestarted, otherwise
    // connect it to prestartReleased() -signal from MApplication so that it's run
    // at the time when the window is really being shown to the user.

    if (!app.isPrestarted()) {
        mainPage.activateWidgets();
    } else {
        app.connect(&app, SIGNAL(prestartReleased()), &mainPage, SLOT(activateWidgets()));
        app.connect(&app, SIGNAL(prestartRestored()), &mainPage, SLOT(deactivateWidgets()));
    }

    return app.exec();
}

void emitMemorySignal(int)
{
    // memory signal cannot be emitted in the current setting.
    requestCounter++;

    if (requestCounter >= 2) {
        (void) signal(SIGINT, SIG_DFL);
    }
}
