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

#include "duiapplicationpageview.h"
#include "duiapplicationpageview_p.h"

#include "duiapplication.h"
#include "duiapplicationwindow.h"
#include "duiscalableimage.h"
#include "duiapplicationpage.h"
#include "duiscenemanager.h"
#include "duiviewcreator.h"

DuiApplicationPageViewPrivate::DuiApplicationPageViewPrivate()
{
}

DuiApplicationPageViewPrivate::~DuiApplicationPageViewPrivate()
{
}

DuiApplicationPageView::DuiApplicationPageView(DuiApplicationPage *controller) :
    DuiSceneWindowView(*new DuiApplicationPageViewPrivate, controller)
{
}

DuiApplicationPageView::~DuiApplicationPageView()
{
}
/*
QRectF DuiApplicationPageView::boundingRect() const
{

}
*/

DUI_REGISTER_VIEW_NEW(DuiApplicationPageView, DuiApplicationPage)
