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

#ifndef DUITOOLBARVIEW_STUB
#define DUITOOLBARVIEW_STUB

#include "duitoolbarview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiToolbarViewStub : public StubBase
{
public:
    virtual void DuiToolbarViewConstructor(DuiToolbar *controller);
    virtual void DuiToolbarViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    virtual void updateStyle();
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint);
    DuiToolbarViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiToolbarViewStub::DuiToolbarViewConstructor(DuiToolbar *controller)
{
    Q_UNUSED(controller);

}
void DuiToolbarViewStub::DuiToolbarViewDestructor()
{

}
void DuiToolbarViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

QRectF DuiToolbarViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void DuiToolbarViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

void DuiToolbarViewStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QSizeF DuiToolbarViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<const QSizeF &>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>(QString("sizeHint"));
}


// 3. CREATE A STUB INSTANCE
DuiToolbarViewStub gDefaultDuiToolbarViewStub;
DuiToolbarViewStub *gDuiToolbarViewStub = &gDefaultDuiToolbarViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiToolbarView::DuiToolbarView(DuiToolbar *controller)
{
    gDuiToolbarViewStub->DuiToolbarViewConstructor(controller);
}

DuiToolbarView::~DuiToolbarView()
{
    gDuiToolbarViewStub->DuiToolbarViewDestructor();
}

void DuiToolbarView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDuiToolbarViewStub->paint(painter, option, widget);
}

QRectF DuiToolbarView::boundingRect() const
{
    return gDuiToolbarViewStub->boundingRect();
}

void DuiToolbarView::setGeometry(const QRectF &rect)
{
    gDuiToolbarViewStub->setGeometry(rect);
}

void DuiToolbarView::updateStyle()
{
    gDuiToolbarViewStub->updateStyle();
}

QSizeF DuiToolbarView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiToolbarViewStub->sizeHint(which, constraint);
}


#endif
