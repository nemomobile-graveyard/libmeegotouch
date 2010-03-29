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

#ifndef DUICOMPONENTDATA_P_H
#define DUICOMPONENTDATA_P_H

#include <QObject>
#include <DuiLocale>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

class DuiTheme;
class DuiComponentData;
class DuiFeedbackPlayer;
class DuiApplicationService;
class DuiWindow;
class DuiApplicationWindow;
class DuiDeviceProfile;
#ifdef TESTABLE
class TestabilityInterface;
#endif

class DuiComponentDataPrivate
{
    Q_DECLARE_PUBLIC(DuiComponentData)

public:
    DuiComponentDataPrivate();
    virtual ~DuiComponentDataPrivate();

    bool softwareRendering;
    bool fullScreen;
    bool showBoundingRect;
    bool showSize;
    bool showPosition;
    bool showMargins;
    bool showObjectNames;
    bool showFps;
    bool logFps;
    bool showCursor;
    bool reverseLayout;
    bool prestarted;
    bool emulateTwoFingerGestures;
    Dui::PrestartMode prestartMode;
    DuiTheme *theme;
    DuiDeviceProfile *deviceProfile;
    QList<DuiWindow *> windows;
    DuiFeedbackPlayer *feedbackPlayer;
    DuiLocale locale;
    QString imglistFilename;
    QString appName;
    QString binaryName;
    QString deviceName;
    DuiApplicationService *service;
#ifdef TESTABLE
    // member to hold reference to the testability plugin in order to destroy plugin when the application is destructed
    TestabilityInterface *testabilityInterface;
#endif

protected:
    DuiComponentData *q_ptr;
private:
    void init(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service);
    void debugInit(bool levelSet);
};


#endif
