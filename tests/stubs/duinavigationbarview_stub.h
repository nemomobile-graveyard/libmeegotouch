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

#ifndef DUINAVIGATIONBARVIEW_STUB
#define DUINAVIGATIONBARVIEW_STUB

#include "duinavigationbarview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiNavigationBarViewStub : public StubBase
{
public:
    virtual void DuiNavigationBarViewConstructor(const DuiNavigationBar *controller);
    virtual void DuiNavigationBarViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    virtual void setGeometry(const QRectF &rect);
    virtual void styleUpdated();
    virtual void registerStyleAttributes(DuiStyleDescription &description);
    virtual void updateData(const int index);
    virtual void viewModeTransition(qreal value);
};

// 2. IMPLEMENT STUB
void DuiNavigationBarViewStub::DuiNavigationBarViewConstructor(const DuiNavigationBar *controller)
{
    Q_UNUSED(controller);

}
void DuiNavigationBarViewStub::DuiNavigationBarViewDestructor()
{

}
void DuiNavigationBarViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF DuiNavigationBarViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QSizeF DuiNavigationBarViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    return stubReturnValue<QSizeF>("sizeHint");
}

void DuiNavigationBarViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void DuiNavigationBarViewStub::styleUpdated()
{
    stubMethodEntered("styleUpdated");
}

void DuiNavigationBarViewStub::registerStyleAttributes(DuiStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiStyleDescription & >(description));
    stubMethodEntered("registerStyleAttributes", params);
}

void DuiNavigationBarViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("updateData", params);
}

void DuiNavigationBarViewStub::viewModeTransition(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("viewModeTransition", params);
}



// 3. CREATE A STUB INSTANCE
DuiNavigationBarViewStub gDefaultDuiNavigationBarViewStub;
DuiNavigationBarViewStub *gDuiNavigationBarViewStub = &gDefaultDuiNavigationBarViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiNavigationBarView::DuiNavigationBarView(const DuiNavigationBar *controller)
{
    gDuiNavigationBarViewStub->DuiNavigationBarViewConstructor(controller);
}

DuiNavigationBarView::~DuiNavigationBarView()
{
    gDuiNavigationBarViewStub->DuiNavigationBarViewDestructor();
}

void DuiNavigationBarView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiNavigationBarViewStub->paint(painter, option, widget);
}

QRectF DuiNavigationBarView::boundingRect() const
{
    return gDuiNavigationBarViewStub->boundingRect();
}

QSizeF DuiNavigationBarView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiNavigationBarViewStub->sizeHint(which, constraint);
}

void DuiNavigationBarView::setGeometry(const QRectF &rect)
{
    gDuiNavigationBarViewStub->setGeometry(rect);
}

void DuiNavigationBarView::styleUpdated()
{
    gDuiNavigationBarViewStub->styleUpdated();
}

void DuiNavigationBarView::registerStyleAttributes(DuiStyleDescription &description)
{
    gDuiNavigationBarViewStub->registerStyleAttributes(description);
}

void DuiNavigationBarView::updateData(const int index)
{
    gDuiNavigationBarViewStub->updateData(index);
}

void DuiNavigationBarView::viewModeTransition(qreal value)
{
    gDuiNavigationBarViewStub->viewModeTransition(value);
}


#endif
