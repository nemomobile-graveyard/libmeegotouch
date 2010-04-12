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

#ifndef MPANNABLEWIDGET_STUB
#define MPANNABLEWIDGET_STUB

#include "mpannablewidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MPannableWidgetStub : public StubBase
{
public:
    virtual void MPannableWidgetConstructor(Qt::Orientations direction, MWidget *parent);
    virtual void MPannableWidgetDestructor();
    virtual void updateStyle();
    virtual bool panningEnabled(Qt::Orientations const  direction) const;
    virtual void setPhysics(MPhysics2DPanning *physics);
    virtual MPhysics2DPanning *physics() const;
    virtual bool sceneEvent(QEvent *event);
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    virtual void timeOut();
    virtual void updatePosition(QPointF const  &pos);
    virtual bool checkForResent(QGraphicsItem *watched, QEvent *event);
    virtual void myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual bool myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual bool myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual void resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    MPannableWidgetPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MPannableWidgetStub::MPannableWidgetConstructor(Qt::Orientations direction, MWidget *parent)
{
    Q_UNUSED(direction);
    Q_UNUSED(parent);

}
void MPannableWidgetStub::MPannableWidgetDestructor()
{

}

void MPannableWidgetStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

bool MPannableWidgetStub::panningEnabled(Qt::Orientations const  direction) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::Orientations const  >(direction));
    stubMethodEntered("panningEnabled", params);
    return stubReturnValue<bool>("panningEnabled");
}

void MPannableWidgetStub::setPhysics(MPhysics2DPanning *physics)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MPhysics2DPanning * >(physics));
    stubMethodEntered("setPhysics", params);
}

MPhysics2DPanning *MPannableWidgetStub::physics() const
{
    stubMethodEntered("physics");
    return stubReturnValue<MPhysics2DPanning *>("physics");
}

bool MPannableWidgetStub::sceneEvent(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("sceneEvent", params);
    return stubReturnValue<bool>("sceneEvent");
}

bool MPannableWidgetStub::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem * >(watched));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("sceneEventFilter", params);
    return stubReturnValue<bool>("sceneEventFilter");
}

void MPannableWidgetStub::timeOut()
{
    stubMethodEntered("timeOut");
}

void MPannableWidgetStub::updatePosition(QPointF const  &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF const & >(pos));
    stubMethodEntered("updatePosition", params);
}

bool MPannableWidgetStub::checkForResent(QGraphicsItem *watched, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem * >(watched));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("checkForResent", params);
    return stubReturnValue<bool>("checkForResent");
}

void MPannableWidgetStub::myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMousePressEvent", params);
}

bool MPannableWidgetStub::myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMouseReleaseEvent", params);
    return stubReturnValue<bool>("myMouseReleaseEvent");
}

bool MPannableWidgetStub::myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMouseMoveEvent", params);
    return stubReturnValue<bool>("myMouseMoveEvent");
}

void MPannableWidgetStub::resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("resendEvent", params);
}



// 3. CREATE A STUB INSTANCE
MPannableWidgetStub gDefaultMPannableWidgetStub;
MPannableWidgetStub *gMPannableWidgetStub = &gDefaultMPannableWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MPannableWidget::MPannableWidget(Qt::Orientations direction, MWidget *parent)
{
    gMPannableWidgetStub->MPannableWidgetConstructor(direction, parent);
}

MPannableWidget::~MPannableWidget()
{
    gMPannableWidgetStub->MPannableWidgetDestructor();
}

void MPannableWidget::updateStyle()
{
    gMPannableWidgetStub->updateStyle();
}

bool MPannableWidget::panningEnabled(Qt::Orientations const  direction) const
{
    return gMPannableWidgetStub->panningEnabled(direction);
}

void MPannableWidget::setPhysics(MPhysics2DPanning *physics)
{
    gMPannableWidgetStub->setPhysics(physics);
}

MPhysics2DPanning *MPannableWidget::physics(void) const
{
    return gMPannableWidgetStub->physics();
}

bool MPannableWidget::sceneEvent(QEvent *event)
{
    return gMPannableWidgetStub->sceneEvent(event);
}

bool MPannableWidget::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    return gMPannableWidgetStub->sceneEventFilter(watched, event);
}

void MPannableWidget::timeOut(void)
{
    gMPannableWidgetStub->timeOut();
}

void MPannableWidget::updatePosition(QPointF const  &pos)
{
    gMPannableWidgetStub->updatePosition(pos);
}

bool MPannableWidget::checkForResent(QGraphicsItem *watched, QEvent *event)
{
    return gMPannableWidgetStub->checkForResent(watched, event);
}

void MPannableWidget::myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    gMPannableWidgetStub->myMousePressEvent(event, watched);
}

bool MPannableWidget::myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    return gMPannableWidgetStub->myMouseReleaseEvent(event, watched);
}

bool MPannableWidget::myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    return gMPannableWidgetStub->myMouseMoveEvent(event, watched);
}

void MPannableWidget::resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    gMPannableWidgetStub->resendEvent(event, watched);
}


#endif
