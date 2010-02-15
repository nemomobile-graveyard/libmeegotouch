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

#include <QPainter>
#include "duiviewcreator.h"
#include "duiapplethandle.h"
#include "duiapplethandleswview.h"
#include "duiapplethandleswview_p.h"


DuiAppletHandleSWViewPrivate::DuiAppletHandleSWViewPrivate(DuiAppletHandle *appletHandle) :
    DuiAppletHandleViewPrivate(appletHandle)
{
}

DuiAppletHandleSWViewPrivate::~DuiAppletHandleSWViewPrivate()
{
}

void DuiAppletHandleSWViewPrivate::drawAppletPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool) const
{
    // Draw the applet pixmap scaled so that it fits the screen
    painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
}

DuiAppletHandleSWView::DuiAppletHandleSWView(DuiAppletHandle *appletHandle) :
    DuiAppletHandleView(* new DuiAppletHandleSWViewPrivate(appletHandle), appletHandle)
{
}

DuiAppletHandleSWView::~DuiAppletHandleSWView()
{
}

DUI_REGISTER_VIEW_NEW(DuiAppletHandleSWView, DuiAppletHandle)
