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

#include "pt_widgetsgallery.h"

#include <QProcess>
#include <QFileInfo>

#include <qtest.h>

void Pt_WidgetsGallery::startupTime()
{
    const QStringList arguments = QStringList() << "-quitimmediately";

    QBENCHMARK {
        executeWidgetsgallery(arguments);
    }
}

void Pt_WidgetsGallery::startupTimeWindowsStyle()
{
    const QStringList arguments = QStringList() << "-quitimmediately" << "-style" << "windows";

    QBENCHMARK {
        executeWidgetsgallery(arguments);
    }
}

void Pt_WidgetsGallery::executeWidgetsgallery(const QStringList& arguments)
{
    QString program = "widgetsgallery";

    //execute local binary file
    QString sLocalProgram("../../demos/widgetsgallery/widgetsgallery");
    if(QFileInfo(sLocalProgram).exists()) {
        program = sLocalProgram;
    }

    QProcess proc;
    proc.start(program, arguments);
    QVERIFY(proc.waitForStarted());
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitStatus(), QProcess::NormalExit);
    QCOMPARE(proc.exitCode(), 0);
}

QTEST_APPLESS_MAIN(Pt_WidgetsGallery)
