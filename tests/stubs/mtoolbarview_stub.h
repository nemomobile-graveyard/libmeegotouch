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

#ifndef MTOOLBARVIEW_STUB
#define MTOOLBARVIEW_STUB

#include "mtoolbarview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MToolbarViewStub : public StubBase
{
public:
    virtual void MToolbarViewConstructor(MToolbar *controller);
    virtual void MToolbarViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    virtual void updateStyle();
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
    MToolbarViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MToolbarViewStub::MToolbarViewConstructor(MToolbar *controller)
{
    Q_UNUSED(controller);

}
void MToolbarViewStub::MToolbarViewDestructor()
{

}
void MToolbarViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF MToolbarViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void MToolbarViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void MToolbarViewStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QSizeF MToolbarViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>(QString("sizeHint"));
}


// 3. CREATE A STUB INSTANCE
MToolbarViewStub gDefaultMToolbarViewStub;
MToolbarViewStub *gMToolbarViewStub = &gDefaultMToolbarViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MToolbarView::MToolbarView(MToolbar *controller)
{
    gMToolbarViewStub->MToolbarViewConstructor(controller);
}

MToolbarView::~MToolbarView()
{
    gMToolbarViewStub->MToolbarViewDestructor();
}

void MToolbarView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMToolbarViewStub->paint(painter, option, widget);
}

QRectF MToolbarView::boundingRect() const
{
    return gMToolbarViewStub->boundingRect();
}

void MToolbarView::setGeometry(const QRectF &rect)
{
    gMToolbarViewStub->setGeometry(rect);
}

void MToolbarView::updateStyle()
{
    gMToolbarViewStub->updateStyle();
}

QSizeF MToolbarView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMToolbarViewStub->sizeHint(which, constraint);
}


#endif
