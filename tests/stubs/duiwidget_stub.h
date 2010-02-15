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

#ifndef DUIWIDGET_STUB
#define DUIWIDGET_STUB

#include "duiwidget.h"
#include <stubbase.h>


class DuiWidgetStub : public StubBase
{
public:
    virtual void DuiWidgetConstructor(QGraphicsItem *parent);
    virtual void DuiWidgetDestructor();
    virtual QPainterPath shape() const;
    virtual void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, DuiOrientationChangeParameters *parameters);
    virtual void finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, DuiOrientationChangeParameters *parameters);
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);
    virtual bool event(QEvent *event);
};

void DuiWidgetStub::DuiWidgetConstructor(QGraphicsItem *parent)
{
    Q_UNUSED(parent);
}

void DuiWidgetStub::DuiWidgetDestructor()
{
}

QPainterPath DuiWidgetStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

void DuiWidgetStub::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paintWindowFrame", params);
}

void DuiWidgetStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

void DuiWidgetStub::executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, DuiOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<qreal >(offset));
    params.append(new Parameter<DuiOrientationChangeParameters * >(parameters));
    stubMethodEntered("executeOrientationAnimationPhase", params);
}

void DuiWidgetStub::finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, DuiOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWindow::OrientationAnimationPhase >(phase));
    params.append(new Parameter<DuiOrientationChangeParameters * >(parameters));
    stubMethodEntered("finalizeOrientationAnimationPhase", params);
}

QVariant DuiWidgetStub::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem::GraphicsItemChange >(change));
    params.append(new Parameter<const QVariant & >(value));
    stubMethodEntered("itemChange", params);
    return stubReturnValue<QVariant>("itemChange");
}

bool DuiWidgetStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}



// 3. CREATE A STUB INSTANCE
DuiWidgetStub gDefaultDuiWidgetStub;
DuiWidgetStub *gDuiWidgetStub = &gDefaultDuiWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB  FIXME
DuiWidget::DuiWidget(QGraphicsItem *parent) : d_ptr(0)
{
    gDuiWidgetStub->DuiWidgetConstructor(parent);
}

DuiWidget::~DuiWidget()
{
    gDuiWidgetStub->DuiWidgetDestructor();
}

QPainterPath DuiWidget::shape() const
{
    return gDuiWidgetStub->shape();
}

void DuiWidget::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiWidgetStub->paintWindowFrame(painter, option, widget);
}

void DuiWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiWidgetStub->paint(painter, option, widget);
}

void DuiWidget::executeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, qreal offset, DuiOrientationChangeParameters *parameters)
{
    gDuiWidgetStub->executeOrientationAnimationPhase(phase, offset, parameters);
}

void DuiWidget::finalizeOrientationAnimationPhase(DuiWindow::OrientationAnimationPhase phase, DuiOrientationChangeParameters *parameters)
{
    gDuiWidgetStub->finalizeOrientationAnimationPhase(phase, parameters);
}

QVariant DuiWidget::itemChange(GraphicsItemChange change, const QVariant &value)
{
    return gDuiWidgetStub->itemChange(change, value);
}

bool DuiWidget::event(QEvent *event)
{
    return gDuiWidgetStub->event(event);
}


#endif
