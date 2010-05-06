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

#ifndef MTEXTEDITVIEW_STUB_H
#define MTEXTEDITVIEW_STUB_H

#include <mtexteditview.h>
#include <stubbase.h>

// 1. DECLARE STUB
class MTextEditViewStub : public StubBase
{
public:
    virtual void mTextEditViewConstructor(MTextEdit *controller);
    virtual void mTextEditViewDestructor();
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
void MTextEditViewStub::mTextEditViewConstructor(MTextEdit *controller)
{
    Q_UNUSED(controller);
}

void MTextEditViewStub::mTextEditViewDestructor()
{
}

void MTextEditViewStub::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter *>(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem *>(option));
    params.append(new Parameter<QWidget *>(widget));
    stubMethodEntered("paint", params);
}

QRectF MTextEditViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void MTextEditViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QRectF>(rect));
    stubMethodEntered("setGeometry", params);
}

void MTextEditViewStub::setFocused(bool focused)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(focused));
    stubMethodEntered("setFocused", params);
}


void MTextEditViewStub::updateData(const int index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const int>(index));
    stubMethodEntered("modelUpdated", params);
}

void MTextEditViewStub::styleUpdated()
{
    stubMethodEntered("updateStyle");
}

void MTextEditViewStub::updateGeometry()
{
    stubMethodEntered("updateGeometry");
}

// 3. CREATE A STUB INSTANCE
MTextEditViewStub gDefaultMTextEditViewStub;
MTextEditViewStub *gMTextEditViewStub = &gDefaultMTextEditViewStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
MTextEditView::MTextEditView(MTextEdit *controller)
{
    gMTextEditViewStub->mTextEditViewConstructor(controller);
}

MTextEditView::~MTextEditView()
{
    gMTextEditViewStub->mTextEditViewDestructor();
}

void MTextEditView::paint(QPainter *painter,
                            const QStyleOptionGraphicsItem *option,
                            QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

QRectF MTextEditView::boundingRect() const
{
    return gMTextEditViewStub->boundingRect();
}

void MTextEditView::setGeometry(const QRectF &rect)
{
    gMTextEditViewStub->setGeometry(rect);
}

void MTextEditView::setFocused(bool focused)
{
    gMTextEditViewStub->setFocused(focused);
}

void MTextEditView::updateData(const int index)
{
    gMTextEditViewStub->updateData(index);
}

void MTextEditView::styleUpdated()
{
    gMTextEditViewStub->styleUpdated();
}

void MTextEditView::updateGeometry()
{
    gMTextEditViewStub->updateGeometry();
}


#endif
