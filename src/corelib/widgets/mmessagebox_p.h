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

#ifndef MMESSAGEBOX_P_H
#define MMESSAGEBOX_P_H

#include "mmessagebox.h"
#include "mdialog_p.h"

#ifdef HAVE_LIBNGF
#   include <dbus/dbus.h>
#   include <libngf/client.h>
#   ifdef HAVE_CONTEXTSUBSCRIBER
#      include <contextproperty.h>
#   endif
#endif

class MMessageBoxPrivate : public MDialogPrivate
{
    Q_DECLARE_PUBLIC(MMessageBox)
public:
    MMessageBoxPrivate();
    ~MMessageBoxPrivate();

    void init();

#ifdef HAVE_LIBNGF
    void _q_onSceneWindowStateChanged(MSceneWindow::SceneWindowState newState,
                                      MSceneWindow::SceneWindowState oldState);
    bool isScreenBlanked();
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty *screenBlankedProperty;
#endif

    DBusConnection* connection;
    NgfClient* client;
#endif
};

#endif
