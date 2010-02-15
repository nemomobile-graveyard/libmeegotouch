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

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QWidget>

#include "duitheme.h"
#include "duiwidgetview.h"
#include "duipannablewidget.h"
#include "duiviewcreator.h"

class DuiPannableWidgetViewPrivate
{
public:

    DuiPannableWidgetViewPrivate();
    virtual ~DuiPannableWidgetViewPrivate();

    DuiPannableWidget *controller;
};

DuiPannableWidgetViewPrivate::DuiPannableWidgetViewPrivate() :
    controller(0)
{
}

DuiPannableWidgetViewPrivate::~DuiPannableWidgetViewPrivate()
{
}

DuiPannableWidgetView::DuiPannableWidgetView(DuiPannableWidget *controller) :
    DuiWidgetView(controller),
    d_ptr(new DuiPannableWidgetViewPrivate)
{
    Q_D(DuiPannableWidgetView);
    d->controller = controller;
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
    // update panning parameters to pannable widget
    model()->setPanThreshold(style()->panThreshold());
    model()->setPanClickThreshold(style()->panClickThreshold());

    //FIXME Set physics parameters through model.
    // update physics parameters to pannable widget
    d->controller->physics()->setPointerSpringK(style()->pointerSpringK());
    d->controller->physics()->setFriction(style()->frictionC());
    d->controller->physics()->setSlidingFriction(style()->slidingFrictionC());
    d->controller->physics()->setBorderSpringK(style()->borderSpringK());
    d->controller->physics()->setBorderFriction(style()->borderFrictionC());

    DuiWidgetView::applyStyle();
}

DUI_REGISTER_VIEW_NEW(DuiPannableWidgetView, DuiPannableWidget)
