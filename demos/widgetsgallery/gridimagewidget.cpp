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

#include "gridimagewidget.h"

#include <QGraphicsSceneMouseEvent>

GridImageWidget::GridImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent)
{
}

void GridImageWidget::setId(const QString& id)
{
    m_id = id;
}

QString GridImageWidget::id()
{
    return m_id;
}

void GridImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event); emit rate(MediaType::OneStar, m_id);
    event->accept();
}

void GridImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}
