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

#ifndef DUIDOCKWIDGETVIEW_H
#define DUIDOCKWIDGETVIEW_H

#include "duiscenewindowview.h"
#include "duidockwidgetmodel.h"
#include <duidockwidgetstyle.h>

class DuiDockWidgetViewPrivate;
class DuiDockWidget;

//! \internal
class DuiDockWidgetView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiDockWidgetModel, DuiDockWidgetStyle)
public:
    explicit DuiDockWidgetView(DuiDockWidget *controller);
    virtual ~DuiDockWidgetView();

private:
    Q_DISABLE_COPY(DuiDockWidgetView)
    Q_DECLARE_PRIVATE(DuiDockWidgetView)
};

//! \internal_end

#endif // DUIDOCKWIDGETVIEW
