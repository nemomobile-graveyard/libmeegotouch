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

#ifndef MCASKETGRIDLAYOUT_STUB
#define MCASKETGRIDLAYOUT_STUB

#include "mcasketgridlayout.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MCasketGridLayoutStub : public StubBase
{
public:
    virtual void MCasketGridLayoutConstructor(QGraphicsLayoutItem *parent);
    virtual void MCasketGridLayoutDestructor();
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
    virtual void MCasketGridLayoutConstructor(const MCasketGridLayout &);
    virtual void updateStyle();
    virtual void addItemsToLayout(QList <MMultiLayoutItem * > const &items);
    virtual QList<MMultiLayoutItem::LayoutParameter> layoutParameters();
    virtual QSizeF layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const;

};

// 2. IMPLEMENT STUB
void MCasketGridLayoutStub::MCasketGridLayoutConstructor(QGraphicsLayoutItem *parent)
{
    Q_UNUSED(parent);

}
void MCasketGridLayoutStub::MCasketGridLayoutDestructor()
{

}

void MCasketGridLayoutStub::insertAt(QGraphicsLayoutItem *item, int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    params.append(new Parameter<int >(index));
    stubMethodEntered("insertAt", params);
}

void MCasketGridLayoutStub::insertAt(QGraphicsLayoutItem *item, int row, int column)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    stubMethodEntered("insertAt", params);
}

QGraphicsLayoutItem *MCasketGridLayoutStub::removeAndCompressAt(int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    stubMethodEntered("removeAndCompressAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("removeAndCompressAt");
}

QGraphicsLayoutItem *MCasketGridLayoutStub::removeAndCompressAt(int row, int column)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    stubMethodEntered("removeAndCompressAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("removeAndCompressAt");
}

QGraphicsLayoutItem *MCasketGridLayoutStub::replaceAt(int index, QGraphicsLayoutItem *item)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(index));
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    stubMethodEntered("replaceAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("replaceAt");
}

QGraphicsLayoutItem *MCasketGridLayoutStub::replaceAt(int row, int column, QGraphicsLayoutItem *item)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(row));
    params.append(new Parameter<int >(column));
    params.append(new Parameter<QGraphicsLayoutItem * >(item));
    stubMethodEntered("replaceAt", params);
    return stubReturnValue<QGraphicsLayoutItem *>("replaceAt");
}

void MCasketGridLayoutStub::moveItem(int from, int to)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(from));
    params.append(new Parameter<int >(to));
    stubMethodEntered("moveItem", params);
}

void MCasketGridLayoutStub::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(fromRow));
    params.append(new Parameter<int >(fromColumn));
    params.append(new Parameter<int >(toRow));
    params.append(new Parameter<int >(toColumn));
    stubMethodEntered("moveItem", params);
}

void MCasketGridLayoutStub::setColumnCount(int count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(count));
    stubMethodEntered("setColumnCount", params);
}

void MCasketGridLayoutStub::setRowCount(int count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<int >(count));
    stubMethodEntered("setRowCount", params);
}



void MCasketGridLayoutStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void MCasketGridLayoutStub::addItemsToLayout(QList <MMultiLayoutItem * > const &items)
{
    QList<ParameterBase *> params;
    params.append(new Parameter< QList <MMultiLayoutItem * > >(items));
    stubMethodEntered("addItemsToLayout", params);
}

QList<MMultiLayoutItem::LayoutParameter> MCasketGridLayoutStub::layoutParameters()
{
    stubMethodEntered("layoutParameters");
    return stubReturnValue<QList<MMultiLayoutItem::LayoutParameter> >(QString("layoutParameters"));
}

QSizeF MCasketGridLayoutStub::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("layoutSizeHint", params);
    return stubReturnValue<QSizeF>(QString("layoutSizeHint"));
}

void MCasketGridLayoutStub::MCasketGridLayoutConstructor(const MCasketGridLayout &)
{

}

// 3. CREATE A STUB INSTANCE
MCasketGridLayoutStub gDefaultMCasketGridLayoutStub;
MCasketGridLayoutStub *gMCasketGridLayoutStub = &gDefaultMCasketGridLayoutStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MCasketGridLayout::MCasketGridLayout(QGraphicsLayoutItem *parent):
    MAbstractLayout(),
    QGraphicsGridLayout(),
    d_ptr(0)
{
    gMCasketGridLayoutStub->MCasketGridLayoutConstructor(parent);
}

MCasketGridLayout::~MCasketGridLayout()
{
    gMCasketGridLayoutStub->MCasketGridLayoutDestructor();
}

void MCasketGridLayout::insertAt(QGraphicsLayoutItem *item, int index)
{
    gMCasketGridLayoutStub->insertAt(item, index);
}

void MCasketGridLayout::insertAt(QGraphicsLayoutItem *item, int row, int column)
{
    gMCasketGridLayoutStub->insertAt(item, row, column);
}

QGraphicsLayoutItem *MCasketGridLayout::removeAndCompressAt(int index)
{
    return gMCasketGridLayoutStub->removeAndCompressAt(index);
}

QGraphicsLayoutItem *MCasketGridLayout::removeAndCompressAt(int row, int column)
{
    return gMCasketGridLayoutStub->removeAndCompressAt(row, column);
}

QGraphicsLayoutItem *MCasketGridLayout::replaceAt(int index, QGraphicsLayoutItem *item)
{
    return gMCasketGridLayoutStub->replaceAt(index, item);
}

QGraphicsLayoutItem *MCasketGridLayout::replaceAt(int row, int column, QGraphicsLayoutItem *item)
{
    return gMCasketGridLayoutStub->replaceAt(row, column, item);
}

void MCasketGridLayout::moveItem(int from, int to)
{
    gMCasketGridLayoutStub->moveItem(from, to);
}

void MCasketGridLayout::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
    gMCasketGridLayoutStub->moveItem(fromRow, fromColumn, toRow, toColumn);
}

void MCasketGridLayout::setColumnCount(int count)
{
    gMCasketGridLayoutStub->setColumnCount(count);
}

void MCasketGridLayout::setRowCount(int count)
{
    gMCasketGridLayoutStub->setRowCount(count);
}


void MCasketGridLayout::updateStyle()
{
    gMCasketGridLayoutStub->updateStyle();
}

void MCasketGridLayout::addItemsToLayout(QList <MMultiLayoutItem * > const &items)
{
    gMCasketGridLayoutStub->addItemsToLayout(items);
}

QList<MMultiLayoutItem::LayoutParameter> MCasketGridLayout::layoutParameters()
{
    return gMCasketGridLayoutStub->layoutParameters();
}

QSizeF MCasketGridLayout::layoutSizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gMCasketGridLayoutStub->layoutSizeHint(which, constraint);

}


MCasketGridLayout::MCasketGridLayout(const MCasketGridLayout &) :
    MAbstractLayout(),
    QGraphicsGridLayout(),
    d_ptr(0)
{
}

MCasketGridLayout &MCasketGridLayout::operator=(const MCasketGridLayout &)
{
    static MCasketGridLayout bla;
    return bla;
}


#endif
