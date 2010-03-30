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

#ifndef DUIWIDGETVIEW_STUB
#define DUIWIDGETVIEW_STUB

#include "duiwidgetview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiWidgetViewStub : public StubBase
{
public:
    virtual void duiWidgetViewConstructor();
    virtual void duiWidgetViewDestructor();
    virtual QPainterPath shape() const;
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture* gesture);
    virtual void pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture);
    virtual void tapGestureEvent(QGestureEvent *event, QTapGesture* gesture);
    virtual void swipeGestureEvent(QGestureEvent *event, QSwipeGesture* gesture);
};

// 2. IMPLEMENT STUB
void DuiWidgetViewStub::duiWidgetViewConstructor()
{
}

void DuiWidgetViewStub::duiWidgetViewDestructor()
{
}

QPainterPath DuiWidgetViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

void DuiWidgetViewStub::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneResizeEvent * >(event));
    stubMethodEntered("resizeEvent", params);
}

void DuiWidgetViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiWidgetViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiWidgetViewStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseMoveEvent", params);
}

QSizeF DuiWidgetViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

void DuiWidgetViewStub::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapAndHoldGesture * >(gesture));
  stubMethodEntered("tapAndHoldGesture",params);
}

void DuiWidgetViewStub::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPanGesture * >(gesture));
  stubMethodEntered("panGesture",params);
}

void DuiWidgetViewStub::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPinchGesture * >(gesture));
  stubMethodEntered("pinchGesture",params);
}

void DuiWidgetViewStub::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapGesture * >(gesture));
  stubMethodEntered("tapGesture",params);
}

void DuiWidgetViewStub::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QSwipeGesture * >(gesture));
  stubMethodEntered("swipeGesture",params);
}

// 3. CREATE A STUB INSTANCE
DuiWidgetViewStub gDefaultDuiWidgetViewStub;
DuiWidgetViewStub *gDuiWidgetViewStub = &gDefaultDuiWidgetViewStub;

/*
// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiWidgetView::DuiWidgetView(DuiWidgetController* controller) :
DuiStylableBase(this, controller), d_ptr(0)
{
    Q_UNUSED(controller);
    gDuiWidgetViewStub->duiWidgetViewConstructor();
}
*/

DuiWidgetView::~DuiWidgetView()
{
    gDuiWidgetViewStub->duiWidgetViewDestructor();
}

QPainterPath DuiWidgetView::shape() const
{
    return gDuiWidgetViewStub->shape();
}

void DuiWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    gDuiWidgetViewStub->resizeEvent(event);
}

void DuiWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetViewStub->mousePressEvent(event);
}

void DuiWidgetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetViewStub->mouseReleaseEvent(event);
}

void DuiWidgetView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetViewStub->mouseMoveEvent(event);
}

QSizeF DuiWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiWidgetViewStub->sizeHint(which, constraint);
}

void DuiWidgetView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  gDuiWidgetViewStub->tapAndHoldGestureEvent(event,gesture);
}

void DuiWidgetView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  gDuiWidgetViewStub->panGestureEvent(event,gesture);
}

void DuiWidgetView::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  gDuiWidgetViewStub->pinchGestureEvent(event,gesture);
}

void DuiWidgetView::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  gDuiWidgetViewStub->tapGestureEvent(event,gesture);
}

void DuiWidgetView::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  gDuiWidgetViewStub->swipeGestureEvent(event, gesture);
}

#endif

