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

#ifndef DUISTATUSBARVIEW_H
#define DUISTATUSBARVIEW_H

#include <duiscenewindowview.h>
#include <duiscenewindowmodel.h>

class DuiStatusBar;

//! \internal

class DuiStatusBarView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiSceneWindowModel, DuiSceneWindowStyle)

public:
    DuiStatusBarView(DuiStatusBar *controller);
    virtual ~DuiStatusBarView();

private:
    Q_DISABLE_COPY(DuiStatusBarView)
};

//! \internal_end

#endif
