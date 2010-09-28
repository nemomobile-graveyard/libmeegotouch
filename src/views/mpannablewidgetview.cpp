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

#include "mpannablewidgetview.h"
#include "mpannablewidgetview_p.h"

#include <QPainter>

#include "mtheme.h"
#include "mpannablewidget.h"
#include "mviewcreator.h"

MPannableWidgetViewPrivate::MPannableWidgetViewPrivate() :
    q_ptr(0), controller(0)
{
}

MPannableWidgetViewPrivate::~MPannableWidgetViewPrivate()
{
}

void MPannableWidgetViewPrivate::_q_applyStyleToPhysics()
{
    Q_Q(MPannableWidgetView);

    controller->model()->setPanThreshold(q->style()->panThreshold());
    controller->model()->setPanClickThreshold(q->style()->panClickThreshold());

    controller->physics()->setPointerSpringK(q->style()->pointerSpringK());
    controller->physics()->setFriction(q->style()->frictionC());
    controller->physics()->setSlidingFriction(q->style()->slidingFrictionC());
    controller->physics()->setBorderSpringK(q->style()->borderSpringK());
    controller->physics()->setBorderFriction(q->style()->borderFrictionC());
    controller->physics()->setPanDirection(controller->panDirection());
    controller->physics()->setMaximumVelocity(q->style()->maximumVelocity());
}

MPannableWidgetView::MPannableWidgetView(MPannableWidget *controller) :
    MWidgetView(controller),
    d_ptr(new MPannableWidgetViewPrivate)
{
    Q_D(MPannableWidgetView);
    d->q_ptr = this;
    d->controller = controller;
    connect(d->controller, SIGNAL(physicsChanged()), this, SLOT(_q_applyStyleToPhysics()));
}

MPannableWidgetView::~MPannableWidgetView()
{
    delete d_ptr;
}

void MPannableWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
}

void MPannableWidgetView::applyStyle()
{
    Q_D(MPannableWidgetView);

    d->_q_applyStyleToPhysics();
    MWidgetView::applyStyle();
}

M_REGISTER_VIEW_NEW(MPannableWidgetView, MPannableWidget)

#include "moc_mpannablewidgetview.cpp"
