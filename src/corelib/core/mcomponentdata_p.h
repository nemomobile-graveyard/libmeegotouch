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

#ifndef MCOMPONENTDATA_P_H
#define MCOMPONENTDATA_P_H

#include <QObject>
#include <MLocale>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

class MTheme;
class MComponentData;
class MFeedbackPlayer;
class MApplicationService;
class MWindow;
class MApplicationWindow;
class MDeviceProfile;
#ifdef TESTABLE
class TestabilityInterface;
#endif

class MComponentDataPrivate
{
    Q_DECLARE_PUBLIC(MComponentData)

public:
    MComponentDataPrivate();
    virtual ~MComponentDataPrivate();

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
    M::PrestartMode prestartMode;
    MTheme *theme;
    MDeviceProfile *deviceProfile;
    QList<MWindow *> windows;
    MFeedbackPlayer *feedbackPlayer;
    MLocale locale;
    QString imglistFilename;
    QString appName;
    QString binaryName;
    QString deviceName;
    MApplicationService *service;
#ifdef TESTABLE
    // member to hold reference to the testability plugin in order to destroy plugin when the application is destructed
    TestabilityInterface *testabilityInterface;
#endif

protected:
    MComponentData *q_ptr;
private:
    void init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);
    void debugInit(bool levelSet);
};


#endif
