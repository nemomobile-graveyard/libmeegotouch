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

#ifndef DUIPOSITIONINDICATOR_STUB_H
#define DUIPOSITIONINDICATOR_STUB_H

#include <stubbase.h>
#include <duipositionindicator.h>
class DuiPositionIndicatorStub
    : public StubBase
{
public:
    virtual void duiPositionIndicatorConstructor(Qt::Orientations panningDirection, DuiWidget *parent);
    virtual void duiPositionIndicatorDestructor();
    virtual void setView(DuiWidgetView *view);

    virtual void updatePosData(const QPointF &);
    virtual void updateSizeData(const QSizeF &, const QSizeF &);
    virtual void show();
    virtual void hide();
    virtual QSizeF pannableViewportSize() const;
};

void DuiPositionIndicatorStub::duiPositionIndicatorConstructor(Qt::Orientations panningDirection, DuiWidget *parent)
{
    Q_UNUSED(parent);
    Q_UNUSED(panningDirection);
}

void DuiPositionIndicatorStub::duiPositionIndicatorDestructor()
{
}

void DuiPositionIndicatorStub::setView(DuiWidgetView *view)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidgetView *>(view));
    stubMethodEntered("setView", params);
    Q_UNUSED(view);
}
void DuiPositionIndicatorStub::updatePosData(const QPointF &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QPointF & >(pos));
    stubMethodEntered("updatePosData", params);

}
void DuiPositionIndicatorStub::updateSizeData(const QSizeF &pannedWidgetSize, const QSizeF &pannableViewportSize)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QSizeF &>(pannedWidgetSize));
    params.append(new Parameter<const QSizeF &>(pannableViewportSize));
    stubMethodEntered("updateSizeData", params);
}
void DuiPositionIndicatorStub::show()
{
    stubMethodEntered("show");
}
void DuiPositionIndicatorStub::hide()
{
    stubMethodEntered("hide");
}

QSizeF DuiPositionIndicatorStub::pannableViewportSize() const
{
    stubMethodEntered("pannableViewportSize");
    return stubReturnValue<QSizeF>("pannableViewportSize");
}

DuiPositionIndicatorStub gDefaultDuiPositionIndicatorStub;
DuiPositionIndicatorStub *gDuiPositionIndicatorStub = &gDefaultDuiPositionIndicatorStub;

DuiPositionIndicator::DuiPositionIndicator(Qt::Orientations panningDirection, DuiWidget *parent) : d_ptr(0)
{
    gDuiPositionIndicatorStub->duiPositionIndicatorConstructor(panningDirection, parent);
}

DuiPositionIndicator::~DuiPositionIndicator()
{
    gDuiPositionIndicatorStub->duiPositionIndicatorDestructor();
}

void DuiPositionIndicator::setView(DuiWidgetView *view)
{
    gDuiPositionIndicatorStub->setView(view);
}
void DuiPositionIndicator::show()
{
    gDuiPositionIndicatorStub->show();
}

void DuiPositionIndicator::hide()
{
    gDuiPositionIndicatorStub->hide();
}

QSizeF DuiPositionIndicator::pannableViewportSize() const
{
    return gDuiPositionIndicatorStub->pannableViewportSize();
}

#endif
