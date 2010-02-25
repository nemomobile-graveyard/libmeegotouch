/****************************************************************************
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

/**
* \defgroup applicationextensionrunner DUI application extension runner
*
* Runner process for the out-of-process (oop) application extensions.
*/

/**
* \file
* \ingroup applicationextensionrunner
*/

#include <QApplication>
#include <QStringList>
#include <QProcess>

#include "duiapplicationextensionrunner.h"
#include <DuiApplication>
#include <DuiApplicationService>
#include <QPluginLoader>
#include <QDebug>
#include <duiapplicationextensionmetadata.h>

/// Error code for initialization failure
static const int ERROR_APPLICATION_EXTENSION_INIT_FAILED = -2;
/// Error code for invalid command line arguments
static const int ERROR_INVALID_ARGUMENTS = -3;

class MyApplicationService: public DuiApplicationService
{
public:
    MyApplicationService(QObject *parent = 0) :
        DuiApplicationService(QString(), parent) {
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
 * - communication server name
 * - metadata file name
 */
int main(int argc, char **argv)
{
    // Check for correct number of arguments
    if (argc != 3) {
        qWarning() << "Invalid number of arguments";
        return ERROR_INVALID_ARGUMENTS;
    }

    // Load metadata
    DuiApplicationExtensionMetaData metadata(argv[2]);
    if (!metadata.isValid()) {
        qWarning() << "Metadata " << argv[2] << "is not valid";
        return ERROR_APPLICATION_EXTENSION_INIT_FAILED;
    }

    DuiApplication app(argc, argv, metadata.resourceIdentifier(), new MyApplicationService());
    DuiApplicationExtensionRunner runner;
    if (!runner.init(argv[1], metadata)) {
        qWarning() << "Application extension initialization failed";
        return ERROR_APPLICATION_EXTENSION_INIT_FAILED;
    }

    return app.exec();
}
