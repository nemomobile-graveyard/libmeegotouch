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

#include "duipannablewidgetview.h"
#include "duipannablewidgetview_p.h"

#include <QPainter>

#include "duitheme.h"
#include "duipannablewidget.h"
#include "duiviewcreator.h"

DuiPannableWidgetViewPrivate::DuiPannableWidgetViewPrivate() :
    q_ptr(0), controller(0)
{
}

DuiPannableWidgetViewPrivate::~DuiPannableWidgetViewPrivate()
{
}

void DuiPannableWidgetViewPrivate::_q_applyStyleToPhysics()
{
    Q_Q(DuiPannableWidgetView);

    controller->model()->setPanThreshold(q->style()->panThreshold());
    controller->model()->setPanClickThreshold(q->style()->panClickThreshold());

    controller->physics()->setPointerSpringK(q->style()->pointerSpringK());
    controller->physics()->setFriction(q->style()->frictionC());
    controller->physics()->setSlidingFriction(q->style()->slidingFrictionC());
    controller->physics()->setBorderSpringK(q->style()->borderSpringK());
    controller->physics()->setBorderFriction(q->style()->borderFrictionC());
    controller->physics()->setPanDirection(controller->panDirection());
}

DuiPannableWidgetView::DuiPannableWidgetView(DuiPannableWidget *controller) :
    DuiWidgetView(controller),
    d_ptr(new DuiPannableWidgetViewPrivate)
{
    Q_D(DuiPannableWidgetView);
    d->q_ptr = this;
    d->controller = controller;
    connect(d->controller, SIGNAL(physicsChanged()), this, SLOT(_q_applyStyleToPhysics()));
}

DuiPannableWidgetView::~DuiPannableWidgetView()
{
    delete d_ptr;
}

void DuiPannableWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void DuiPannableWidgetView::applyStyle()
{
    Q_D(DuiPannableWidgetView);

    d->_q_applyStyleToPhysics();
    DuiWidgetView::applyStyle();
}

DUI_REGISTER_VIEW_NEW(DuiPannableWidgetView, DuiPannableWidget)

#include "moc_duipannablewidgetview.cpp"
