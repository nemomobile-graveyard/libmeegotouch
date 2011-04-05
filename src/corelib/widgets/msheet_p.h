/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSHEET_P_H
#define MSHEET_P_H

#include "msheet.h"
#include "mscenewindow_p.h"

class MSheetPrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MSheet)
public:
    MSheetPrivate();
    virtual ~MSheetPrivate();

    // reimplemented from MSceneWindowPrivate
    virtual bool canDisappear();
    virtual bool canDismiss();

    bool canDisappear(bool dismissing);

    void _q_makeSystemSheetDisappear();
    void _q_dismissSystemSheet();
    void _q_onCentralWidgetDestroyed();
    void _q_onHeaderWidgetDestroyed();

#ifdef Q_WS_X11
    void appendMSheetTypePropertyToStandAloneWindow();
#endif

    void _q_onStandAloneSheetDisappeared();
    void appearSystemwide(MSceneWindow::DeletionPolicy policy);

    // Objects used to display the sheet systemwide (inside a separate
    // top-level MWindow)
    MWindow *standAloneWindow;
    MSceneWindow::DeletionPolicy *appearSystemwideDeletionPolicy;
};

#endif // MSHEET_P_H
