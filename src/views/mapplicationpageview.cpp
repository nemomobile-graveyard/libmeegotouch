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

#include "mapplicationpageview.h"
#include "mapplicationpageview_p.h"

#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mscalableimage.h"
#include "mapplicationpage.h"
#include "mscenemanager.h"
#include "mviewcreator.h"

MApplicationPageViewPrivate::MApplicationPageViewPrivate()
    : q_ptr(0)
{
}

MApplicationPageViewPrivate::~MApplicationPageViewPrivate()
{
}

MApplicationPageView::MApplicationPageView(MApplicationPage *controller) :
    MSceneWindowView(controller),
    d_ptr(new MApplicationPageViewPrivate)
{
    Q_D(MApplicationPageView);
    d->q_ptr = this;
}

MApplicationPageView::~MApplicationPageView()
{
    delete d_ptr;
}
/*
QRectF MApplicationPageView::boundingRect() const
{

}
*/

M_REGISTER_VIEW_NEW(MApplicationPageView, MApplicationPage)
