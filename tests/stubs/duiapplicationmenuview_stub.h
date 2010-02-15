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

#ifndef DUIAPPLICATIONMENUVIEW_STUB
#define DUIAPPLICATIONMENUVIEW_STUB

#include "duiapplicationmenuview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationMenuViewStub : public StubBase
{
public:
    virtual void DuiApplicationMenuViewConstructor(DuiApplicationMenu *controller);
    virtual void DuiApplicationMenuViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void updateStyle();
    virtual void setGeometry(const QRectF &rect);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
    DuiApplicationMenuViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiApplicationMenuViewStub::DuiApplicationMenuViewConstructor(DuiApplicationMenu *controller)
{
    Q_UNUSED(controller);

}
void DuiApplicationMenuViewStub::DuiApplicationMenuViewDestructor()
{

}
void DuiApplicationMenuViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF DuiApplicationMenuViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void DuiApplicationMenuViewStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void DuiApplicationMenuViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QSizeF DuiApplicationMenuViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>(QString("sizeHint"));
}


// 3. CREATE A STUB INSTANCE
DuiApplicationMenuViewStub gDefaultDuiApplicationMenuViewStub;
DuiApplicationMenuViewStub *gDuiApplicationMenuViewStub = &gDefaultDuiApplicationMenuViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiApplicationMenuView::DuiApplicationMenuView(DuiApplicationMenu *controller)
{
    gDuiApplicationMenuViewStub->DuiApplicationMenuViewConstructor(controller);
}

DuiApplicationMenuView::~DuiApplicationMenuView()
{
    gDuiApplicationMenuViewStub->DuiApplicationMenuViewDestructor();
}

void DuiApplicationMenuView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiApplicationMenuViewStub->paint(painter, option, widget);
}

QRectF DuiApplicationMenuView::boundingRect() const
{
    return gDuiApplicationMenuViewStub->boundingRect();
}

void DuiApplicationMenuView::updateStyle()
{
    gDuiApplicationMenuViewStub->updateStyle();
}

void DuiApplicationMenuView::setGeometry(const QRectF &rect)
{
    gDuiApplicationMenuViewStub->setGeometry(rect);
}

QSizeF DuiApplicationMenuView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiApplicationMenuViewStub->sizeHint(which, constraint);
}


#endif
