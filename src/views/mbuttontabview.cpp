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

#include "mbuttontabview.h"
#include "mbuttontabview_p.h"
#include "mviewconstants.h"

#include <QGraphicsSceneMouseEvent>

MButtonTabView::MButtonTabView(MButton *controller) :
    MButtonView(* new MButtonTabViewPrivate, controller)
{
}

MButtonTabView::MButtonTabView(MButtonTabViewPrivate &dd, MButton *controller) :
    MButtonView(dd, controller)
{
}

MButtonTabView::~MButtonTabView()
{
}

void MButtonTabView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    if (model()->checkable() && !model()->checked()) {
        style()->pressFeedback().play();

        // Activate it (i.e. "click") straight away.
        model()->click();
    }
}

void MButtonTabView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MButtonTabView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MButtonTabView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
}

M_REGISTER_VIEW_NEW(MButtonTabView, MButton)
