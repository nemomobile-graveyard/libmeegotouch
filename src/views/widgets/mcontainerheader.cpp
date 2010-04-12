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

#include <QGraphicsWidget>
#include "mcontainerheader_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MContainerHeader)

/*
 * View
 */

M_REGISTER_VIEW(MContainerHeaderView, MContainerHeader)

MContainerHeaderView::MContainerHeaderView(MContainerHeader *controller)
    : MWidgetView(controller)
{
    connect(this, SIGNAL(pressed()), controller, SIGNAL(pressed()));
    connect(this, SIGNAL(released()), controller, SIGNAL(released()));
    connect(this, SIGNAL(canceled()), controller, SIGNAL(canceled()));
    connect(this, SIGNAL(moved()), controller, SIGNAL(moved()));
}

void MContainerHeaderView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModePressed();
    emit pressed();
    update();
}

void MContainerHeaderView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit released();
    update();
}

void MContainerHeaderView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit moved();
    update();
}

void MContainerHeaderView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    style().setModeDefault();
    emit canceled();
    update();
}


/*
 * Controller
 */

MContainerHeader::MContainerHeader()
    : MWidgetController(new MWidgetModel, NULL)
{
    setView(new MContainerHeaderView(this));
}
