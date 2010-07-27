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

#include <mtheme.h>

#include "mprogressindicator.h"
#include "mprogressindicator_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MProgressIndicator)

const MTheme::ViewType MProgressIndicator::spinnerType  = "spinner";
const MTheme::ViewType MProgressIndicator::barType      = "bar";


MProgressIndicatorPrivate::MProgressIndicatorPrivate()
{
}

MProgressIndicatorPrivate::~MProgressIndicatorPrivate()
{
}

void MProgressIndicatorPrivate::init()
{
}

MProgressIndicator::MProgressIndicator(MProgressIndicatorPrivate *dd, MProgressIndicatorModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MProgressIndicator);

    d->init();
}

MProgressIndicator::MProgressIndicator(QGraphicsItem *parent, const QString &viewType)
    : MWidgetController(new MProgressIndicatorPrivate, new MProgressIndicatorModel, parent)
{
    setViewType(viewType);
    Q_D(MProgressIndicator);

    d->init();
}

MProgressIndicator::~MProgressIndicator()
{
}


void MProgressIndicator::reset()
{
    setValue(minimum());
    setUnknownDuration(false);
}


void MProgressIndicator::setRange(int minimum, int maximum)
{
    model()->setMinimum(minimum);
    model()->setMaximum(qMax(minimum, maximum));
    if (model()->value() < model()->minimum() || model()->value() > model()->maximum())
        reset();
}

void MProgressIndicator::setValue(int value)
{
    if (model()->value() == value)
        return;

    if ((value > model()->maximum()) || (value < model()->minimum()))
        return;

    model()->setValue(value);
    emit valueChanged(value);
}

int MProgressIndicator::value() const
{
    return model()->value();
}


void MProgressIndicator::setMinimum(int minimum)
{
    setRange(minimum, qMax(maximum(), minimum));
}


int MProgressIndicator::minimum() const
{
    return model()->minimum();
}


void MProgressIndicator::setMaximum(int maximum)
{
    setRange(qMin(minimum(), maximum), maximum);
}


int MProgressIndicator::maximum() const
{
    return model()->maximum();
}


void MProgressIndicator::setUnknownDuration(bool unknownDuration)
{
    model()->setUnknownDuration(unknownDuration);
}


bool MProgressIndicator::unknownDuration() const
{
    return model()->unknownDuration();
}

