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

#include "duithemedaemonserver.h"
#include <QDebug>
#include <QApplication>

#ifdef CLOSE_ON_ENTER
#include "keypresswaiter.h"
#endif

int main(int argc, char **argv)
{
    QApplication::setGraphicsSystem(QLatin1String("native"));

    QApplication app(argc, argv);

    DuiThemeDaemonServer server;

#ifdef CLOSE_ON_ENTER
    KeyPressWaiter keyWaiter;
    QObject::connect(&keyWaiter, SIGNAL(finished()), qApp, SLOT(quit()));
    keyWaiter.start();
#endif

    return app.exec();
}
