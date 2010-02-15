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

#ifndef DUIAPPLICATION_P_H
#define DUIAPPLICATION_P_H

#include <QObject>
#include "duicomponentdata.h"
#include <DuiLocale>
#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

#include <duiapplicationwindow.h>

class DuiTheme;
class DuiApplication;
class DuiSceneWindow;
class DuiFeedbackPlayer;
class DuiApplicationService;
class DuiDeviceProfile;

class DuiApplicationPrivate
{
    Q_DECLARE_PUBLIC(DuiApplication)

public:
    DuiApplicationPrivate();
    virtual ~DuiApplicationPrivate();

    DuiComponentData *componentData;

    int xDamageEventBase;
    int xDamageErrorBase;

#ifdef Q_WS_X11
    static int handleXError(Display *display, XErrorEvent *event);
#endif

protected:
    DuiApplication *q_ptr;

private:
    void init(int &argc, char **argv, const QString &appIdentifier, DuiApplicationService *service);

    static void releasePrestart();
    static void restorePrestart();

    static void stdExit(int status);
#ifdef Q_WS_X11
    static void setWindowVisibility(Window window, bool visible);
    static void removeWindowsFromSwitcher(bool remove);
#endif
    friend class DuiApplicationServicePrivate;
    friend bool DuiApplicationWindow::close();
};


#endif
