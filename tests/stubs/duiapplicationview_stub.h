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

#ifndef DUIAPPLICATIONVIEW_STUB_H
#define DUIAPPLICATIONVIEW_STUB_H

#include <stubbase.h>
#include <duiapplicationview.h>

class DuiApplicationViewStub : public StubBase
{
public:
    virtual void duiApplicationViewConstructor(QGraphicsWidget *parent = 0);
    virtual void duiApplicationViewDestructor();
    virtual void setWidget(QGraphicsWidget *widget);
    virtual QGraphicsWidget *widget() const;
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

void DuiApplicationViewStub::duiApplicationViewConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);
}

void DuiApplicationViewStub::duiApplicationViewDestructor()
{
}

void DuiApplicationViewStub::setWidget(QGraphicsWidget *widget)
{
    Q_UNUSED(widget);
    /*
    QList<ParameterBase*> params;
    params.append( new Parameter<QGraphicsWidget*>(widget) );
    stubMethodEntered("setWidget", params);
    */
}

QGraphicsWidget *DuiApplicationViewStub::widget() const
{
    /*
    QList<ParameterBase*> params;
    stubMethodEntered("widget", params);
    return stubReturnValue<QGraphicsWidget*>(QString("widget"));
    */
    return new QGraphicsWidget();
}

QRectF DuiApplicationViewStub::boundingRect() const
{
    /*
    QList<ParameterBase*> params;
    stubMethodEntered("boundingRect", params);
    return stubReturnValue<QRectF>(QString("boundingRect"));
    */
    return QRectF(0, 0, 0, 0);
}

void DuiApplicationViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
    /*
    QList<ParameterBase*> params;
    params.append( new Parameter<QPainter*>(painter) );
    params.append( new Parameter<const QStyleOptionGraphicsItem*>(option) );
    params.append( new Parameter<QWidget*>(widget) );
    stubMethodEntered("paint", params);
    */
}

DuiApplicationViewStub gDefaultDuiApplicationViewStub;
DuiApplicationViewStub *gDuiApplicationViewStub = &gDefaultDuiApplicationViewStub;

DuiApplicationView::DuiApplicationView(QGraphicsWidget *parent)
{
    gDuiApplicationViewStub->duiApplicationViewConstructor(parent);
}

DuiApplicationView::~DuiApplicationView()
{
    gDuiApplicationViewStub->duiApplicationViewDestructor();
}

void DuiApplicationView::setWidget(QGraphicsWidget *widget)
{
    gDuiApplicationViewStub->setWidget(widget);
}

QGraphicsWidget *DuiApplicationView::widget() const
{
    return gDuiApplicationViewStub->widget();
}

QRectF DuiApplicationView::boundingRect() const
{
    return gDuiApplicationViewStub->boundingRect();
}

void DuiApplicationView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiApplicationViewStub->paint(painter, option, widget);
}

#endif
