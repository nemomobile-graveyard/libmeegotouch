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

#include "gridvideowidget.h"

#include "mgridpage.h"

#include <QGraphicsSceneMouseEvent>

#ifdef HAVE_GSTREAMER

GridVideoWidget::GridVideoWidget(QGraphicsItem *parent)
    : MVideoWidget(parent),
      m_id()
{
}

void GridVideoWidget::setId(const QString& id)
{
    m_id = id;
}

QString GridVideoWidget::id()
{
    return m_id;
}

void GridVideoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mousePressEvent(event);
    event->accept();
}

void GridVideoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MVideoWidget::mouseReleaseEvent(event);
    emit clicked();
}

void GridVideoWidget::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    MVideoWidget::contextMenuEvent(event);
    emit longPressed();
}

void GridVideoWidget::setPage(MGridPage* page)
{
    if( receivers( SIGNAL(clicked()) ) > 0 )
        return;
    connect( this, SIGNAL(clicked()), page, SLOT(itemClicked()) );
}
#endif
