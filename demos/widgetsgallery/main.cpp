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
#include <QDebug>

#include <MApplication>
#include <MApplicationService>
#ifdef HAVE_DBUS
#include <MApplicationIfAdaptor>
#endif //HAVE_DBUS
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MAction>
#include <MLocale>
#include <MGConfItem>
#include <MSceneManager>
#include <MComponentCache>

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

#ifndef Q_OS_WIN
M_EXPORT
#endif
int main(int argc, char **argv)
{
#ifdef M_OS_MAEMO5
    QApplication::setGraphicsSystem(QLatin1String("native"));
#endif //M_OS_MAEMO5
    //MApplication application(argc, argv, "widgetsgallery", new MyApplicationService() );
#ifndef Q_OS_WIN
    MApplication *application = MComponentCache::mApplication(argc, argv);
#else
    MApplication *application = new MApplication(argc, argv);
#endif

    WidgetsgalleryRetranslator widgetsgalleryRetranslator;
    QObject::connect(application, SIGNAL(localeSettingsChanged()), &widgetsgalleryRetranslator, SLOT(widgetsgalleryRetranslate()));

#ifndef Q_OS_WIN
    MApplicationWindow* window = MComponentCache::mApplicationWindow();
#else
    MApplicationWindow* window = new MApplicationWindow();
#endif

    MainPage *mainPage = new MainPage;
    Timedemo *timedemo = 0;
    if (qApp->arguments().indexOf("-timedemo") >= 0) {
        QStringList demoPages;
        int idx = qApp->arguments().indexOf("-demopages");
        if (idx >= 0 && idx + 1 < qApp->arguments().count()) {
            demoPages = qApp->arguments()[idx + 1].split(',');
        }

        timedemo = new Timedemo(mainPage, demoPages);

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
          mainPage->setInitialPageToShow(qApp->arguments()[index + 1]);
      }
    }

    EmptyMainLoopHelper mainLoopHelper;
    // -quitimmediately will create fully the content of the main page
    // -exitimmediately will skip creating the content of the main page
    if (qApp->arguments().indexOf("-quitimmediately") >= 0 || qApp->arguments().indexOf("-exitimmediately") >= 0) {
        // terminate widgetsgallery as soon as the content is shown and main loop is idle
        mainPage->setMainLoopHelper(&mainLoopHelper);
        mainLoopHelper.triggerTermination(EmptyMainLoopHelper::QuitOnEmpty);
    }

    window->sceneManager()->appearSceneWindowNow(mainPage);
    window->show();

    int exitCode = application->exec();
    delete timedemo;
    delete mainPage;
    delete window;
    delete application;
    return exitCode;
}
