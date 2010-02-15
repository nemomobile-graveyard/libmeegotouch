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

#ifndef DUIPANNABLEVIEWPORT_STUB
#define DUIPANNABLEVIEWPORT_STUB

#include "duipannableviewport.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiPannableViewportStub : public StubBase
{
public:
    virtual void DuiPannableViewportConstructor(Qt::Orientations direction, DuiWidget *parent);
    virtual void DuiPannableViewportDestructor();
    virtual void updateStyle();
    virtual QGraphicsWidget *setWidget(QGraphicsWidget *widget);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual bool event(QEvent *event);
    virtual void updatePosition(QPointF const  &pos);
    DuiPannableViewportPrivate *d_ptr ;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
};

// 2. IMPLEMENT STUB
void DuiPannableViewportStub::DuiPannableViewportConstructor(Qt::Orientations direction, DuiWidget *parent)
{
    Q_UNUSED(direction);
    Q_UNUSED(parent);

}
void DuiPannableViewportStub::DuiPannableViewportDestructor()
{

}
void DuiPannableViewportStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QGraphicsWidget *DuiPannableViewportStub::setWidget(QGraphicsWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsWidget * >(widget));
    stubMethodEntered("setWidget", params);
    return stubReturnValue<QGraphicsWidget *>("setWidget");
}

void DuiPannableViewportStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF DuiPannableViewportStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath DuiPannableViewportStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

bool DuiPannableViewportStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}

void DuiPannableViewportStub::updatePosition(QPointF const  &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF const & >(pos));
    stubMethodEntered("updatePosition", params);
}

QSizeF DuiPannableViewportStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

// 3. CREATE A STUB INSTANCE
DuiPannableViewportStub gDefaultDuiPannableViewportStub;
DuiPannableViewportStub *gDuiPannableViewportStub = &gDefaultDuiPannableViewportStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiPannableViewport::DuiPannableViewport(Qt::Orientations direction, DuiWidget *parent)
{
    gDuiPannableViewportStub->DuiPannableViewportConstructor(direction, parent);
}

DuiPannableViewport::~DuiPannableViewport()
{
    gDuiPannableViewportStub->DuiPannableViewportDestructor();
}

void DuiPannableViewport::updateStyle()
{
    gDuiPannableViewportStub->updateStyle();
}

QGraphicsWidget *DuiPannableViewport::setWidget(QGraphicsWidget *widget)
{
    return gDuiPannableViewportStub->setWidget(widget);
}

void DuiPannableViewport::setGeometry(const QRectF &rect)
{
    gDuiPannableViewportStub->setGeometry(rect);
}

QRectF DuiPannableViewport::boundingRect() const
{
    return gDuiPannableViewportStub->boundingRect();
}

QPainterPath DuiPannableViewport::shape() const
{
    return gDuiPannableViewportStub->shape();
}

bool DuiPannableViewport::event(QEvent *event)
{
    return gDuiPannableViewportStub->event(event);
}

void DuiPannableViewport::updatePosition(QPointF const  &pos)
{
    gDuiPannableViewportStub->updatePosition(pos);
}

QSizeF DuiPannableViewport::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiPannableViewportStub->sizeHint(which, constraint);
}

#endif
