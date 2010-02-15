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

#ifndef DUISLIDERDOTVIEW_STUB
#define DUISLIDERDOTVIEW_STUB

#include "duisliderdotview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSliderDotViewStub : public StubBase
{
public:
    virtual void DuiSliderDotViewConstructor(DuiSlider *controller);
    virtual void DuiSliderDotViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    virtual void updateStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    DuiSliderDotViewPrivate *d_ptr ;
    virtual void setPressed(bool pressed);
    virtual void modelUpdated(const int index);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
};

// 2. IMPLEMENT STUB
void DuiSliderDotViewStub::DuiSliderDotViewConstructor(DuiSlider *controller)
{
    Q_UNUSED(controller);

}
void DuiSliderDotViewStub::DuiSliderDotViewDestructor()
{

}
void DuiSliderDotViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF DuiSliderDotViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void DuiSliderDotViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void DuiSliderDotViewStub::updateStyle(void)
{
    stubMethodEntered("updateStyle");
}

void DuiSliderDotViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiSliderDotViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiSliderDotViewStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseMoveEvent", params);
}

void DuiSliderDotViewStub::setPressed(bool pressed)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(pressed));
    stubMethodEntered("setPressed", params);
}

void DuiSliderDotViewStub::modelUpdated(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("modelUpdated", params);
}

QSizeF DuiSliderDotViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}


// 3. CREATE A STUB INSTANCE
DuiSliderDotViewStub gDefaultDuiSliderDotViewStub;
DuiSliderDotViewStub *gDuiSliderDotViewStub = &gDefaultDuiSliderDotViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSliderDotView::DuiSliderDotView(DuiSlider *controller)
{
    gDuiSliderDotViewStub->DuiSliderDotViewConstructor(controller);
}

DuiSliderDotView::~DuiSliderDotView()
{
    gDuiSliderDotViewStub->DuiSliderDotViewDestructor();
}

void DuiSliderDotView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiSliderDotViewStub->paint(painter, option, widget);
}

QRectF DuiSliderDotView::boundingRect() const
{
    return gDuiSliderDotViewStub->boundingRect();
}

void DuiSliderDotView::setGeometry(const QRectF &rect)
{
    gDuiSliderDotViewStub->setGeometry(rect);
}

void DuiSliderDotView::updateStyle()
{
    gDuiSliderDotViewStub->updateStyle();
}

void DuiSliderDotView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiSliderDotViewStub->mousePressEvent(event);
}

void DuiSliderDotView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiSliderDotViewStub->mouseReleaseEvent(event);
}

void DuiSliderDotView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiSliderDotViewStub->mouseMoveEvent(event);
}

void DuiSliderDotView::setPressed(bool pressed)
{
    gDuiSliderDotViewStub->setPressed(pressed);
}

void DuiSliderDotView::modelUpdated(const int index)
{
    gDuiSliderDotViewStub->modelUpdated(index);
}

QSizeF DuiSliderDotView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiSliderDotViewStub->sizeHint(which, constraint);
}
#endif
