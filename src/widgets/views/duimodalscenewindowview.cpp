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

#include "duimodalscenewindowview.h"
#include "duimodalscenewindowview_p.h"

#include "duimodalscenewindow.h"
#include "duimodalscenewindow_p.h"
#include "duiviewcreator.h"

DuiModalSceneWindowViewPrivate::DuiModalSceneWindowViewPrivate()
{
}

DuiModalSceneWindowViewPrivate::~DuiModalSceneWindowViewPrivate()
{
}


DuiModalSceneWindowView::DuiModalSceneWindowView(DuiModalSceneWindow *controller) :
    DuiSceneWindowView(*new DuiModalSceneWindowViewPrivate, controller)
{
    Q_D(DuiModalSceneWindowView);
    d->controller = controller;
}

DuiModalSceneWindowView::DuiModalSceneWindowView(DuiModalSceneWindowViewPrivate &dd, DuiModalSceneWindow *controller)
    : DuiSceneWindowView(dd, controller)
{
    Q_D(DuiModalSceneWindowView);
    d->controller = controller;
}


DuiModalSceneWindowView::~DuiModalSceneWindowView()
{
}

void DuiModalSceneWindowView::applyStyle()
{
    DuiSceneWindowView::applyStyle();
}

void DuiModalSceneWindowView::setupModel()
{
    DuiSceneWindowView::setupModel();
}

void DuiModalSceneWindowView::updateData(const QList<const char *> &modifications)
{
    DuiSceneWindowView::updateData(modifications);
}

DUI_REGISTER_VIEW_NEW(DuiModalSceneWindowView, DuiModalSceneWindow)
