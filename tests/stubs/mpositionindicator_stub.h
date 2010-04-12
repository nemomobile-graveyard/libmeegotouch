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

#ifndef MPOSITIONINDICATOR_STUB_H
#define MPOSITIONINDICATOR_STUB_H

#include <stubbase.h>
#include <mpositionindicator.h>
class MPositionIndicatorStub
    : public StubBase
{
public:
    virtual void mPositionIndicatorConstructor(Qt::Orientations panningDirection, MWidget *parent);
    virtual void mPositionIndicatorDestructor();
    virtual void setView(MWidgetView *view);

    virtual void updatePosData(const QPointF &);
    virtual void updateSizeData(const QSizeF &, const QSizeF &);
    virtual void show();
    virtual void hide();
    virtual QSizeF pannableViewportSize() const;
};

void MPositionIndicatorStub::mPositionIndicatorConstructor(Qt::Orientations panningDirection, MWidget *parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(panningDirection);
}

void MPositionIndicatorStub::mPositionIndicatorDestructor()
{
}

void MPositionIndicatorStub::setView(MWidgetView *view)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidgetView *>(view));
    stubMethodEntered("setView", params);
    Q_UNUSED(view);
}
void MPositionIndicatorStub::updatePosData(const QPointF &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QPointF & >(pos));
    stubMethodEntered("updatePosData", params);

}
void MPositionIndicatorStub::updateSizeData(const QSizeF &pannedWidgetSize, const QSizeF &pannableViewportSize)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QSizeF &>(pannedWidgetSize));
    params.append(new Parameter<const QSizeF &>(pannableViewportSize));
    stubMethodEntered("updateSizeData", params);
}
void MPositionIndicatorStub::show()
{
    stubMethodEntered("show");
}
void MPositionIndicatorStub::hide()
{
    stubMethodEntered("hide");
}

QSizeF MPositionIndicatorStub::pannableViewportSize() const
{
    stubMethodEntered("pannableViewportSize");
    return stubReturnValue<QSizeF>("pannableViewportSize");
}

MPositionIndicatorStub gDefaultMPositionIndicatorStub;
MPositionIndicatorStub *gMPositionIndicatorStub = &gDefaultMPositionIndicatorStub;

MPositionIndicator::MPositionIndicator(Qt::Orientations panningDirection, MWidget *parent) : d_ptr(0)
{
    gMPositionIndicatorStub->mPositionIndicatorConstructor(panningDirection, parent);
}

MPositionIndicator::~MPositionIndicator()
{
    gMPositionIndicatorStub->mPositionIndicatorDestructor();
}

void MPositionIndicator::setView(MWidgetView *view)
{
    gMPositionIndicatorStub->setView(view);
}
void MPositionIndicator::show()
{
    gMPositionIndicatorStub->show();
}

void MPositionIndicator::hide()
{
    gMPositionIndicatorStub->hide();
}

QSizeF MPositionIndicator::pannableViewportSize() const
{
    return gMPositionIndicatorStub->pannableViewportSize();
}

#endif
