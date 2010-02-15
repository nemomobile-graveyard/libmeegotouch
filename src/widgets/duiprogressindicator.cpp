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

#include <duitheme.h>

#include "duiprogressindicator.h"
#include "duiprogressindicator_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiProgressIndicator)

const DuiTheme::ViewType DuiProgressIndicator::spinnerType  = "spinner";
const DuiTheme::ViewType DuiProgressIndicator::barType      = "bar";


DuiProgressIndicatorPrivate::DuiProgressIndicatorPrivate()
{
}

DuiProgressIndicatorPrivate::~DuiProgressIndicatorPrivate()
{
}

void DuiProgressIndicatorPrivate::init()
{
}

DuiProgressIndicator::DuiProgressIndicator(DuiProgressIndicatorPrivate *dd, DuiProgressIndicatorModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    Q_D(DuiProgressIndicator);

    d->init();
}

DuiProgressIndicator::DuiProgressIndicator(QGraphicsItem *parent, const QString &viewType)
    : DuiWidgetController(new DuiProgressIndicatorPrivate, new DuiProgressIndicatorModel, parent)
{
    setViewType(viewType);
    Q_D(DuiProgressIndicator);

    d->init();
}

DuiProgressIndicator::~DuiProgressIndicator()
{
}


void DuiProgressIndicator::reset()
{
    setValue(minimum());
    setUnknownDuration(false);
}


void DuiProgressIndicator::setRange(int minimum, int maximum)
{
    model()->setMinimum(minimum);
    model()->setMaximum(qMax(minimum, maximum));
    if (model()->value() < model()->minimum() || model()->value() > model()->maximum())
        reset();
}

void DuiProgressIndicator::setValue(int value)
{
    if (model()->value() == value)
        return;

    if ((value > model()->maximum()) || (value < model()->minimum()))
        return;

    model()->setValue(value);
    emit valueChanged(value);
}

int DuiProgressIndicator::value() const
{
    return model()->value();
}


void DuiProgressIndicator::setMinimum(int minimum)
{
    setRange(minimum, qMax(maximum(), minimum));
}


int DuiProgressIndicator::minimum() const
{
    return model()->minimum();
}


void DuiProgressIndicator::setMaximum(int maximum)
{
    setRange(qMin(minimum(), maximum), maximum);
}


int DuiProgressIndicator::maximum() const
{
    return model()->maximum();
}


void DuiProgressIndicator::setUnknownDuration(bool unknownDuration)
{
    model()->setUnknownDuration(unknownDuration);
}


bool DuiProgressIndicator::unknownDuration() const
{
    return model()->unknownDuration();
}



