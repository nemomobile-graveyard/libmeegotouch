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

#include <DuiApplication>
#include <QProcess>

#include <qtest.h>

#include "pt_duiapplication.h"

/*
  Test how long it takes to launch an application which is quitting immediately.
*/
void Pt_DuiApplication::processCreation()
{
    executeSelf(QLatin1String("--exit-immediately"));
}

void Pt_DuiApplication::processCreationAndCtor()
{
    executeSelf(QLatin1String("--exit-after-qapp"));
}

void Pt_DuiApplication::ctor()
{
    DuiApplication *a;
    QBENCHMARK {
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_duiapplication";
        fakeArgv[0] = appName;
        a = new DuiApplication(fakeArgc, fakeArgv);
    }
    delete a;
}

void Pt_DuiApplication::ctor2()
{
    DuiApplication *a;
    QBENCHMARK {
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_duiapplication2";
        fakeArgv[0] = appName;
        a = new DuiApplication(fakeArgc, fakeArgv);
    }
    delete a;
}

void Pt_DuiApplication::executeSelf(const QLatin1String &parameter)
{
    QProcess proc;
    const QString program       = "./pt_duiapplication";
    const QStringList arguments = QStringList() << QLatin1String(parameter);

    QBENCHMARK {
        proc.start(program, arguments);
        QVERIFY(proc.waitForStarted());
        QVERIFY(proc.waitForFinished());
        QCOMPARE(proc.exitStatus(), QProcess::NormalExit);
        QCOMPARE(proc.exitCode(), 0);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2 && QLatin1String("--exit-immediately") == QLatin1String(argv[1])) {
        return 0;
    }

    if (argc == 2 && QLatin1String("--exit-after-qapp") == QLatin1String(argv[1])) {
        DuiApplication app(argc, argv);
        return 0;
    }

    Pt_DuiApplication test;
    return QTest::qExec(&test, argc, argv);
}

