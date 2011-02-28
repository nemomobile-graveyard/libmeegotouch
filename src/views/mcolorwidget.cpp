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

#include "mcolorwidget.h"

MColorWidget::MColorWidget(QGraphicsItem *parent)
    : MStylableWidget(parent),
      m_color(Qt::color0)
{
}

void MColorWidget::setColor(const QColor &color)
{
    m_color = color;
    update();
}

QColor MColorWidget::color() const
{
    return m_color;
}

void MColorWidget::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (!m_color.isValid())
        return;

    QRectF rct(style()->marginLeft(),
        style()->marginTop(),
        size().width() - style()->marginLeft() - style()->marginRight(),
        size().height() - style()->marginTop() - style()->marginBottom());

    QPen pen(m_color);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setOpacity(style()->opacity());
    painter->setBrush(m_color);
    // hard-coded round values for the color bar to full-fill ALG requirements:
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRoundedRect(rct, style()->xRadius(), style()->yRadius());
}
