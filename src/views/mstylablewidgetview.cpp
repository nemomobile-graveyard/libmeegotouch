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

#include "mstylablewidgetview_p.h"
#include "mstylablewidget.h"
#include "mtheme.h"
#include "mviewcreator.h"
#include "private/mwidgetview_p.h"

MStylableWidgetView::MStylableWidgetView(MStylableWidget *controller) :
    MWidgetView(controller)
{
    this->controller = controller;
}

MStylableWidgetView::~MStylableWidgetView()
{
}

MWidgetStyleContainer *MStylableWidgetView::createStyleContainer() const
{
    // returns Style Container stored in the controller side.
    return controller->createStyleContainer();
}

void MStylableWidgetView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawBackground(painter, option);
}

void MStylableWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawContents(painter, option);
}

void MStylableWidgetView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawForeground(painter, option);
}

void MStylableWidgetView::applyStyle()
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    MWidgetView::applyStyle();
    controller->applyStyle();
}
M_REGISTER_VIEW_NEW(MStylableWidgetView, MStylableWidget)

