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

#ifndef MBUTTONVIEW_STUB
#define MBUTTONVIEW_STUB

#include "mbuttonview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MButtonViewStub : public StubBase
{
public:
    virtual void MButtonViewConstructor(MButton *controller);
    virtual void MButtonViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void updateData(const int index);
    virtual void timelineValueChanged(qreal value);
    virtual void registerStyleAttributes(MStyleDescription &description);

    MButtonViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MButtonViewStub::MButtonViewConstructor(MButton *controller)
{
    Q_UNUSED(controller);

}
void MButtonViewStub::MButtonViewDestructor()
{

}
void MButtonViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

void MButtonViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF MButtonViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath MButtonViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

void MButtonViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void MButtonViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void MButtonViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("updateData", params);
}

void MButtonViewStub::timelineValueChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("timelineValueChanged", params);
}

void MButtonViewStub::registerStyleAttributes(MStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MStyleDescription &>(description));
    stubMethodEntered("registerStyleAttributes", params);
}



// 3. CREATE A STUB INSTANCE
MButtonViewStub gDefaultMButtonViewStub;
MButtonViewStub *gMButtonViewStub = &gDefaultMButtonViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MButtonView::MButtonView(MButton *controller)
{
    gMButtonViewStub->MButtonViewConstructor(controller);
}

MButtonView::~MButtonView()
{
    gMButtonViewStub->MButtonViewDestructor();
}

void MButtonView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMButtonViewStub->paint(painter, option, widget);
}

void MButtonView::setGeometry(const QRectF &rect)
{
    gMButtonViewStub->setGeometry(rect);
}

QRectF MButtonView::boundingRect() const
{
    return gMButtonViewStub->boundingRect();
}

QPainterPath MButtonView::shape() const
{
    return gMButtonViewStub->shape();
}

void MButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gMButtonViewStub->mousePressEvent(event);
}

void MButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gMButtonViewStub->mouseReleaseEvent(event);
}

void MButtonView::updateData(const int index)
{
    gMButtonViewStub->updateData(index);
}

void MButtonView::timelineValueChanged(qreal value)
{
    gMButtonViewStub->timelineValueChanged(value);
}


void MButtonView::resizeEvent(QGraphicsSceneResizeEvent *)
{
}

void MButtonView::scalePNGMode()
{
}

void MButtonView::updateSize()
{
}

void MButtonView::resize(qreal, qreal)
{
}

void MButtonView::registerStyleAttributes(MStyleDescription &description)
{
    Q_UNUSED(description);
}

void MButtonView::styleUpdated()
{
}

#endif
