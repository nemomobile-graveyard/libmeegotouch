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

#ifndef MBUTTONICONVIEW_STUB_H
#define MBUTTONICONVIEW_STUB_H

#include <stubbase.h>
#include <mbuttoniconview.h>

class MButtonIconViewStub : public StubBase
{
public:
    virtual void mButtonIconViewConstructor(MButton *controller);
    virtual void mButtonIconViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void updateData(const int index);
    virtual void timelineValueChanged(qreal value);
    virtual void registerStyleAttributes(MStyleDescription &description);

};

void MButtonIconViewStub::mButtonIconViewConstructor(MButton *controller)
{
    Q_UNUSED(controller);
}

void MButtonIconViewStub::mButtonIconViewDestructor()
{
}

void MButtonIconViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter *>(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem *>(option));
    params.append(new Parameter<QWidget *>(widget));
    stubMethodEntered("paint", params);
}

void MButtonIconViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF &>(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF MButtonIconViewStub::boundingRect() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("boundingRect", params);
    return stubReturnValue<QRectF>(QString("boundingRect"));
}

void MButtonIconViewStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mousePressEvent", params);
}

void MButtonIconViewStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void MButtonIconViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int>(index));
    stubMethodEntered("updateData", params);
}

void MButtonIconViewStub::timelineValueChanged(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("timelineValueChanged", params);
}

void MButtonIconViewStub::registerStyleAttributes(MStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MStyleDescription &>(description));
    stubMethodEntered("registerStyleAttributes", params);
}

MButtonIconViewStub gDefaultMButtonIconViewStub;
MButtonIconViewStub *gMButtonIconViewStub = &gDefaultMButtonIconViewStub;

MButtonIconView::MButtonIconView(MButton *controller)
{
    gMButtonIconViewStub->mButtonIconViewConstructor(controller);
}

MButtonIconView::~MButtonIconView()
{
    gMButtonIconViewStub->mButtonIconViewDestructor();
}

void MButtonIconView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMButtonIconViewStub->paint(painter, option, widget);
}

void MButtonIconView::setGeometry(const QRectF &rect)
{
    gMButtonIconViewStub->setGeometry(rect);
}

QRectF MButtonIconView::boundingRect() const
{
    return gMButtonIconViewStub->boundingRect();
}

void MButtonIconView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gMButtonIconViewStub->mousePressEvent(event);
}

void MButtonIconView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gMButtonIconViewStub->mouseReleaseEvent(event);
}

void MButtonIconView::updateData(const int index)
{
    gMButtonIconViewStub->updateData(index);
}

void MButtonIconView::timelineValueChanged(qreal value)
{
    gMButtonIconViewStub->timelineValueChanged(value);
}

void MButtonIconView::registerStyleAttributes(MStyleDescription &description)
{
    gMButtonIconViewStub->registerStyleAttributes(description);
}

void MButtonIconView::styleUpdated()
{
}

#endif
