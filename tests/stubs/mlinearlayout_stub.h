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

#ifndef MLINEARLAYOUT_STUB
#define MLINEARLAYOUT_STUB

#include "mlinearlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MLinearLayoutStub : public StubBase
{
public:
    virtual void MLinearLayoutConstructor(Qt::Orientation orientation, QGraphicsLayoutItem *parent);
    virtual void MLinearLayoutDestructor();
    virtual MLayoutFactory::Type type();
    virtual void setMargin(int margin);
    virtual void setPadding(int padding);
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    virtual void addItemsToLayout(QList<MMultiLayoutItem *> const  &items);
    virtual void updateStyle();
    virtual QList<MMultiLayoutItem::LayoutParameter> layoutParameters();
    virtual void MLinearLayoutConstructor(const MLinearLayout &);
    MLinearLayoutPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MLinearLayoutStub::MLinearLayoutConstructor(Qt::Orientation orientation, QGraphicsLayoutItem *parent)
{
    Q_UNUSED(orientation);
    Q_UNUSED(parent);

}
void MLinearLayoutStub::MLinearLayoutDestructor()
{

}
MLayoutFactory::Type MLinearLayoutStub::type()
{
    stubMethodEntered("type");
    return stubReturnValue<MLayoutFactory::Type>("type");
}

void MLinearLayoutStub::setMargin(int margin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(margin));
    stubMethodEntered("setMargin", params);
}

void MLinearLayoutStub::setPadding(int padding)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(padding));
    stubMethodEntered("setPadding", params);
}

QSizeF MLinearLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint >(which));
    params.append(new Parameter<const QSizeF & >(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>("layoutSizeHint");
}

void MLinearLayoutStub::addItemsToLayout(QList<MMultiLayoutItem *> const  &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<MMultiLayoutItem *> const & >(items));
    stubMethodEntered("addItemsToLayout", params);
}

void MLinearLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QList<MMultiLayoutItem::LayoutParameter> MLinearLayoutStub::layoutParameters()
{
    stubMethodEntered("layoutParameters");
    return stubReturnValue<QList<MMultiLayoutItem::LayoutParameter> >("layoutParameters");
}

void MLinearLayoutStub::MLinearLayoutConstructor(const MLinearLayout &)
{

}


// 3. CREATE A STUB INSTANCE
MLinearLayoutStub gDefaultMLinearLayoutStub;
MLinearLayoutStub *gMLinearLayoutStub = &gDefaultMLinearLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MLinearLayout::MLinearLayout(Qt::Orientation orientation, QGraphicsLayoutItem *parent): d_ptr(0)
{
    gMLinearLayoutStub->MLinearLayoutConstructor(orientation, parent);
}

MLinearLayout::~MLinearLayout()
{
    gMLinearLayoutStub->MLinearLayoutDestructor();
}

MLayoutFactory::Type MLinearLayout::type()
{
    return gMLinearLayoutStub->type();
}

void MLinearLayout::setMargin(int margin)
{
    gMLinearLayoutStub->setMargin(margin);
}

void MLinearLayout::setPadding(int padding)
{
    gMLinearLayoutStub->setPadding(padding);
}

QSizeF MLinearLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMLinearLayoutStub->layoutSizeHint(which, constraint);
}

void MLinearLayout::addItemsToLayout(QList<MMultiLayoutItem *> const  &items)
{
    gMLinearLayoutStub->addItemsToLayout(items);
}

void MLinearLayout::updateStyle()
{
    gMLinearLayoutStub->updateStyle();
}

QList<MMultiLayoutItem::LayoutParameter> MLinearLayout::layoutParameters()
{
    return gMLinearLayoutStub->layoutParameters();
}

MLinearLayout::MLinearLayout(const MLinearLayout &):
    MAbstractLayout(),
    QGraphicsLinearLayout(),
    d_ptr(0)
{
}

MLinearLayout &MLinearLayout::operator=(const MLinearLayout &)
{
    static MLinearLayout bla(Qt::Horizontal);
    return bla;
}


#endif
