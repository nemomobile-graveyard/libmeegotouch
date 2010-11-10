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
#include "mgridpage.h"

#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>

GridImageWidget::GridImageWidget(QGraphicsItem *parent)
    : MImageWidget(parent),
    m_id(),
    opacityAnimation(0)
{
    opacityAnimation = new QPropertyAnimation(this, "opacity", this);
    opacityAnimation->setStartValue(0.0);
    opacityAnimation->setEndValue(1.0);
    opacityAnimation->setDuration(150);
}

void GridImageWidget::setId(const QString& id)
{
    showAnimated();
    m_id = id;
}

QString GridImageWidget::id()
{
    return m_id;
}

void GridImageWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mousePressEvent(event);
    event->accept();
}

void GridImageWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    MImageWidget::mouseReleaseEvent(event);
    emit clicked();
}

void GridImageWidget::showAnimated()
{
    opacityAnimation->start();
}
