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

#include "mdockwidgetview.h"
#include "mdockwidgetview_p.h"
#include "mdockwidget.h"
#include <QGraphicsLinearLayout>

#include "mtheme.h"
#include "mviewcreator.h"

MDockWidgetViewPrivate::MDockWidgetViewPrivate()
    : controller(0)
{
}

MDockWidgetViewPrivate::~MDockWidgetViewPrivate()
{
}

MDockWidgetView::MDockWidgetView(MDockWidget *controller)
    : MSceneWindowView(* new MDockWidgetViewPrivate, controller)
{
    Q_D(MDockWidgetView);
    d->controller = controller;
}

MDockWidgetView::~MDockWidgetView()
{
}

M_REGISTER_VIEW_NEW(MDockWidgetView, MDockWidget)
