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

#ifndef MAPPLICATION_P_H
#define MAPPLICATION_P_H

#include <QObject>
#include "mcomponentdata.h"
#include <MLocale>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#include <mwindow.h>

class MTheme;
class MApplication;
class MSceneWindow;
class MFeedbackPlayer;
class MApplicationService;
class MDeviceProfile;

class MApplicationPrivate
{
    Q_DECLARE_PUBLIC(MApplication)

public:
    MApplicationPrivate();
    virtual ~MApplicationPrivate();

    MComponentData *componentData;

    int xDamageEventBase;
    int xDamageErrorBase;

#ifdef Q_WS_X11
    static int handleXError(Display *display, XErrorEvent *event);
#endif

protected:
    MApplication *q_ptr;

private:
    void init(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);

    static void releasePrestart();
    static void restorePrestart();
    static void stdExit(int status);
    static bool prestartModeIsLazyShutdown();
    static bool prestartModeIsMultiWindowed();

#ifdef Q_WS_X11
    static void setWindowVisibility(Window window, bool visible);
    static void removeWindowsFromSwitcher(bool remove);
    static void removeWindowFromSwitcher(Window window, bool remove);
#endif
    friend class MApplicationServicePrivate;
    friend class MWindow;
};


#endif
