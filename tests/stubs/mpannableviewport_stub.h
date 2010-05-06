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

#ifndef MPANNABLEVIEWPORT_STUB
#define MPANNABLEVIEWPORT_STUB

#include "mpannableviewport.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MPannableViewportStub : public StubBase
{
public:
    virtual void MPannableViewportConstructor(Qt::Orientations direction, MWidget *parent);
    virtual void MPannableViewportDestructor();
    virtual void updateStyle();
    virtual QGraphicsWidget *setWidget(QGraphicsWidget *widget);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual bool event(QEvent *event);
    virtual void updatePosition(QPointF const  &pos);
    MPannableViewportPrivate *d_ptr ;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
};

// 2. IMPLEMENT STUB
void MPannableViewportStub::MPannableViewportConstructor(Qt::Orientations direction, MWidget *parent)
{
    Q_UNUSED(direction);
    Q_UNUSED(parent);

}
void MPannableViewportStub::MPannableViewportDestructor()
{

}
void MPannableViewportStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QGraphicsWidget *MPannableViewportStub::setWidget(QGraphicsWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsWidget * >(widget));
    stubMethodEntered("setWidget", params);
    return stubReturnValue<QGraphicsWidget *>("setWidget");
}

void MPannableViewportStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF MPannableViewportStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath MPannableViewportStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

bool MPannableViewportStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}

void MPannableViewportStub::updatePosition(QPointF const  &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF const & >(pos));
    stubMethodEntered("updatePosition", params);
}

QSizeF MPannableViewportStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

// 3. CREATE A STUB INSTANCE
MPannableViewportStub gDefaultMPannableViewportStub;
MPannableViewportStub *gMPannableViewportStub = &gDefaultMPannableViewportStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MPannableViewport::MPannableViewport(Qt::Orientations direction, MWidget *parent)
{
    gMPannableViewportStub->MPannableViewportConstructor(direction, parent);
}

MPannableViewport::~MPannableViewport()
{
    gMPannableViewportStub->MPannableViewportDestructor();
}

void MPannableViewport::updateStyle()
{
    gMPannableViewportStub->updateStyle();
}

QGraphicsWidget *MPannableViewport::setWidget(QGraphicsWidget *widget)
{
    return gMPannableViewportStub->setWidget(widget);
}

void MPannableViewport::setGeometry(const QRectF &rect)
{
    gMPannableViewportStub->setGeometry(rect);
}

QRectF MPannableViewport::boundingRect() const
{
    return gMPannableViewportStub->boundingRect();
}

QPainterPath MPannableViewport::shape() const
{
    return gMPannableViewportStub->shape();
}

bool MPannableViewport::event(QEvent *event)
{
    return gMPannableViewportStub->event(event);
}

void MPannableViewport::updatePosition(QPointF const  &pos)
{
    gMPannableViewportStub->updatePosition(pos);
}

QSizeF MPannableViewport::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMPannableViewportStub->sizeHint(which, constraint);
}

#endif
