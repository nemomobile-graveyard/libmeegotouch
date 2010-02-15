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

#ifndef DUITEXTEDITVIEW_STUB_H
#define DUITEXTEDITVIEW_STUB_H

#include <duitexteditview.h>
#include <stubbase.h>

// 1. DECLARE STUB
class DuiTextEditViewStub : public StubBase
{
public:
    virtual void duiTextEditViewConstructor(DuiTextEdit *controller);
    virtual void duiTextEditViewDestructor();
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0);
    virtual QRectF boundingRect() const;
    virtual void setGeometry(const QRectF &rect);
    virtual void setFocused(bool focused);
    virtual void updateData(const int index);
    virtual void styleUpdated();
    virtual void updateGeometry();
};


// 2. IMPLEMENT STUB
void DuiTextEditViewStub::duiTextEditViewConstructor(DuiTextEdit *controller)
{
    Q_UNUSED(controller);
}

void DuiTextEditViewStub::duiTextEditViewDestructor()
{
}

void DuiTextEditViewStub::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter *>(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem *>(option));
    params.append(new Parameter<QWidget *>(widget));
    stubMethodEntered("paint", params);
}

QRectF DuiTextEditViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void DuiTextEditViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QRectF>(rect));
    stubMethodEntered("setGeometry", params);
}

void DuiTextEditViewStub::setFocused(bool focused)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(focused));
    stubMethodEntered("setFocused", params);
}


void DuiTextEditViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int>(index));
    stubMethodEntered("modelUpdated", params);
}

void DuiTextEditViewStub::styleUpdated()
{
    stubMethodEntered("updateStyle");
}

void DuiTextEditViewStub::updateGeometry()
{
    stubMethodEntered("updateGeometry");
}

// 3. CREATE A STUB INSTANCE
DuiTextEditViewStub gDefaultDuiTextEditViewStub;
DuiTextEditViewStub *gDuiTextEditViewStub = &gDefaultDuiTextEditViewStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiTextEditView::DuiTextEditView(DuiTextEdit *controller)
{
    gDuiTextEditViewStub->duiTextEditViewConstructor(controller);
}

DuiTextEditView::~DuiTextEditView()
{
    gDuiTextEditViewStub->duiTextEditViewDestructor();
}

void DuiTextEditView::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF DuiTextEditView::boundingRect() const
{
    return gDuiTextEditViewStub->boundingRect();
}

void DuiTextEditView::setGeometry(const QRectF &rect)
{
    gDuiTextEditViewStub->setGeometry(rect);
}

void DuiTextEditView::setFocused(bool focused)
{
    gDuiTextEditViewStub->setFocused(focused);
}

void DuiTextEditView::updateData(const int index)
{
    gDuiTextEditViewStub->updateData(index);
}

void DuiTextEditView::styleUpdated()
{
    gDuiTextEditViewStub->styleUpdated();
}

void DuiTextEditView::updateGeometry()
{
    gDuiTextEditViewStub->updateGeometry();
}


#endif
