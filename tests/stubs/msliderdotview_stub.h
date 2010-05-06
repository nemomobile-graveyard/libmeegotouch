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

#ifndef MSLIDERDOTVIEW_STUB
#define MSLIDERDOTVIEW_STUB

#include "msliderdotview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSliderDotViewStub : public StubBase
{
public:
    virtual void MSliderDotViewConstructor(MSlider *controller);
    virtual void MSliderDotViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    virtual void updateStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    MSliderDotViewPrivate *d_ptr ;
    virtual void setPressed(bool pressed);
    virtual void modelUpdated(const int index);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
};

// 2. IMPLEMENT STUB
void MSliderDotViewStub::MSliderDotViewConstructor(MSlider *controller)
{
    Q_UNUSED(controller);

}
void MSliderDotViewStub::MSliderDotViewDestructor()
{

}
void MSliderDotViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF MSliderDotViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void MSliderDotViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void MSliderDotViewStub::updateStyle(void)
{
    stubMethodEntered("updateStyle");
}

void MSliderDotViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void MSliderDotViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void MSliderDotViewStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseMoveEvent", params);
}

void MSliderDotViewStub::setPressed(bool pressed)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(pressed));
    stubMethodEntered("setPressed", params);
}

void MSliderDotViewStub::modelUpdated(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("modelUpdated", params);
}

QSizeF MSliderDotViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}


// 3. CREATE A STUB INSTANCE
MSliderDotViewStub gDefaultMSliderDotViewStub;
MSliderDotViewStub *gMSliderDotViewStub = &gDefaultMSliderDotViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSliderDotView::MSliderDotView(MSlider *controller)
{
    gMSliderDotViewStub->MSliderDotViewConstructor(controller);
}

MSliderDotView::~MSliderDotView()
{
    gMSliderDotViewStub->MSliderDotViewDestructor();
}

void MSliderDotView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMSliderDotViewStub->paint(painter, option, widget);
}

QRectF MSliderDotView::boundingRect() const
{
    return gMSliderDotViewStub->boundingRect();
}

void MSliderDotView::setGeometry(const QRectF &rect)
{
    gMSliderDotViewStub->setGeometry(rect);
}

void MSliderDotView::updateStyle()
{
    gMSliderDotViewStub->updateStyle();
}

void MSliderDotView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gMSliderDotViewStub->mousePressEvent(event);
}

void MSliderDotView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gMSliderDotViewStub->mouseReleaseEvent(event);
}

void MSliderDotView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gMSliderDotViewStub->mouseMoveEvent(event);
}

void MSliderDotView::setPressed(bool pressed)
{
    gMSliderDotViewStub->setPressed(pressed);
}

void MSliderDotView::modelUpdated(const int index)
{
    gMSliderDotViewStub->modelUpdated(index);
}

QSizeF MSliderDotView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMSliderDotViewStub->sizeHint(which, constraint);
}
#endif
