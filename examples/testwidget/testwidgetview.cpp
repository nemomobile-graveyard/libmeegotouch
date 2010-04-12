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

/*
 * testwidgetview.cpp
 *
 *  Created on: May 19, 2009
 *      Author: jak
 */


#include "testwidgetview.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include "testwidget.h"
#include "mtheme.h"
#include "mviewcreator.h"


TestWidgetView::TestWidgetView(TestWidget *controller) :
    MWidgetView(controller)
{
}

TestWidgetView::~TestWidgetView()
{
}

void TestWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MWidgetView::resizeEvent(event);
}


void TestWidgetView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    // Create string that contains color from current style as rgb values.
    QString text("color:    ");

    QString num;
    num.setNum(style()->color().red(), 10);
    text.append(num);
    text.append(QString("    "));

    num.setNum(style()->color().green(), 10);
    text.append(num);
    text.append(QString("    "));

    num.setNum(style()->color().blue(), 10);
    text.append(num);

    // Draw rectangle with size from geometry and text that display color.
    QRectF box(QPointF(0, 0), size());
    painter->fillRect(box, style()->color());
    painter->drawText(box, text);
}



void TestWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    // Get style index from model, increase it by one and check boundaries, then reset model style index.
    int styleIndex = model()->styleIndex();
    styleIndex ++;
    if (styleIndex > 5)
        styleIndex = 0;
    model()->setStyleIndex(styleIndex);

    // Set style mode by style index.
    switch (styleIndex) {
    case 1:
        style().setModeGreen();
        break;
    case 2:
        style().setModeBlue();
        break;
    case 3:
        style().setModeYellow();
        break;
    case 4:
        style().setModeCyan();
        break;
    case 5:
        style().setModePurple();
        break;
    default:
        style().setModeDefault();
        break;
    }
    update();
}

M_REGISTER_VIEW(TestWidgetView, TestWidget)
