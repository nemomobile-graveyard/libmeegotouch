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

#ifndef DuiMultiLayoutItem_STUB
#define DuiMultiLayoutItem_STUB

#include "DuiMultiLayoutItem.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiMultiLayoutItemStub : public StubBase
{
public:
    enum LayoutParameter { PosCoordinates = 0, PosIndex, ColumnSpanning, RowSpanning } ;
    virtual void DuiMultiLayoutItemConstructor();
    virtual void DuiMultiLayoutItemDestructor();
    virtual bool belongsToLayout(QString const  &layoutName) const;
    virtual void assignToLayout(QString const  &layoutName, QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter);
    virtual void assignToLayout(QString const  &layoutName);
    virtual QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> layoutParameters(QString const  &layoutName) const;
    virtual void setWidget(DuiWidget *widget);
    virtual void DuiMultiLayoutItemConstructor(const DuiMultiLayoutItem &);
    DuiMultiLayoutItemPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiMultiLayoutItemStub::DuiMultiLayoutItemConstructor()
{

}
void DuiMultiLayoutItemStub::DuiMultiLayoutItemDestructor()
{

}
bool DuiMultiLayoutItemStub::belongsToLayout(QString const  &layoutName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("belongsToLayout", params);
    return stubReturnValue<bool>("belongsToLayout");
}

void DuiMultiLayoutItemStub::assignToLayout(QString const  &layoutName, QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    params.append(new Parameter<QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> const & >(layoutParameter));
    stubMethodEntered("assignToLayout", params);
}

void DuiMultiLayoutItemStub::assignToLayout(QString const  &layoutName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("assignToLayout", params);
}

QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> DuiMultiLayoutItemStub::layoutParameters(QString const  &layoutName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("layoutParameters", params);
    return stubReturnValue<QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> >("layoutParameters");
}

void DuiMultiLayoutItemStub::setWidget(DuiWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("setWidget", params);
}

void DuiMultiLayoutItemStub::DuiMultiLayoutItemConstructor(const DuiMultiLayoutItem &)
{

}


// 3. CREATE A STUB INSTANCE
DuiMultiLayoutItemStub gDefaultDuiMultiLayoutItemStub;
DuiMultiLayoutItemStub *gDuiMultiLayoutItemStub = &gDefaultDuiMultiLayoutItemStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiMultiLayoutItem::DuiMultiLayoutItem()
{
    gDuiMultiLayoutItemStub->DuiMultiLayoutItemConstructor();
}

DuiMultiLayoutItem::~DuiMultiLayoutItem()
{
    gDuiMultiLayoutItemStub->DuiMultiLayoutItemDestructor();
}

bool DuiMultiLayoutItem::belongsToLayout(QString const  &layoutName) const
{
    return gDuiMultiLayoutItemStub->belongsToLayout(layoutName);
}

void DuiMultiLayoutItem::assignToLayout(QString const  &layoutName, QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter)
{
    gDuiMultiLayoutItemStub->assignToLayout(layoutName, layoutParameter);
}

void DuiMultiLayoutItem::assignToLayout(QString const  &layoutName)
{
    gDuiMultiLayoutItemStub->assignToLayout(layoutName);
}

QMap<DuiMultiLayoutItem::LayoutParameter, QVariant> DuiMultiLayoutItem::layoutParameters(QString const  &layoutName) const
{
    return gDuiMultiLayoutItemStub->layoutParameters(layoutName);
}

void DuiMultiLayoutItem::setWidget(DuiWidget *widget)
{
    gDuiMultiLayoutItemStub->setWidget(widget);
}

DuiMultiLayoutItem::DuiMultiLayoutItem(const DuiMultiLayoutItem &)
{
    gDuiMultiLayoutItemStub->DuiMultiLayoutItemConstructor();
}


#endif
