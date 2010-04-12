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

#ifndef MWIDGETVIEW_STUB
#define MWIDGETVIEW_STUB

#include "mwidgetview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MWidgetViewStub : public StubBase
{
public:
    virtual void mWidgetViewConstructor();
    virtual void mWidgetViewDestructor();
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
void MWidgetViewStub::mWidgetViewConstructor()
{
}

void MWidgetViewStub::mWidgetViewDestructor()
{
}

QPainterPath MWidgetViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

void MWidgetViewStub::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneResizeEvent * >(event));
    stubMethodEntered("resizeEvent", params);
}

void MWidgetViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mousePressEvent", params);
}

void MWidgetViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void MWidgetViewStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    stubMethodEntered("mouseMoveEvent", params);
}

QSizeF MWidgetViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

void MWidgetViewStub::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapAndHoldGesture * >(gesture));
  stubMethodEntered("tapAndHoldGesture",params);
}

void MWidgetViewStub::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPanGesture * >(gesture));
  stubMethodEntered("panGesture",params);
}

void MWidgetViewStub::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QPinchGesture * >(gesture));
  stubMethodEntered("pinchGesture",params);
}

void MWidgetViewStub::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QTapGesture * >(gesture));
  stubMethodEntered("tapGesture",params);
}

void MWidgetViewStub::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<QGestureEvent * >(event));
  params.append( new Parameter<QSwipeGesture * >(gesture));
  stubMethodEntered("swipeGesture",params);
}

// 3. CREATE A STUB INSTANCE
MWidgetViewStub gDefaultMWidgetViewStub;
MWidgetViewStub *gMWidgetViewStub = &gDefaultMWidgetViewStub;

/*
// 4. CREATE A PROXY WHICH CALLS THE STUB
MWidgetView::MWidgetView(MWidgetController* controller) :
MStylableBase(this, controller), d_ptr(0)
{
    Q_UNUSED(controller);
    gMWidgetViewStub->mWidgetViewConstructor();
}
*/

MWidgetView::~MWidgetView()
{
    gMWidgetViewStub->mWidgetViewDestructor();
}

QPainterPath MWidgetView::shape() const
{
    return gMWidgetViewStub->shape();
}

void MWidgetView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    gMWidgetViewStub->resizeEvent(event);
}

void MWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gMWidgetViewStub->mousePressEvent(event);
}

void MWidgetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gMWidgetViewStub->mouseReleaseEvent(event);
}

void MWidgetView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gMWidgetViewStub->mouseMoveEvent(event);
}

QSizeF MWidgetView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMWidgetViewStub->sizeHint(which, constraint);
}

void MWidgetView::tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture)
{
  gMWidgetViewStub->tapAndHoldGestureEvent(event,gesture);
}

void MWidgetView::panGestureEvent(QGestureEvent *event, QPanGesture *gesture)
{
  gMWidgetViewStub->panGestureEvent(event,gesture);
}

void MWidgetView::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
  gMWidgetViewStub->pinchGestureEvent(event,gesture);
}

void MWidgetView::tapGestureEvent(QGestureEvent *event, QTapGesture *gesture)
{
  gMWidgetViewStub->tapGestureEvent(event,gesture);
}

void MWidgetView::swipeGestureEvent(QGestureEvent *event, QSwipeGesture *gesture)
{
  gMWidgetViewStub->swipeGestureEvent(event, gesture);
}

#endif

