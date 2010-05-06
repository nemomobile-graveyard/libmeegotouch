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

#include <QApplication>
#include <QtDebug>

#include "textprocessor.h"
#include "textprocessorinterfaceadaptor.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // implements the text processor interface methods
    TextProcessor        service;

    // memory owned by QDBusAbstractAdaptor instance and must be on the heap
    new TextProcessorInterfaceAdaptor(&service);

    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("org.maemo.TextProcessor");
    qDebug() << "textprocessor::main::registerService: org.maemo.TextProcessor: " << ret;
    ret = connection.registerObject("/", &service);
    qDebug() << "textprocessor::main::registerObject: /: " << ret;

    int retVal = app.exec();

    return retVal;
}
