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

#include <QGraphicsWidget>
#include "duicontainerheader_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiContainerHeader)

/*
 * View
 */

DUI_REGISTER_VIEW(DuiContainerHeaderView, DuiContainerHeader)

DuiContainerHeaderView::DuiContainerHeaderView(DuiContainerHeader *controller)
    : DuiWidgetView(controller)
{
    connect(this, SIGNAL(pressed()), controller, SIGNAL(pressed()));
    connect(this, SIGNAL(released()), controller, SIGNAL(released()));
    connect(this, SIGNAL(canceled()), controller, SIGNAL(canceled()));
    connect(this, SIGNAL(moved()), controller, SIGNAL(moved()));
}

void DuiContainerHeaderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModePressed();
    emit pressed();
    update();
}

void DuiContainerHeaderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit released();
    update();
}

void DuiContainerHeaderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit moved();
    update();
}

void DuiContainerHeaderView::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit canceled();
    update();
}


/*
 * Controller
 */

DuiContainerHeader::DuiContainerHeader()
    : DuiWidgetController(new DuiWidgetModel, NULL)
{
    setView(new DuiContainerHeaderView(this));
}
