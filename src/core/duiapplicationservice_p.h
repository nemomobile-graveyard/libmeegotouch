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

#ifndef DUIAPPLICATIONSERVICE_P_H
#define DUIAPPLICATIONSERVICE_P_H

#include <QString>
#include <QDBusConnection>

class DuiApplicationService;

class DuiApplicationServicePrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationService)

public:
    DuiApplicationServicePrivate(const QString &newService);
    virtual ~DuiApplicationServicePrivate();

    QString serviceName;
    bool registered;
    int instanceCounter;

protected:
    DuiApplicationService *q_ptr;

    void launchNewProcess(const QString &binaryName, const QStringList &arguments);

    QString appName();
    QString binaryName();
    QStringList arguments();
    bool registerService(const QString &serviceName);
    void registerObject(const QString &path, QObject *object);
    void unregisterObject(const QString &path);
    void stdExit(int exitValue);
    void activateActiveWindow();
    void raiseActiveWindow();
    bool thisAppRunWithDBus();
    void closeAllWindows();
    bool prestartModeIsLazyShutdown();
    void releasePrestart();
    void restorePrestart();
    bool isPrestarted();
    bool activeWindowSet();

private:
    QDBusConnection dBusConnection;
    DuiApplication *duiApp;

};

#endif

