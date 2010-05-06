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
 * testwidgetview.h
 *
 *  Created on: May 19, 2009
 *      Author: Janne Koivuranta
 */

#ifndef TESTWIDGETVIEW_H_
#define TESTWIDGETVIEW_H_

#include <mwidgetview.h>
#include <testwidgetmodel.h>
#include <testwidgetstyle.h>

class TestWidget;
class QGraphicsSceneResizeEvent;

class TestWidgetView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(TestWidgetModel, TestWidgetStyle)

public:
    TestWidgetView(TestWidget *controller);

    virtual ~TestWidgetView();
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif /* TESTWIDGETVIEW_H_ */
