/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libm.
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
#include <QTimer>

#include "benchmarkremoteclient.h"
#include "benchmarklocalclient.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    BenchmarkRemoteClient remoteClient;
    BenchmarkLocalClient localClient;

    QTimer::singleShot(0, &remoteClient, SLOT(run()));
    QObject::connect(&remoteClient, SIGNAL(finished()), &localClient, SLOT(run()));
    QObject::connect(&localClient, SIGNAL(finished()), qApp, SLOT(quit()));

    return app.exec();
}
