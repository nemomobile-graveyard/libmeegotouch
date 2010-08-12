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

#include "pt_qapplication.h"

#include <mbenchmark.h>

#include <QApplication>
#include <QProcess>

#include <qtest.h>


void Pt_QApplication::processCreation()
{
    executeSelf(QLatin1String("--exit-immediately"));
}

void Pt_QApplication::processCreationAndConstructor()
{
    executeSelf(QLatin1String("--exit-after-qapp"));
}

void Pt_QApplication::uncachedConstructor()
{
    QApplication *a(NULL);
    MBENCHMARK_ONCE (
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_qapplication";
        fakeArgv[0] = appName;
        a = new QApplication(fakeArgc, fakeArgv);
    )
    delete a;
}

void Pt_QApplication::cachedConstructor()
{
    QApplication *a(NULL);
    MBENCHMARK_ONCE (
        int fakeArgc = 1;
        char *fakeArgv[fakeArgc];
        char appName[] = "./pt_qapplication2";
        fakeArgv[0] = appName;
        a = new QApplication(fakeArgc, fakeArgv);
    )
    delete a;
}

void Pt_QApplication::executeSelf(const QLatin1String &parameter)
{
    QProcess proc;
    const QString program       = "./pt_qapplication";
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
        QApplication app(argc, argv);
        return 0;
    }

    Pt_QApplication test;
    return QTest::qExec(&test, argc, argv);
}

