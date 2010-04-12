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

#include "mmodalscenewindowview.h"
#include "mmodalscenewindowview_p.h"

#include "mmodalscenewindow.h"
#include "mmodalscenewindow_p.h"
#include "mviewcreator.h"

MModalSceneWindowViewPrivate::MModalSceneWindowViewPrivate()
  : q_ptr(0),
    controller(0)
{
}

MModalSceneWindowViewPrivate::~MModalSceneWindowViewPrivate()
{
}


MModalSceneWindowView::MModalSceneWindowView(MModalSceneWindow *controller) :
    MSceneWindowView(controller),
    d_ptr(new MModalSceneWindowViewPrivate)
{
    Q_D(MModalSceneWindowView);
    d->q_ptr = this;
    d->controller = controller;
}

MModalSceneWindowView::MModalSceneWindowView(MModalSceneWindowViewPrivate &dd, MModalSceneWindow *controller)
    : MSceneWindowView(controller),
      d_ptr(&dd)
{
    Q_D(MModalSceneWindowView);
    d->q_ptr = this;
    d->controller = controller;
}


MModalSceneWindowView::~MModalSceneWindowView()
{
    delete d_ptr;
}

void MModalSceneWindowView::applyStyle()
{
    MSceneWindowView::applyStyle();
}

void MModalSceneWindowView::setupModel()
{
    MSceneWindowView::setupModel();
}

void MModalSceneWindowView::updateData(const QList<const char *> &modifications)
{
    MSceneWindowView::updateData(modifications);
}

M_REGISTER_VIEW_NEW(MModalSceneWindowView, MModalSceneWindow)
