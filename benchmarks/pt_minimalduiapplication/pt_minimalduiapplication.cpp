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

#include "pt_minimalduiapplication.h"
#include "../performancebenchmark/emptymainloophelper.h"

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiSceneManager>
#include <DuiLocale>

#include <QProcess>
#include <QTimer>
#include <qtest.h>

#define MY_QBENCHMARK_ONCE(CODE) \
    if (!noBenchmark) \
        QBENCHMARK_ONCE { \
        CODE \
    } else { \
        CODE \
    }

Pt_minimalduiapplication::Pt_minimalduiapplication()
    :  appearType("appear"), noBenchmark(false)
{
}

int Pt_minimalduiapplication::executeAll()
{
    noBenchmark = true;

    createDuiLocale();
    createDuiApplicationWindow();
    windowShow();
    createDuiApplicationPage();
    pageAppear();

    // terminate as soon as startup is complete and the main loop is empty
    EmptyMainLoopHelper mainLoopHelper;
    mainLoopHelper.triggerTermination(EmptyMainLoopHelper::ExitOnEmpty);

    return qApp->exec();
}

void Pt_minimalduiapplication::createDuiLocale()
{
    MY_QBENCHMARK_ONCE(
        locale = new DuiLocale();
    )
}

void Pt_minimalduiapplication::createDuiApplicationWindow()
{
    MY_QBENCHMARK_ONCE(
        window = new DuiApplicationWindow();
    )
}

void Pt_minimalduiapplication::windowShow()
{
    MY_QBENCHMARK_ONCE(
        window->show();
    )
}

void Pt_minimalduiapplication::createDuiApplicationPage()
{
    MY_QBENCHMARK_ONCE(
        page = new DuiApplicationPage();
    )
}

void Pt_minimalduiapplication::pageAppear()
{
    if (appearType == "appear") {
        MY_QBENCHMARK_ONCE(page->appear();)
    } else {
        MY_QBENCHMARK_ONCE(window->sceneManager()->appearSceneWindowNow(page);)
    }
}

void Pt_minimalduiapplication::overallRuntime()
{
    executeSelf("appear");
}

void Pt_minimalduiapplication::overallRuntimeAppearNow()
{
    executeSelf("appearNow");
}

void Pt_minimalduiapplication::executeSelf(const QString &param)
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
    char appName[] = "./pt_minimalduiapplication";
    fakeArgv[0] = appName;

    //#MS - waiting for remote daemon get stucked the unit test..
    //char remoteTheme[] = "-remote-theme";
    //fakeArgv[1] = remoteTheme;

    if (argc >= 3 && strcmp("--exit-immediately", argv[1]) == 0
            && (strcmp("appear", argv[2]) == 0 || strcmp("appearNow", argv[2]) == 0)) {
        // a DuiApplication can just run once. create the second one with a different name
        char appName[] = "./pt_minimalduiapplication2";
        fakeArgv[0] = appName;
        DuiApplication a(fakeArgc, fakeArgv);

        Pt_minimalduiapplication test;
        test.appearType = argv[2];
        return test.executeAll();
    }

    DuiApplication a(fakeArgc, fakeArgv);
    Pt_minimalduiapplication test;

    return QTest::qExec(&test, argc, argv);
}
