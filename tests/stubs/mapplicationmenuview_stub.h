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

#ifndef MAPPLICATIONMENUVIEW_STUB
#define MAPPLICATIONMENUVIEW_STUB

#include "mapplicationmenuview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationMenuViewStub : public StubBase
{
public:
    virtual void MApplicationMenuViewConstructor(MApplicationMenu *controller);
    virtual void MApplicationMenuViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void updateStyle();
    virtual void setGeometry(const QRectF &rect);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
    MApplicationMenuViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MApplicationMenuViewStub::MApplicationMenuViewConstructor(MApplicationMenu *controller)
{
    Q_UNUSED(controller);

}
void MApplicationMenuViewStub::MApplicationMenuViewDestructor()
{

}
void MApplicationMenuViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF MApplicationMenuViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void MApplicationMenuViewStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void MApplicationMenuViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QSizeF MApplicationMenuViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>(QString("sizeHint"));
}


// 3. CREATE A STUB INSTANCE
MApplicationMenuViewStub gDefaultMApplicationMenuViewStub;
MApplicationMenuViewStub *gMApplicationMenuViewStub = &gDefaultMApplicationMenuViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MApplicationMenuView::MApplicationMenuView(MApplicationMenu *controller)
{
    gMApplicationMenuViewStub->MApplicationMenuViewConstructor(controller);
}

MApplicationMenuView::~MApplicationMenuView()
{
    gMApplicationMenuViewStub->MApplicationMenuViewDestructor();
}

void MApplicationMenuView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gMApplicationMenuViewStub->paint(painter, option, widget);
}

QRectF MApplicationMenuView::boundingRect() const
{
    return gMApplicationMenuViewStub->boundingRect();
}

void MApplicationMenuView::updateStyle()
{
    gMApplicationMenuViewStub->updateStyle();
}

void MApplicationMenuView::setGeometry(const QRectF &rect)
{
    gMApplicationMenuViewStub->setGeometry(rect);
}

QSizeF MApplicationMenuView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMApplicationMenuViewStub->sizeHint(which, constraint);
}


#endif
