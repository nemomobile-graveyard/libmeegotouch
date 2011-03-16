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
#include <QUrl>
#include <MLocale>
#ifdef Q_WS_X11
#include <QStack>
#include <X11/Xlib.h>
#endif

#include "mtheme.h"

#ifdef HAVE_GCONF
#include "mgconfitem.h"
#endif

class MComponentData;
class MFeedbackPlayer;
class MApplicationService;
class MWindow;
class MApplicationWindow;
class MDeviceProfile;
class TestabilityInterface;

class MComponentDataPrivate
{
    Q_DECLARE_PUBLIC(MComponentData)

public:
    MComponentDataPrivate();
    virtual ~MComponentDataPrivate();

    void parseArguments(int &argc, char **argv,
                        MTheme::ThemeService &themeService);

    void _q_notifyInputMethodActiveWindowOrientationChangeStarted();
    void _q_notifyInputMethodActiveWindowOrientationChangeFinished();
#ifdef HAVE_GCONF
    void _q_updateDebugOptionsFromGConfValues();
#endif
    bool softwareRendering;
    bool fullScreen;
    bool showBoundingRect;
    bool showSize;
    bool showPosition;
    bool showMargins;
    bool showObjectNames;
    bool showStyleNames;
    bool showFps;
    bool logFps;
    bool showCursor;
    bool reverseLayout;
    bool prestarted;
    bool emulateTwoFingerGestures;
    M::PrestartMode prestartMode;
    M::OrientationAngle forcedOrientationAngle;
    bool isOrientationForced;
    MTheme *theme;
    MDeviceProfile *deviceProfile;
    QList<MWindow *> windows;
    MWindow *firstWindow;
    MFeedbackPlayer *feedbackPlayer;
    MLocale locale;
    QString appName;
    QString binaryName;
    QString deviceName;
    QUrl syslogServer;
    MApplicationService *service;
    TestabilityInterface *testabilityInterface;

#ifdef Q_WS_X11
    static QStack<MComponentData::ChainData> chainData;
#endif

#ifdef HAVE_GCONF
    MGConfItem debugOptions;
    /*
     ONLY FOR DEVELOPMENT -remove in the future-
     gconf value (string) containing an emulation of bitarray
      _ _ _ _ _ _ _
     |_|_|_|_|_|_|_|
      |
      |
      |
     This can be 0 or 1, if it is other value (a string) the position will be consider as false.
     If one value is not provided the position is initialized to false too.
    */
#endif

protected:
    MComponentData *q_ptr;
private:
    void init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);
    void registerDefaultService(const QString &appName);
    void registerNewService(MApplicationService *newService);
    void debugInit(bool levelSet);
    void testabilityInit();
    bool initSyslogConnection(const QUrl &url);
};

#endif
