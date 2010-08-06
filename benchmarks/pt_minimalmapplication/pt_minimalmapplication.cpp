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

#include "pt_minimalmapplication.h"
#include "../performancebenchmark/emptymainloophelper.h"

#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MSceneManager>
#include <MLocale>

#include <QProcess>
#include <QTimer>
#include <qtest.h>

#define MY_QBENCHMARK_ONCE(CODE) \
    if (!noBenchmark) { \
        QBENCHMARK_ONCE { \
            static bool firstRun = true; \
            if (!firstRun) { \
                firstRun = false; \
            } else { \
                CODE \
            } \
        } \
    } else { \
        CODE \
    }

Pt_minimalmapplication::Pt_minimalmapplication()
    :  appearType("appear"),
       noBenchmark(false),
       locale(0),
       window(0),
       page(0)
{
}

int Pt_minimalmapplication::executeAll()
{
    noBenchmark = true;

    createMLocale();
    createMApplicationWindow();
    windowShow();
    createMApplicationPage();
    pageAppear();

    // terminate as soon as startup is complete and the main loop is empty
    EmptyMainLoopHelper mainLoopHelper;
    mainLoopHelper.triggerTermination(EmptyMainLoopHelper::ExitOnEmpty);

    return qApp->exec();
}

void Pt_minimalmapplication::createMLocale()
{
    MY_QBENCHMARK_ONCE(
        locale = new MLocale();
    )
}

void Pt_minimalmapplication::createMApplicationWindow()
{
    MY_QBENCHMARK_ONCE(
        window = new MApplicationWindow();
    )
}

void Pt_minimalmapplication::windowShow()
{
    MY_QBENCHMARK_ONCE(
        window->show();
    )
}

void Pt_minimalmapplication::createMApplicationPage()
{
    MY_QBENCHMARK_ONCE(
        page = new MApplicationPage();
    )
}

void Pt_minimalmapplication::pageAppear()
{
    if (appearType == "appear") {
        MY_QBENCHMARK_ONCE(page->appear(window);)
    } else {
        MY_QBENCHMARK_ONCE(window->sceneManager()->appearSceneWindowNow(page);)
    }
}

void Pt_minimalmapplication::overallRuntime()
{
    executeSelf("appear");
}

void Pt_minimalmapplication::overallRuntimeAppearNow()
{
    executeSelf("appearNow");
}

void Pt_minimalmapplication::executeSelf(const QString &param)
{
    QProcess proc;
    const QString program = QCoreApplication::applicationFilePath();
    const QStringList arguments = QStringList() << QLatin1String("--exit-immediately") << param;

    QBENCHMARK {
        // execute ourselve and check for success
        proc.start(program, arguments);
        QVERIFY(proc.waitForStarted());
        QVERIFY(proc.waitForFinished());
        QCOMPARE(proc.exitStatus(), QProcess::NormalExit);
        QCOMPARE(proc.exitCode(), 0);
    }
}

int main(int argc, char **argv)
{
    // fake arc and argv to use the remote theme daemon
    int fakeArgc(1);//2;
    char *fakeArgv[fakeArgc];
    char appName[] = "./pt_minimalmapplication";
    fakeArgv[0] = appName;

    //#MS - waiting for remote daemon get stucked the unit test..
    //char remoteTheme[] = "-remote-theme";
    //fakeArgv[1] = remoteTheme;

    if (argc >= 3 && strcmp("--exit-immediately", argv[1]) == 0
            && (strcmp("appear", argv[2]) == 0 || strcmp("appearNow", argv[2]) == 0)) {
        // a MApplication can just run once. create the second one with a different name
        char appName[] = "./pt_minimalmapplication2";
        fakeArgv[0] = appName;
        MApplication a(fakeArgc, fakeArgv);

        Pt_minimalmapplication test;
        test.appearType = argv[2];
        return test.executeAll();
    }

    MApplication a(fakeArgc, fakeArgv);
    Pt_minimalmapplication test;

    return QTest::qExec(&test, argc, argv);
}
