/****************************************************************************
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

/**
* \defgroup appletrunner M applet runner
*
* Runner process for the out-of-process (oop) applets.
*/

/**
* \file
* \ingroup appletrunner
*/

#include <QApplication>
#include <QStringList>
#include <QProcess>

#include "mappletrunner.h"
#include <MApplication>
#include <MApplicationService>
#include <QPluginLoader>
#include <QDebug>
#include <mappletmetadata.h>

/// Error code for applet initialization failure
static const int ERROR_APPLET_INIT_FAILED = -2;
/// Error code for invalid command line arguments
static const int ERROR_INVALID_ARGUMENTS = -3;

class MyApplicationService: public MApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        MApplicationService(QString(), parent) {
    }

    void launch() {
        launchAnotherWithQProcess();
    }

    virtual void handleServiceRegistrationFailure() {
        qDebug() << "MyApplicationService::handleServiceRegistrationFailure()";
    }

    virtual QString registeredName() {
        return QString();
    }

    virtual bool isRegistered() {
        return false;
    }

    virtual bool registerService() {
        return true;
    }

};

/**
 * The arguments to the executable must be:
 * - applet communication server name
 * - metadata file name
 * - applet ID as a string
 * - applet instance file data path
 */
int main(int argc, char **argv)
{
    // Check for correct number of arguments
    if (argc != 5) {
        qWarning() << "Invalid number of arguments";
        return ERROR_INVALID_ARGUMENTS;
    }

    // Load applet metadata
    MAppletMetaData metadata(argv[2]);
    if (!metadata.isValid()) {
        qWarning() << "Applet metadata " << argv[2] << "is not valid";
        return ERROR_APPLET_INIT_FAILED;
    }

    MApplication app(argc, argv, metadata.resourceIdentifier(), new MyApplicationService());
    MAppletRunner runner;
    if (!runner.init(argv[1], metadata, argv[3], argv[4])) {
        qWarning() << "Applet initialization failed";
        return ERROR_APPLET_INIT_FAILED;
    }

    return app.exec();
}
