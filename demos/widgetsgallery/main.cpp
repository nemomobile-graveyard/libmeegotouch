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
#include <QObject>

#include <MApplication>
#include <MApplicationService>
#include <MApplicationIfAdaptor>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MAction>
#include <MLocale>
#include <MGConfItem>
#include <MSceneManager>

#include "mainpage.h"
#include "timedemo.h"
#include "swaphook.h"
#include "widgetsgalleryretranslator.h"
#include "../../benchmarks/performancebenchmark/emptymainloophelper.h"

class MyApplicationService: public MApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        MApplicationService("com.nokia.widgetsgallery", parent) {
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
#ifdef HAVE_N900
    QApplication::setGraphicsSystem(QLatin1String("native"));
#endif
    //MApplication application(argc, argv, "widgetsgallery", new MyApplicationService() );
    MApplication application(argc, argv, "widgetsgallery");

    WidgetsgalleryRetranslator widgetsgalleryRetranslator;
    QObject::connect(&application, SIGNAL(localeSettingsChanged()), &widgetsgalleryRetranslator, SLOT(widgetsgalleryRetranslate()));

    MApplicationWindow window;
    window.show();

    MainPage mainPage;
    Timedemo *timedemo = 0;
    if (qApp->arguments().indexOf("-timedemo") >= 0) {
        QStringList demoPages;
        int idx = qApp->arguments().indexOf("-demopages");
        if (idx >= 0 && idx + 1 < qApp->arguments().count()) {
            demoPages = qApp->arguments()[idx + 1].split(',');
        }

        timedemo = new Timedemo(&mainPage, demoPages);

        idx = qApp->arguments().indexOf("-outputcsv");
        if (idx >= 0 && idx + 1 < qApp->arguments().count()) {
            timedemo->setOutputCsv(qApp->arguments()[idx + 1]);
        }
        idx = qApp->arguments().indexOf("-framelog");
        if (idx >= 0 && idx + 1 < qApp->arguments().count()) {
            timedemo->setFramelog(qApp->arguments()[idx + 1]);
        }
    }

    int index = qApp->arguments().indexOf("-initialpage");
    if (index >= 0) {
      if (index + 1 < qApp->arguments().count()) {
          mainPage.setInitialPageToShow(qApp->arguments()[index + 1]);
      }
    }

    EmptyMainLoopHelper mainLoopHelper;
    if (qApp->arguments().indexOf("-quitimmediately") >= 0) {
        // terminate widgetsgallery as soon as the content is shown and main loop is idle
        mainLoopHelper.triggerTermination(EmptyMainLoopHelper::QuitOnEmpty);
    }

    window.sceneManager()->appearSceneWindowNow(&mainPage);

    int exitCode = application.exec();
    delete timedemo;
    return exitCode;
}
