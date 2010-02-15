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

#ifndef DUIDOCKWIDGET_P_H
#define DUIDOCKWIDGET_P_H

#include "duiscenewindow_p.h"
#include "duidockwidget.h"

class QGraphicsLinearLayout;
class DuiToolBar;
class DuiApplicationWindow;

class DuiDockWidgetPrivate : public DuiSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(DuiDockWidget)

public:
    DuiDockWidgetPrivate();
    virtual ~DuiDockWidgetPrivate();
    void init();

    QGraphicsLinearLayout *layout;
    DuiToolBar *toolbar;

};

#endif
