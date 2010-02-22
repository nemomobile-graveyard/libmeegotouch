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

#include <QProcess>
#include <QStringList>
#include <QObject>

#include <DuiApplication>
#include <DuiApplicationService>
#include <DuiApplicationIfAdaptor>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiAction>
#include <DuiLocale>
#include <DuiGConfItem>

#include "timingscene.h"
#include "listpage.h"
#include "timedemo.h"
#include "widgetsgalleryretranslator.h"
#include "../../benchmarks/performancebenchmark/emptymainloophelper.h"

class MyApplicationService: public DuiApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        DuiApplicationService("com.nokia.widgetsgallery", parent) {
    }

    void launch() {
        launchAnotherWithQProcess();
    }

    void handleServiceRegistrationFailure() {
        qDebug() << "MyApplicationService::handleServiceRegistrationFailure()";

        incrementAndRegister();
    }
};

int main(int argc, char **argv)
{
    //DuiApplication application(argc, argv, "widgetsgallery", new MyApplicationService() );
    DuiApplication application(argc, argv, "widgetsgallery");

    WidgetsgalleryRetranslator widgetsgalleryRetranslator;
    QObject::connect(&application, SIGNAL(localeSettingsChanged()), &widgetsgalleryRetranslator, SLOT(widgetsgalleryRetranslate()));

    TimingScene scene;
    DuiApplicationWindow window(&scene);
    window.show();

    ListPage listPage;
    Timedemo *timedemo = 0;
    if (qApp->arguments().indexOf("-timedemo") >= 0) {
        timedemo = new Timedemo(&scene, &listPage);

        const int idx = qApp->arguments().indexOf("-outputcsv");
        if (idx >= 0 && idx + 1 < qApp->arguments().count()) {
            timedemo->setOutputCsv(qApp->arguments()[idx + 1]);
        }
    }

    EmptyMainLoopHelper mainLoopHelper;
    if (qApp->arguments().indexOf("-quitimmediately") >= 0) {
        // terminate widgetsgallery as soon as the content is shown and main loop is idle
        mainLoopHelper.triggerTermination(EmptyMainLoopHelper::QuitOnEmpty);
    }

    listPage.appearNow();

    int exitCode = application.exec();
    delete timedemo;
    return exitCode;
}
