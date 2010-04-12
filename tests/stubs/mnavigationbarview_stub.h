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

#ifndef MNAVIGATIONBARVIEW_STUB
#define MNAVIGATIONBARVIEW_STUB

#include "mnavigationbarview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNavigationBarViewStub : public StubBase
{
public:
    virtual void MNavigationBarViewConstructor(const MNavigationBar *controller);
    virtual void MNavigationBarViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    virtual void setGeometry(const QRectF &rect);
    virtual void styleUpdated();
    virtual void registerStyleAttributes(MStyleDescription &description);
    virtual void updateData(const int index);
    virtual void viewModeTransition(qreal value);
};

// 2. IMPLEMENT STUB
void MNavigationBarViewStub::MNavigationBarViewConstructor(const MNavigationBar *controller)
{
    Q_UNUSED(controller);

}
void MNavigationBarViewStub::MNavigationBarViewDestructor()
{

}
void MNavigationBarViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF MNavigationBarViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QSizeF MNavigationBarViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    return stubReturnValue<QSizeF>("sizeHint");
}

void MNavigationBarViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void MNavigationBarViewStub::styleUpdated()
{
    stubMethodEntered("styleUpdated");
}

void MNavigationBarViewStub::registerStyleAttributes(MStyleDescription &description)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MStyleDescription & >(description));
    stubMethodEntered("registerStyleAttributes", params);
}

void MNavigationBarViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int >(index));
    stubMethodEntered("updateData", params);
}

void MNavigationBarViewStub::viewModeTransition(qreal value)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(value));
    stubMethodEntered("viewModeTransition", params);
}



// 3. CREATE A STUB INSTANCE
MNavigationBarViewStub gDefaultMNavigationBarViewStub;
MNavigationBarViewStub *gMNavigationBarViewStub = &gDefaultMNavigationBarViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNavigationBarView::MNavigationBarView(const MNavigationBar *controller)
{
    gMNavigationBarViewStub->MNavigationBarViewConstructor(controller);
}

MNavigationBarView::~MNavigationBarView()
{
    gMNavigationBarViewStub->MNavigationBarViewDestructor();
}

void MNavigationBarView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMNavigationBarViewStub->paint(painter, option, widget);
}

QRectF MNavigationBarView::boundingRect() const
{
    return gMNavigationBarViewStub->boundingRect();
}

QSizeF MNavigationBarView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMNavigationBarViewStub->sizeHint(which, constraint);
}

void MNavigationBarView::setGeometry(const QRectF &rect)
{
    gMNavigationBarViewStub->setGeometry(rect);
}

void MNavigationBarView::styleUpdated()
{
    gMNavigationBarViewStub->styleUpdated();
}

void MNavigationBarView::registerStyleAttributes(MStyleDescription &description)
{
    gMNavigationBarViewStub->registerStyleAttributes(description);
}

void MNavigationBarView::updateData(const int index)
{
    gMNavigationBarViewStub->updateData(index);
}

void MNavigationBarView::viewModeTransition(qreal value)
{
    gMNavigationBarViewStub->viewModeTransition(value);
}


#endif
