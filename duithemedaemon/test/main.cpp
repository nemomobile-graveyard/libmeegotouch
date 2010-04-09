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

#include <QDebug>
#include <QApplication>
#include <QDir>
#include <DuiGConfItem>
#include <theme/duithemedaemon.h>
#include "keypresswaiter.h"
#include "clientmanager.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#ifdef HAVE_GCONF
    const QString themeName = "theme_one";
    QDir themeDirectory(THEME_ROOT_DIRECTORY + '/' + themeName);
    if (themeDirectory.exists()) {
        DuiGConfItem theme("/Dui/theme/name");
        theme.set(themeName);
    }
#endif

    QProcess td;
    td.start("./testdaemon/testdaemon", QStringList());
    // start theme daemon
    if (td.waitForStarted()) {
        // This is the class that handles the test
        ClientManager manager(td);

        // close on enter
        KeyPressWaiter keyWaiter;
        QObject::connect(&keyWaiter, SIGNAL(finished()), &manager, SLOT(stop()));
        keyWaiter.start();

        // event loop
        int result = app.exec();

        // stop theme daemon
        if (td.state() == QProcess::Running) {
            td.close();
        }
        return result;
    }

    qDebug() << "Theme daemon failed to start";
    return 0;
}
