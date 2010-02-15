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

#ifndef DUIGRIDLAYOUT_STUB
#define DUIGRIDLAYOUT_STUB

#include "duigridlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiGridLayoutStub : public StubBase
{
public:
    virtual void DuiGridLayoutConstructor(QGraphicsLayoutItem *parent);
    virtual void DuiGridLayoutDestructor();
    virtual void setMargin(int margin);
    virtual void setPadding(int padding);
    virtual void updateStyle();
    virtual void addItemsToLayout(QList <DuiMultiLayoutItem * > const &items);
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

    //virtual void Q_DISABLE_COPYConstructor(DuiGridLayout);
    DuiGridLayoutPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiGridLayoutStub::DuiGridLayoutConstructor(QGraphicsLayoutItem *parent)
{
    Q_UNUSED(parent);

}
void DuiGridLayoutStub::DuiGridLayoutDestructor()
{

}
void DuiGridLayoutStub::setMargin(int margin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(margin));
    stubMethodEntered("setMargin", params);
}

void DuiGridLayoutStub::setPadding(int padding)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(padding));
    stubMethodEntered("setPadding", params);
}

void DuiGridLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void DuiGridLayoutStub::addItemsToLayout(QList <DuiMultiLayoutItem * > const &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QList <DuiMultiLayoutItem * > >(items));
    stubMethodEntered("addItemsToLayout", params);
}

QSizeF DuiGridLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>(QString("layoutSizeHint"));
}

/*
void DuiGridLayoutStub::Q_DISABLE_COPYConstructor(DuiGridLayout) {

}
*/

// 3. CREATE A STUB INSTANCE
DuiGridLayoutStub gDefaultDuiGridLayoutStub;
DuiGridLayoutStub *gDuiGridLayoutStub = &gDefaultDuiGridLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiGridLayout::DuiGridLayout(QGraphicsLayoutItem *parent) : d_ptr(0)
{
    gDuiGridLayoutStub->DuiGridLayoutConstructor(parent);
}

DuiGridLayout::~DuiGridLayout()
{
    gDuiGridLayoutStub->DuiGridLayoutDestructor();
}

void DuiGridLayout::setMargin(int margin)
{
    gDuiGridLayoutStub->setMargin(margin);
}

void DuiGridLayout::setPadding(int padding)
{
    gDuiGridLayoutStub->setPadding(padding);
}

void DuiGridLayout::updateStyle()
{
    gDuiGridLayoutStub->updateStyle();
}
void DuiGridLayout::addItemsToLayout(QList <DuiMultiLayoutItem * > const &items)
{
    gDuiGridLayoutStub->addItemsToLayout(items);
}


QSizeF DuiGridLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiGridLayoutStub->layoutSizeHint(which, constraint);

}

QList<DuiMultiLayoutItem::LayoutParameter> DuiGridLayout::layoutParameters()
{
    QList<DuiMultiLayoutItem::LayoutParameter> params;
    return params;
}
/*
DuiGridLayout::Q_DISABLE_COPY(DuiGridLayout) {
  gDuiGridLayoutStub->Q_DISABLE_COPYConstructor();
}
*/

#endif
