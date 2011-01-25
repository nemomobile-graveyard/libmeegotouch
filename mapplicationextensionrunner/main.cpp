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
* \defgroup applicationextensionrunner M application extension runner
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
#include <QFile>

#include "mapplicationextensionrunner.h"
#include <MApplication>
#include <MApplicationService>
#include <MDebug>
#include <QPluginLoader>
#include <mapplicationextensionmetadata.h>

/// Error code for initialization failure
static const int ERROR_APPLICATION_EXTENSION_INIT_FAILED = -2;
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
    // Set the out of memory adjustment value
    QFile file("/proc/self/oom_adj");
    if (file.open(QIODevice::WriteOnly)) {
        file.write("-1");
        file.close();
    }

    // Check for correct number of arguments
    if (argc != 3) {
        mWarning("MApplicationExtensionRunner") << "Invalid number of arguments";
        return ERROR_INVALID_ARGUMENTS;
    }

    // Load metadata
    MApplicationExtensionMetaData metadata(argv[2]);
    if (!metadata.isValid()) {
        mWarning("MApplicationExtensionRunner") << "Application extension" << argv[2] << "metadata is not valid.";
        return ERROR_APPLICATION_EXTENSION_INIT_FAILED;
    }

    // Set the output level to warning to help debugging
    if (getenv("M_OUTPUT_LEVEL") == NULL) {
        setenv("M_OUTPUT_LEVEL", "warning", 0);
    }

    int returnValue = 0;
    MApplication *app = new MApplication(argc, argv, metadata.resourceIdentifier(), new MyApplicationService());
    MApplicationExtensionRunner *runner = new MApplicationExtensionRunner;
    if (!runner->init(argv[1], metadata)) {
        mWarning("MApplicationExtensionRunner") << "Application extension" << argv[2] << "initialization failed.";
        returnValue = ERROR_APPLICATION_EXTENSION_INIT_FAILED;
    } else {
        returnValue = app->exec();
    }
    delete runner;
    delete app;

    return returnValue;
}
