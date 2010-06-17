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

#include <MApplication>
#include <QProcess>

#include <qtest.h>

#include "pt_mapplication.h"

/*
  Test how long it takes to launch an application which is quitting immediately.
*/
void Pt_MApplication::processCreation()
{
    executeSelf(QLatin1String("--exit-immediately"));
}

void Pt_MApplication::processCreationAndCtor()
{
    executeSelf(QLatin1String("--exit-after-qapp"));
}

void Pt_MApplication::ctor()
{
    MApplication *a = NULL;
    QBENCHMARK_ONCE {
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_mapplication";
        fakeArgv[0] = appName;
        a = new MApplication(fakeArgc, fakeArgv);
    }
    delete a;
}

void Pt_MApplication::ctor2()
{
    MApplication *a = NULL;
    QBENCHMARK_ONCE {
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_mapplication2";
        fakeArgv[0] = appName;
        a = new MApplication(fakeArgc, fakeArgv);
    }
    delete a;
}

void Pt_MApplication::executeSelf(const QLatin1String &parameter)
{
    QProcess proc;
    const QString program       = "./pt_mapplication";
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
        MApplication app(argc, argv);
        return 0;
    }

    Pt_MApplication test;
    return QTest::qExec(&test, argc, argv);
}

