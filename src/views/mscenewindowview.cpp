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

#include <mscenemanager.h>
#include "mviewcreator.h"
#include "mscenewindow.h"
#include "mscenewindow_p.h"
#include "mscenewindowview.h"
#include "mscenewindowview_p.h"
#include "mclassfactory.h"

MSceneWindowViewPrivate::MSceneWindowViewPrivate() :
    controller(0)
{
}

MSceneWindowViewPrivate::~MSceneWindowViewPrivate()
{
}


MSceneWindowView::MSceneWindowView(MSceneWindow *controller) :
    MWidgetView(*new MSceneWindowViewPrivate, controller)
{
    Q_D(MSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

MSceneWindowView::MSceneWindowView(MSceneWindowViewPrivate &dd, MSceneWindow *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MSceneWindowView);

    d->controller = controller;
    connect(this, SIGNAL(geometryAttributesChanged()), controller, SIGNAL(repositionNeeded()));
}

MSceneWindowView::~MSceneWindowView()
{
}

void MSceneWindowView::applyStyle()
{
    MWidgetView::applyStyle();

    emit geometryAttributesChanged();
}

void MSceneWindowView::setupModel()
{
    MWidgetView::setupModel();
    emit geometryAttributesChanged();
}

Qt::Alignment MSceneWindowView::alignment() const
{
    Qt::Alignment verticalAlign = style()->verticalAlign() & Qt::AlignVertical_Mask;
    Qt::Alignment horizontalAlign = style()->horizontalAlign() & Qt::AlignHorizontal_Mask;

    return verticalAlign | horizontalAlign;
}

QPointF MSceneWindowView::offset() const
{
    return style()->offset();
}

M_REGISTER_VIEW_NEW(MSceneWindowView, MSceneWindow)
