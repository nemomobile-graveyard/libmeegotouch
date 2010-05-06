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

#ifndef MGRIDLAYOUT_STUB
#define MGRIDLAYOUT_STUB

#include "mgridlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MGridLayoutStub : public StubBase
{
public:
    virtual void MGridLayoutConstructor(QGraphicsLayoutItem *parent);
    virtual void MGridLayoutDestructor();
    virtual void setMargin(int margin);
    virtual void setPadding(int padding);
    virtual void updateStyle();
    virtual void addItemsToLayout(QList <MMultiLayoutItem * > const &items);
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

    //virtual void Q_DISABLE_COPYConstructor(MGridLayout);
    MGridLayoutPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MGridLayoutStub::MGridLayoutConstructor(QGraphicsLayoutItem *parent)
{
    Q_UNUSED(parent);

}
void MGridLayoutStub::MGridLayoutDestructor()
{

}
void MGridLayoutStub::setMargin(int margin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(margin));
    stubMethodEntered("setMargin", params);
}

void MGridLayoutStub::setPadding(int padding)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(padding));
    stubMethodEntered("setPadding", params);
}

void MGridLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void MGridLayoutStub::addItemsToLayout(QList <MMultiLayoutItem * > const &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QList <MMultiLayoutItem * > >(items));
    stubMethodEntered("addItemsToLayout", params);
}

QSizeF MGridLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>(QString("layoutSizeHint"));
}

/*
void MGridLayoutStub::Q_DISABLE_COPYConstructor(MGridLayout) {

}
*/

// 3. CREATE A STUB INSTANCE
MGridLayoutStub gDefaultMGridLayoutStub;
MGridLayoutStub *gMGridLayoutStub = &gDefaultMGridLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MGridLayout::MGridLayout(QGraphicsLayoutItem *parent) : d_ptr(0)
{
    gMGridLayoutStub->MGridLayoutConstructor(parent);
}

MGridLayout::~MGridLayout()
{
    gMGridLayoutStub->MGridLayoutDestructor();
}

void MGridLayout::setMargin(int margin)
{
    gMGridLayoutStub->setMargin(margin);
}

void MGridLayout::setPadding(int padding)
{
    gMGridLayoutStub->setPadding(padding);
}

void MGridLayout::updateStyle()
{
    gMGridLayoutStub->updateStyle();
}
void MGridLayout::addItemsToLayout(QList <MMultiLayoutItem * > const &items)
{
    gMGridLayoutStub->addItemsToLayout(items);
}


QSizeF MGridLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMGridLayoutStub->layoutSizeHint(which, constraint);

}

QList<MMultiLayoutItem::LayoutParameter> MGridLayout::layoutParameters()
{
    QList<MMultiLayoutItem::LayoutParameter> params;
    return params;
}
/*
MGridLayout::Q_DISABLE_COPY(MGridLayout) {
  gMGridLayoutStub->Q_DISABLE_COPYConstructor();
}
*/

#endif
