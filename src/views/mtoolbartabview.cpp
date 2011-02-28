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

#include "mtoolbar.h"
#include "mbuttongroup.h"
#include "mtoolbartabview.h"
#include "mtoolbartabview_p.h"

MToolbarTabViewPrivate::MToolbarTabViewPrivate(MToolBar *controller) : MToolBarViewPrivate(controller)
{
    buttonGroup = new MButtonGroup(controller);
    buttonGroup->setExclusive(true);
}

MToolbarTabViewPrivate::~MToolbarTabViewPrivate()
{
    //buttonGroup deleted by MToolBarView
}


MToolbarTabView::MToolbarTabView(MToolBar *controller) :
    MToolBarView(*(new MToolbarTabViewPrivate(controller)), controller)
{
}

MToolbarTabView::MToolbarTabView(MToolbarTabViewPrivate &dd, MToolBar *controller) :
    MToolBarView(dd, controller)
{
}

MToolbarTabView::~MToolbarTabView()
{
}

// bind view and controller together
M_REGISTER_VIEW_NEW(MToolbarTabView, MToolBar)

#include "moc_mtoolbartabview.cpp"
