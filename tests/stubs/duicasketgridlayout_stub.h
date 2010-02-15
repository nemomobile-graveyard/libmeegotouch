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

#ifndef DUICASKETGRIDLAYOUT_STUB
#define DUICASKETGRIDLAYOUT_STUB

#include "duicasketgridlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiCasketGridLayoutStub : public StubBase
{
public:
    virtual void DuiCasketGridLayoutConstructor(QGraphicsLayoutItem *parent);
    virtual void DuiCasketGridLayoutDestructor();
    virtual void insertAt(QGraphicsLayoutItem *item, int index);
    virtual void insertAt(QGraphicsLayoutItem *item, int row, int column);
    virtual QGraphicsLayoutItem *removeAndCompressAt(int index);
    virtual QGraphicsLayoutItem *removeAndCompressAt(int row, int column);
    virtual QGraphicsLayoutItem *replaceAt(int index, QGraphicsLayoutItem *item);
    virtual QGraphicsLayoutItem *replaceAt(int row, int column, QGraphicsLayoutItem *item);
    virtual void moveItem(int from, int to);
    virtual void moveItem(int fromRow, int fromColumn, int toRow, int toColumn);
    virtual void setColumnCount(int count);
    virtual void setRowCount(int count);
    virtual void DuiCasketGridLayoutConstructor(const DuiCasketGridLayout &);
    virtual void updateStyle();
    virtual void addItemsToLayout(QList <DuiMultiLayoutItem * > const &items);
    virtual QList<DuiMultiLayoutItem::LayoutParameter> layoutParameters();
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

};

// 2. IMPLEMENT STUB
void DuiCasketGridLayoutStub::DuiCasketGridLayoutConstructor(QGraphicsLayoutItem *parent)
{
    Q_UNUSED(parent);

}
void DuiCasketGridLayoutStub::DuiCasketGridLayoutDestructor()
{

}

void DuiCasketGridLayoutStub::insertAt(QGraphicsLayoutItem *item, int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    params.append(new Parameter<int >(index));
    stubMethodEntered("insertAt", params);
}

void DuiCasketGridLayoutStub::insertAt(QGraphicsLayoutItem *item, int row, int column)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    stubMethodEntered("insertAt", params);
}

QGraphicsLayoutItem *DuiCasketGridLayoutStub::removeAndCompressAt(int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("removeAndCompressAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("removeAndCompressAt");
}

QGraphicsLayoutItem *DuiCasketGridLayoutStub::removeAndCompressAt(int row, int column)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    stubMethodEntered("removeAndCompressAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("removeAndCompressAt");
}

QGraphicsLayoutItem *DuiCasketGridLayoutStub::replaceAt(int index, QGraphicsLayoutItem *item)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    stubMethodEntered("replaceAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("replaceAt");
}

QGraphicsLayoutItem *DuiCasketGridLayoutStub::replaceAt(int row, int column, QGraphicsLayoutItem *item)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    stubMethodEntered("replaceAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("replaceAt");
}

void DuiCasketGridLayoutStub::moveItem(int from, int to)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(from));
    params.append(new Parameter<int >(to));
    stubMethodEntered("moveItem", params);
}

void DuiCasketGridLayoutStub::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(fromRow));
    params.append(new Parameter<int >(fromColumn));
    params.append(new Parameter<int >(toRow));
    params.append(new Parameter<int >(toColumn));
    stubMethodEntered("moveItem", params);
}

void DuiCasketGridLayoutStub::setColumnCount(int count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(count));
    stubMethodEntered("setColumnCount", params);
}

void DuiCasketGridLayoutStub::setRowCount(int count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(count));
    stubMethodEntered("setRowCount", params);
}



void DuiCasketGridLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void DuiCasketGridLayoutStub::addItemsToLayout(QList <DuiMultiLayoutItem * > const &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QList <DuiMultiLayoutItem * > >(items));
    stubMethodEntered("addItemsToLayout", params);
}

QList<DuiMultiLayoutItem::LayoutParameter> DuiCasketGridLayoutStub::layoutParameters()
{
    stubMethodEntered("layoutParameters");
    return stubReturnValue<QList<DuiMultiLayoutItem::LayoutParameter> >(QString("layoutParameters"));
}

QSizeF DuiCasketGridLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>(QString("layoutSizeHint"));
}

void DuiCasketGridLayoutStub::DuiCasketGridLayoutConstructor(const DuiCasketGridLayout &)
{

}

// 3. CREATE A STUB INSTANCE
DuiCasketGridLayoutStub gDefaultDuiCasketGridLayoutStub;
DuiCasketGridLayoutStub *gDuiCasketGridLayoutStub = &gDefaultDuiCasketGridLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiCasketGridLayout::DuiCasketGridLayout(QGraphicsLayoutItem *parent):
    DuiAbstractLayout(),
    QGraphicsGridLayout(),
    d_ptr(0)
{
    gDuiCasketGridLayoutStub->DuiCasketGridLayoutConstructor(parent);
}

DuiCasketGridLayout::~DuiCasketGridLayout()
{
    gDuiCasketGridLayoutStub->DuiCasketGridLayoutDestructor();
}

void DuiCasketGridLayout::insertAt(QGraphicsLayoutItem *item, int index)
{
    gDuiCasketGridLayoutStub->insertAt(item, index);
}

void DuiCasketGridLayout::insertAt(QGraphicsLayoutItem *item, int row, int column)
{
    gDuiCasketGridLayoutStub->insertAt(item, row, column);
}

QGraphicsLayoutItem *DuiCasketGridLayout::removeAndCompressAt(int index)
{
    return gDuiCasketGridLayoutStub->removeAndCompressAt(index);
}

QGraphicsLayoutItem *DuiCasketGridLayout::removeAndCompressAt(int row, int column)
{
    return gDuiCasketGridLayoutStub->removeAndCompressAt(row, column);
}

QGraphicsLayoutItem *DuiCasketGridLayout::replaceAt(int index, QGraphicsLayoutItem *item)
{
    return gDuiCasketGridLayoutStub->replaceAt(index, item);
}

QGraphicsLayoutItem *DuiCasketGridLayout::replaceAt(int row, int column, QGraphicsLayoutItem *item)
{
    return gDuiCasketGridLayoutStub->replaceAt(row, column, item);
}

void DuiCasketGridLayout::moveItem(int from, int to)
{
    gDuiCasketGridLayoutStub->moveItem(from, to);
}

void DuiCasketGridLayout::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
    gDuiCasketGridLayoutStub->moveItem(fromRow, fromColumn, toRow, toColumn);
}

void DuiCasketGridLayout::setColumnCount(int count)
{
    gDuiCasketGridLayoutStub->setColumnCount(count);
}

void DuiCasketGridLayout::setRowCount(int count)
{
    gDuiCasketGridLayoutStub->setRowCount(count);
}


void DuiCasketGridLayout::updateStyle()
{
    gDuiCasketGridLayoutStub->updateStyle();
}

void DuiCasketGridLayout::addItemsToLayout(QList <DuiMultiLayoutItem * > const &items)
{
    gDuiCasketGridLayoutStub->addItemsToLayout(items);
}

QList<DuiMultiLayoutItem::LayoutParameter> DuiCasketGridLayout::layoutParameters()
{
    return gDuiCasketGridLayoutStub->layoutParameters();
}

QSizeF DuiCasketGridLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDuiCasketGridLayoutStub->layoutSizeHint(which, constraint);

}


DuiCasketGridLayout::DuiCasketGridLayout(const DuiCasketGridLayout &) :
    DuiAbstractLayout(),
    QGraphicsGridLayout(),
    d_ptr(0)
{
}

DuiCasketGridLayout &DuiCasketGridLayout::operator=(const DuiCasketGridLayout &)
{
    static DuiCasketGridLayout bla;
    return bla;
}


#endif
