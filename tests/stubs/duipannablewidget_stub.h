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

#ifndef DUIPANNABLEWIDGET_STUB
#define DUIPANNABLEWIDGET_STUB

#include "duipannablewidget.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiPannableWidgetStub : public StubBase
{
public:
    virtual void DuiPannableWidgetConstructor(Qt::Orientations direction, DuiWidget *parent);
    virtual void DuiPannableWidgetDestructor();
    virtual void updateStyle();
    virtual bool panningEnabled(Qt::Orientations const  direction) const;
    virtual void setPhysics(DuiPhysics2DPanning *physics);
    virtual DuiPhysics2DPanning *physics() const;
    virtual bool sceneEvent(QEvent *event);
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    virtual void timeOut();
    virtual void updatePosition(QPointF const  &pos);
    virtual bool checkForResent(QGraphicsItem *watched, QEvent *event);
    virtual void myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual bool myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual bool myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    virtual void resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched);
    DuiPannableWidgetPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiPannableWidgetStub::DuiPannableWidgetConstructor(Qt::Orientations direction, DuiWidget *parent)
{
    Q_UNUSED(direction);
    Q_UNUSED(parent);

}
void DuiPannableWidgetStub::DuiPannableWidgetDestructor()
{

}

void DuiPannableWidgetStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

bool DuiPannableWidgetStub::panningEnabled(Qt::Orientations const  direction) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::Orientations const  >(direction));
    stubMethodEntered("panningEnabled", params);
    return stubReturnValue<bool>("panningEnabled");
}

void DuiPannableWidgetStub::setPhysics(DuiPhysics2DPanning *physics)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiPhysics2DPanning * >(physics));
    stubMethodEntered("setPhysics", params);
}

DuiPhysics2DPanning *DuiPannableWidgetStub::physics() const
{
    stubMethodEntered("physics");
    return stubReturnValue<DuiPhysics2DPanning *>("physics");
}

bool DuiPannableWidgetStub::sceneEvent(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("sceneEvent", params);
    return stubReturnValue<bool>("sceneEvent");
}

bool DuiPannableWidgetStub::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem * >(watched));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("sceneEventFilter", params);
    return stubReturnValue<bool>("sceneEventFilter");
}

void DuiPannableWidgetStub::timeOut()
{
    stubMethodEntered("timeOut");
}

void DuiPannableWidgetStub::updatePosition(QPointF const  &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPointF const & >(pos));
    stubMethodEntered("updatePosition", params);
}

bool DuiPannableWidgetStub::checkForResent(QGraphicsItem *watched, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsItem * >(watched));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("checkForResent", params);
    return stubReturnValue<bool>("checkForResent");
}

void DuiPannableWidgetStub::myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMousePressEvent", params);
}

bool DuiPannableWidgetStub::myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMouseReleaseEvent", params);
    return stubReturnValue<bool>("myMouseReleaseEvent");
}

bool DuiPannableWidgetStub::myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("myMouseMoveEvent", params);
    return stubReturnValue<bool>("myMouseMoveEvent");
}

void DuiPannableWidgetStub::resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent * >(event));
    params.append(new Parameter<QGraphicsItem * >(watched));
    stubMethodEntered("resendEvent", params);
}



// 3. CREATE A STUB INSTANCE
DuiPannableWidgetStub gDefaultDuiPannableWidgetStub;
DuiPannableWidgetStub *gDuiPannableWidgetStub = &gDefaultDuiPannableWidgetStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiPannableWidget::DuiPannableWidget(Qt::Orientations direction, DuiWidget *parent)
{
    gDuiPannableWidgetStub->DuiPannableWidgetConstructor(direction, parent);
}

DuiPannableWidget::~DuiPannableWidget()
{
    gDuiPannableWidgetStub->DuiPannableWidgetDestructor();
}

void DuiPannableWidget::updateStyle()
{
    gDuiPannableWidgetStub->updateStyle();
}

bool DuiPannableWidget::panningEnabled(Qt::Orientations const  direction) const
{
    return gDuiPannableWidgetStub->panningEnabled(direction);
}

void DuiPannableWidget::setPhysics(DuiPhysics2DPanning *physics)
{
    gDuiPannableWidgetStub->setPhysics(physics);
}

DuiPhysics2DPanning *DuiPannableWidget::physics(void) const
{
    return gDuiPannableWidgetStub->physics();
}

bool DuiPannableWidget::sceneEvent(QEvent *event)
{
    return gDuiPannableWidgetStub->sceneEvent(event);
}

bool DuiPannableWidget::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    return gDuiPannableWidgetStub->sceneEventFilter(watched, event);
}

void DuiPannableWidget::timeOut(void)
{
    gDuiPannableWidgetStub->timeOut();
}

void DuiPannableWidget::updatePosition(QPointF const  &pos)
{
    gDuiPannableWidgetStub->updatePosition(pos);
}

bool DuiPannableWidget::checkForResent(QGraphicsItem *watched, QEvent *event)
{
    return gDuiPannableWidgetStub->checkForResent(watched, event);
}

void DuiPannableWidget::myMousePressEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    gDuiPannableWidgetStub->myMousePressEvent(event, watched);
}

bool DuiPannableWidget::myMouseReleaseEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    return gDuiPannableWidgetStub->myMouseReleaseEvent(event, watched);
}

bool DuiPannableWidget::myMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    return gDuiPannableWidgetStub->myMouseMoveEvent(event, watched);
}

void DuiPannableWidget::resendEvent(QGraphicsSceneMouseEvent *event, QGraphicsItem *watched)
{
    gDuiPannableWidgetStub->resendEvent(event, watched);
}


#endif
