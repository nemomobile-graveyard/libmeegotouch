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

#ifndef MAPPLICATIONVIEW_STUB_H
#define MAPPLICATIONVIEW_STUB_H

#include <stubbase.h>
#include <mapplicationview.h>

class MApplicationViewStub : public StubBase
{
public:
    virtual void mApplicationViewConstructor(QGraphicsWidget *parent = 0);
    virtual void mApplicationViewDestructor();
    virtual void setWidget(QGraphicsWidget *widget);
    virtual QGraphicsWidget *widget() const;
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};

void MApplicationViewStub::mApplicationViewConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);
}

void MApplicationViewStub::mApplicationViewDestructor()
{
}

void MApplicationViewStub::setWidget(QGraphicsWidget *widget)
{
    Q_UNUSED(widget);
    /*
    QList<ParameterBase*> params;
    params.append( new Parameter<QGraphicsWidget*>(widget) );
    stubMethodEntered("setWidget", params);
    */
}

QGraphicsWidget *MApplicationViewStub::widget() const
{
    /*
    QList<ParameterBase*> params;
    stubMethodEntered("widget", params);
    return stubReturnValue<QGraphicsWidget*>(QString("widget"));
    */
    return new QGraphicsWidget();
}

QRectF MApplicationViewStub::boundingRect() const
{
    /*
    QList<ParameterBase*> params;
    stubMethodEntered("boundingRect", params);
    return stubReturnValue<QRectF>(QString("boundingRect"));
    */
    return QRectF(0, 0, 0, 0);
}

void MApplicationViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

MApplicationViewStub gDefaultMApplicationViewStub;
MApplicationViewStub *gMApplicationViewStub = &gDefaultMApplicationViewStub;

MApplicationView::MApplicationView(QGraphicsWidget *parent)
{
    gMApplicationViewStub->mApplicationViewConstructor(parent);
}

MApplicationView::~MApplicationView()
{
    gMApplicationViewStub->mApplicationViewDestructor();
}

void MApplicationView::setWidget(QGraphicsWidget *widget)
{
    gMApplicationViewStub->setWidget(widget);
}

QGraphicsWidget *MApplicationView::widget() const
{
    return gMApplicationViewStub->widget();
}

QRectF MApplicationView::boundingRect() const
{
    return gMApplicationViewStub->boundingRect();
}

void MApplicationView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMApplicationViewStub->paint(painter, option, widget);
}

#endif
