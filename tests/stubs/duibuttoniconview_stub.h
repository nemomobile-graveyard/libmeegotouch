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

#ifndef DUIBUTTONICONVIEW_STUB_H
#define DUIBUTTONICONVIEW_STUB_H

#include <stubbase.h>
#include <duibuttoniconview.h>

class DuiButtonIconViewStub : public StubBase
{
public:
    virtual void duiButtonIconViewConstructor(DuiButton *controller);
    virtual void duiButtonIconViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void updateData(const int index);
    virtual void timelineValueChanged(qreal value);
    virtual void registerStyleAttributes(DuiStyleDescription &description);

};

void DuiButtonIconViewStub::duiButtonIconViewConstructor(DuiButton *controller)
{
    Q_UNUSED(controller);
}

void DuiButtonIconViewStub::duiButtonIconViewDestructor()
{
}

void DuiButtonIconViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter *>(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem *>(option));
    params.append(new Parameter<QWidget *>(widget));
    stubMethodEntered("paint", params);
}

void DuiButtonIconViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF &>(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF DuiButtonIconViewStub::boundingRect() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("boundingRect", params);
    return stubReturnValue<QRectF>(QString("boundingRect"));
}

void DuiButtonIconViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiButtonIconViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiButtonIconViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int>(index));
    stubMethodEntered("updateData", params);
}

void DuiButtonIconViewStub::timelineValueChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("timelineValueChanged", params);
}

void DuiButtonIconViewStub::registerStyleAttributes(DuiStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiStyleDescription &>(description));
    stubMethodEntered("registerStyleAttributes", params);
}

DuiButtonIconViewStub gDefaultDuiButtonIconViewStub;
DuiButtonIconViewStub *gDuiButtonIconViewStub = &gDefaultDuiButtonIconViewStub;

DuiButtonIconView::DuiButtonIconView(DuiButton *controller)
{
    gDuiButtonIconViewStub->duiButtonIconViewConstructor(controller);
}

DuiButtonIconView::~DuiButtonIconView()
{
    gDuiButtonIconViewStub->duiButtonIconViewDestructor();
}

void DuiButtonIconView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiButtonIconViewStub->paint(painter, option, widget);
}

void DuiButtonIconView::setGeometry(const QRectF &rect)
{
    gDuiButtonIconViewStub->setGeometry(rect);
}

QRectF DuiButtonIconView::boundingRect() const
{
    return gDuiButtonIconViewStub->boundingRect();
}

void DuiButtonIconView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiButtonIconViewStub->mousePressEvent(event);
}

void DuiButtonIconView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiButtonIconViewStub->mouseReleaseEvent(event);
}

void DuiButtonIconView::updateData(const int index)
{
    gDuiButtonIconViewStub->updateData(index);
}

void DuiButtonIconView::timelineValueChanged(qreal value)
{
    gDuiButtonIconViewStub->timelineValueChanged(value);
}

void DuiButtonIconView::registerStyleAttributes(DuiStyleDescription &description)
{
    gDuiButtonIconViewStub->registerStyleAttributes(description);
}

void DuiButtonIconView::styleUpdated()
{
}

#endif
