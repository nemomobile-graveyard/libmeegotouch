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

#ifndef DUIBUTTONVIEW_STUB
#define DUIBUTTONVIEW_STUB

#include "duibuttonview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiButtonViewStub : public StubBase
{
public:
    virtual void DuiButtonViewConstructor(DuiButton *controller);
    virtual void DuiButtonViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void updateData(const int index);
    virtual void timelineValueChanged(qreal value);
    virtual void registerStyleAttributes(DuiStyleDescription &description);

    DuiButtonViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiButtonViewStub::DuiButtonViewConstructor(DuiButton *controller)
{
    Q_UNUSED(controller);

}
void DuiButtonViewStub::DuiButtonViewDestructor()
{

}
void DuiButtonViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

void DuiButtonViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF DuiButtonViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath DuiButtonViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

void DuiButtonViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiButtonViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiButtonViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("updateData", params);
}

void DuiButtonViewStub::timelineValueChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("timelineValueChanged", params);
}

void DuiButtonViewStub::registerStyleAttributes(DuiStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiStyleDescription &>(description));
    stubMethodEntered("registerStyleAttributes", params);
}



// 3. CREATE A STUB INSTANCE
DuiButtonViewStub gDefaultDuiButtonViewStub;
DuiButtonViewStub *gDuiButtonViewStub = &gDefaultDuiButtonViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiButtonView::DuiButtonView(DuiButton *controller)
{
    gDuiButtonViewStub->DuiButtonViewConstructor(controller);
}

DuiButtonView::~DuiButtonView()
{
    gDuiButtonViewStub->DuiButtonViewDestructor();
}

void DuiButtonView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiButtonViewStub->paint(painter, option, widget);
}

void DuiButtonView::setGeometry(const QRectF &rect)
{
    gDuiButtonViewStub->setGeometry(rect);
}

QRectF DuiButtonView::boundingRect() const
{
    return gDuiButtonViewStub->boundingRect();
}

QPainterPath DuiButtonView::shape() const
{
    return gDuiButtonViewStub->shape();
}

void DuiButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiButtonViewStub->mousePressEvent(event);
}

void DuiButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiButtonViewStub->mouseReleaseEvent(event);
}

void DuiButtonView::updateData(const int index)
{
    gDuiButtonViewStub->updateData(index);
}

void DuiButtonView::timelineValueChanged(qreal value)
{
    gDuiButtonViewStub->timelineValueChanged(value);
}


void DuiButtonView::resizeEvent(QGraphicsSceneResizeEvent *)
{
}

void DuiButtonView::scalePNGMode()
{
}

void DuiButtonView::updateSize()
{
}

void DuiButtonView::resize(qreal, qreal)
{
}

void DuiButtonView::registerStyleAttributes(DuiStyleDescription &description)
{
    Q_UNUSED(description);
}

void DuiButtonView::styleUpdated()
{
}

#endif
