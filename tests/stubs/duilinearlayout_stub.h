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

#ifndef DUILINEARLAYOUT_STUB
#define DUILINEARLAYOUT_STUB

#include "duilinearlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiLinearLayoutStub : public StubBase
{
public:
    virtual void DuiLinearLayoutConstructor(Qt::Orientation orientation, QGraphicsLayoutItem *parent);
    virtual void DuiLinearLayoutDestructor();
    virtual DuiLayoutFactory::Type type();
    virtual void setMargin(int margin);
    virtual void setPadding(int padding);
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    virtual void addItemsToLayout(QList<DuiMultiLayoutItem *> const  &items);
    virtual void updateStyle();
    virtual QList<DuiMultiLayoutItem::LayoutParameter> layoutParameters();
    virtual void DuiLinearLayoutConstructor(const DuiLinearLayout &);
    DuiLinearLayoutPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiLinearLayoutStub::DuiLinearLayoutConstructor(Qt::Orientation orientation, QGraphicsLayoutItem *parent)
{
    Q_UNUSED(orientation);
    Q_UNUSED(parent);

}
void DuiLinearLayoutStub::DuiLinearLayoutDestructor()
{

}
DuiLayoutFactory::Type DuiLinearLayoutStub::type()
{
    stubMethodEntered("type");
    return stubReturnValue<DuiLayoutFactory::Type>("type");
}

void DuiLinearLayoutStub::setMargin(int margin)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(margin));
    stubMethodEntered("setMargin", params);
}

void DuiLinearLayoutStub::setPadding(int padding)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(padding));
    stubMethodEntered("setPadding", params);
}

QSizeF DuiLinearLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint >(which));
    params.append(new Parameter<const QSizeF & >(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>("layoutSizeHint");
}

void DuiLinearLayoutStub::addItemsToLayout(QList<DuiMultiLayoutItem *> const  &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QList<DuiMultiLayoutItem *> const & >(items));
    stubMethodEntered("addItemsToLayout", params);
}

void DuiLinearLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

QList<DuiMultiLayoutItem::LayoutParameter> DuiLinearLayoutStub::layoutParameters()
{
    stubMethodEntered("layoutParameters");
    return stubReturnValue<QList<DuiMultiLayoutItem::LayoutParameter> >("layoutParameters");
}

void DuiLinearLayoutStub::DuiLinearLayoutConstructor(const DuiLinearLayout &)
{

}


// 3. CREATE A STUB INSTANCE
DuiLinearLayoutStub gDefaultDuiLinearLayoutStub;
DuiLinearLayoutStub *gDuiLinearLayoutStub = &gDefaultDuiLinearLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiLinearLayout::DuiLinearLayout(Qt::Orientation orientation, QGraphicsLayoutItem *parent): d_ptr(0)
{
    gDuiLinearLayoutStub->DuiLinearLayoutConstructor(orientation, parent);
}

DuiLinearLayout::~DuiLinearLayout()
{
    gDuiLinearLayoutStub->DuiLinearLayoutDestructor();
}

DuiLayoutFactory::Type DuiLinearLayout::type()
{
    return gDuiLinearLayoutStub->type();
}

void DuiLinearLayout::setMargin(int margin)
{
    gDuiLinearLayoutStub->setMargin(margin);
}

void DuiLinearLayout::setPadding(int padding)
{
    gDuiLinearLayoutStub->setPadding(padding);
}

QSizeF DuiLinearLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiLinearLayoutStub->layoutSizeHint(which, constraint);
}

void DuiLinearLayout::addItemsToLayout(QList<DuiMultiLayoutItem *> const  &items)
{
    gDuiLinearLayoutStub->addItemsToLayout(items);
}

void DuiLinearLayout::updateStyle()
{
    gDuiLinearLayoutStub->updateStyle();
}

QList<DuiMultiLayoutItem::LayoutParameter> DuiLinearLayout::layoutParameters()
{
    return gDuiLinearLayoutStub->layoutParameters();
}

DuiLinearLayout::DuiLinearLayout(const DuiLinearLayout &):
    DuiAbstractLayout(),
    QGraphicsLinearLayout(),
    d_ptr(0)
{
}

DuiLinearLayout &DuiLinearLayout::operator=(const DuiLinearLayout &)
{
    static DuiLinearLayout bla(Qt::Horizontal);
    return bla;
}


#endif
