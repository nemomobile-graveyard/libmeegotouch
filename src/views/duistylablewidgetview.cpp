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

#include "duistylablewidgetview_p.h"
#include "duistylablewidget.h"
#include "duitheme.h"
#include "duiviewcreator.h"
#include "private/duiwidgetview_p.h"

DuiStylableWidgetView::DuiStylableWidgetView(DuiStylableWidget *controller) :
    DuiWidgetView(controller)
{
    this->controller = controller;
}

DuiStylableWidgetView::~DuiStylableWidgetView()
{
}

DuiWidgetStyleContainer *DuiStylableWidgetView::createStyleContainer() const
{
    // returns Style Container stored in the controller side.
    return controller->createStyleContainer();
}

void DuiStylableWidgetView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawBackground(painter, option);
}

void DuiStylableWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawContents(painter, option);
}

void DuiStylableWidgetView::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->drawForeground(painter, option);
}

void DuiStylableWidgetView::applyStyle()
{
    //Rerouting the call to the controller to allow implementation of this
    //method in a simple widget class.
    controller->applyStyle();
}
DUI_REGISTER_VIEW_NEW(DuiStylableWidgetView, DuiStylableWidget)

