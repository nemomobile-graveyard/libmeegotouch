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

#include <duiscenemanager.h>
#include "duiviewcreator.h"
#include "duiscenewindow.h"
#include "duiscenewindow_p.h"
#include "duiscenewindowview.h"
#include "duiscenewindowview_p.h"
#include "duiclassfactory.h"

DuiSceneWindowViewPrivate::DuiSceneWindowViewPrivate() :
    controller(0)
{
}

DuiSceneWindowViewPrivate::~DuiSceneWindowViewPrivate()
{
}


DuiSceneWindowView::DuiSceneWindowView(DuiSceneWindow *controller) :
    DuiWidgetView(*new DuiSceneWindowViewPrivate, controller)
{
    Q_D(DuiSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

DuiSceneWindowView::DuiSceneWindowView(DuiSceneWindowViewPrivate &dd, DuiSceneWindow *controller) :
    DuiWidgetView(dd, controller)
{
    Q_D(DuiSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

DuiSceneWindowView::~DuiSceneWindowView()
{
}

void DuiSceneWindowView::applyStyle()
{
    DuiWidgetView::applyStyle();

    emit geometryAttributesChanged();
}

void DuiSceneWindowView::setupModel()
{
    DuiWidgetView::setupModel();
    emit geometryAttributesChanged();
}

Qt::Alignment DuiSceneWindowView::alignment() const
{
    Qt::Alignment verticalAlign = style()->verticalAlign() & Qt::AlignVertical_Mask;
    Qt::Alignment horizontalAlign = style()->horizontalAlign() & Qt::AlignHorizontal_Mask;

    return verticalAlign | horizontalAlign;
}

QPointF DuiSceneWindowView::offset() const
{
    return style()->offset();
}

DUI_REGISTER_VIEW_NEW(DuiSceneWindowView, DuiSceneWindow)
