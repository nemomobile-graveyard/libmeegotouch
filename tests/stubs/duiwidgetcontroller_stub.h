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

#ifndef DUIWIDGETCONTROLLER_STUB_H
#define DUIWIDGETCONTROLLER_STUB_H

#include <duiwidgetcontroller.h>
#include <stubbase.h>

class DuiWidgetView;
class QGraphicsItem;

/**
 * DuiWidgetController mocker base class.
 * To mock DuiWidgetController operations, derive from this class
 * and implement the methods you want to mock. Instantiate your
 * derived mock class and assign it to gDuiWidgetControllerStub
 * global variable.
 */
class DuiWidgetControllerStub : public StubBase
{
public:
    virtual void duiWidgetControllerConstructor(QGraphicsItem *parent, const QString &viewType);
    virtual void duiWidgetControllerDestructor();

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void setObjectName(const QString &name);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void setView(DuiWidgetView *view);


    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
};

void DuiWidgetControllerStub::duiWidgetControllerConstructor(QGraphicsItem *parent, const QString &viewType)
{
    Q_UNUSED(parent);
    Q_UNUSED(viewType);
}

void DuiWidgetControllerStub::duiWidgetControllerDestructor()
{
}

void DuiWidgetControllerStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter *>(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem *>(option));
    params.append(new Parameter<QWidget *>(widget));
    stubMethodEntered("paint", params);
}

void DuiWidgetControllerStub::setObjectName(const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString &>(name));
    stubMethodEntered("setObjectName", params);
}

void DuiWidgetControllerStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF &>(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF DuiWidgetControllerStub::boundingRect() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("boundingRect", params);
    return stubReturnValue<QRectF>(QString("boundingRect"));
}

QPainterPath DuiWidgetControllerStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}

QSizeF DuiWidgetControllerStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>(QString("sizeHint"));
}

void DuiWidgetControllerStub::setView(DuiWidgetView *view)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidgetView *>(view));
    stubMethodEntered("setView", params);
}

void DuiWidgetControllerStub::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mousePressEvent", params);
}

void DuiWidgetControllerStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseReleaseEvent", params);
}

void DuiWidgetControllerStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneMouseEvent *>(event));
    stubMethodEntered("mouseMoveEvent", params);
}

void DuiWidgetControllerStub::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneResizeEvent *>(event));
    stubMethodEntered("resizeEvent", params);
}

DuiWidgetControllerStub gDefaultDuiWidgetControllerStub;

/**
 * This is the mock class instance used in the mocking. If you want to alter mocking behaviour
 * derive your mocker from DuiWidgetControllerStub, implement the methods you want to
 * affect, create an instance of your mocker class and assign the instance into this global variable.
 */
DuiWidgetControllerStub *gDuiWidgetControllerStub = &gDefaultDuiWidgetControllerStub;

/**
 * These are the stub method implementations of DuiWidgetController. They will
 * call the mocking methods of the gDuiWidgetControllerStub.
 */
DuiWidgetController::DuiWidgetController(QGraphicsItem *parent, const QString &viewType)
    : DuiWidget(parent)
{
    gDuiWidgetControllerStub->duiWidgetControllerConstructor(parent, viewType);
}

DuiWidgetController::~DuiWidgetController()
{
    gDuiWidgetControllerStub->duiWidgetControllerDestructor();
}

void DuiWidgetController::setGeometry(const QRectF &rect)
{
    gDuiWidgetControllerStub->setGeometry(rect);
}

void DuiWidgetController::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiWidgetControllerStub->paint(painter, option, widget);
}

void DuiWidgetController::setObjectName(const QString &name)
{
    gDuiWidgetControllerStub->setObjectName(name);
}

QRectF DuiWidgetController::boundingRect() const
{
    return gDuiWidgetControllerStub->boundingRect();
}

QPainterPath DuiWidgetController::shape() const
{
    return gDuiWidgetControllerStub->shape();
}

QSizeF DuiWidgetController::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiWidgetControllerStub->sizeHint(which, constraint);
}

void DuiWidgetController::setView(DuiWidgetView *view)
{
    gDuiWidgetControllerStub->setView(view);
}

void DuiWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetControllerStub->mousePressEvent(event);
}

void DuiWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetControllerStub->mouseReleaseEvent(event);
}

void DuiWidgetController::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    gDuiWidgetControllerStub->mouseMoveEvent(event);
}

void DuiWidgetController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    gDuiWidgetControllerStub->resizeEvent(event);
}

#endif
