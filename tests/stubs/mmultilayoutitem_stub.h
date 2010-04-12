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

#ifndef MMultiLayoutItem_STUB
#define MMultiLayoutItem_STUB

#include "MMultiLayoutItem.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MMultiLayoutItemStub : public StubBase
{
public:
    enum LayoutParameter { PosCoordinates = 0, PosIndex, ColumnSpanning, RowSpanning } ;
    virtual void MMultiLayoutItemConstructor();
    virtual void MMultiLayoutItemDestructor();
    virtual bool belongsToLayout(QString const  &layoutName) const;
    virtual void assignToLayout(QString const  &layoutName, QMap<MMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter);
    virtual void assignToLayout(QString const  &layoutName);
    virtual QMap<MMultiLayoutItem::LayoutParameter, QVariant> layoutParameters(QString const  &layoutName) const;
    virtual void setWidget(MWidget *widget);
    virtual void MMultiLayoutItemConstructor(const MMultiLayoutItem &);
    MMultiLayoutItemPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MMultiLayoutItemStub::MMultiLayoutItemConstructor()
{

}
void MMultiLayoutItemStub::MMultiLayoutItemDestructor()
{

}
bool MMultiLayoutItemStub::belongsToLayout(QString const  &layoutName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("belongsToLayout", params);
    return stubReturnValue<bool>("belongsToLayout");
}

void MMultiLayoutItemStub::assignToLayout(QString const  &layoutName, QMap<MMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    params.append(new Parameter<QMap<MMultiLayoutItem::LayoutParameter, QVariant> const & >(layoutParameter));
    stubMethodEntered("assignToLayout", params);
}

void MMultiLayoutItemStub::assignToLayout(QString const  &layoutName)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("assignToLayout", params);
}

QMap<MMultiLayoutItem::LayoutParameter, QVariant> MMultiLayoutItemStub::layoutParameters(QString const  &layoutName) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString const & >(layoutName));
    stubMethodEntered("layoutParameters", params);
    return stubReturnValue<QMap<MMultiLayoutItem::LayoutParameter, QVariant> >("layoutParameters");
}

void MMultiLayoutItemStub::setWidget(MWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    stubMethodEntered("setWidget", params);
}

void MMultiLayoutItemStub::MMultiLayoutItemConstructor(const MMultiLayoutItem &)
{

}


// 3. CREATE A STUB INSTANCE
MMultiLayoutItemStub gDefaultMMultiLayoutItemStub;
MMultiLayoutItemStub *gMMultiLayoutItemStub = &gDefaultMMultiLayoutItemStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MMultiLayoutItem::MMultiLayoutItem()
{
    gMMultiLayoutItemStub->MMultiLayoutItemConstructor();
}

MMultiLayoutItem::~MMultiLayoutItem()
{
    gMMultiLayoutItemStub->MMultiLayoutItemDestructor();
}

bool MMultiLayoutItem::belongsToLayout(QString const  &layoutName) const
{
    return gMMultiLayoutItemStub->belongsToLayout(layoutName);
}

void MMultiLayoutItem::assignToLayout(QString const  &layoutName, QMap<MMultiLayoutItem::LayoutParameter, QVariant> const  &layoutParameter)
{
    gMMultiLayoutItemStub->assignToLayout(layoutName, layoutParameter);
}

void MMultiLayoutItem::assignToLayout(QString const  &layoutName)
{
    gMMultiLayoutItemStub->assignToLayout(layoutName);
}

QMap<MMultiLayoutItem::LayoutParameter, QVariant> MMultiLayoutItem::layoutParameters(QString const  &layoutName) const
{
    return gMMultiLayoutItemStub->layoutParameters(layoutName);
}

void MMultiLayoutItem::setWidget(MWidget *widget)
{
    gMMultiLayoutItemStub->setWidget(widget);
}

MMultiLayoutItem::MMultiLayoutItem(const MMultiLayoutItem &)
{
    gMMultiLayoutItemStub->MMultiLayoutItemConstructor();
}


#endif
