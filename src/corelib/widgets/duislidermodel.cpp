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

#include "duislidermodel.h"

const int &DuiSliderModel::minimum() const
{
    return _minimum();
}

void DuiSliderModel::setMinimum(const int &minimum)
{
    //update the value and emit change signal only if the value has changed
    if (minimum != _minimum()) {
        _minimum() = minimum;
        memberModified(Minimum);
    }

    // the maximum will be at least the minimum
    if (minimum > _maximum()) {
        setMaximum(minimum);
    }

    //limit the value to new min and max
    setValue(value());
}

const int &DuiSliderModel::maximum() const
{
    return _maximum();
}

void DuiSliderModel::setMaximum(const int &maximum)
{
    //update the value and emit change signal only if the value has changed
    if (maximum != _maximum()) {
        _maximum() = maximum;
        memberModified(Maximum);
    }

    // the minimum will be at most the maximum
    if (maximum < _minimum()) {
        setMinimum(maximum);
    }

    //limit the value to new min and max
    setValue(value());
}

const int &DuiSliderModel::value() const
{
    return _value();
}

void DuiSliderModel::setValue(const int &value)
{
    int setVal = value;
    if (setVal < _minimum()) {
        setVal = _minimum();
    }

    if (setVal > _maximum()) {
        setVal = _maximum();
    }

    if (setVal != _value()) {
        _value() = setVal;
        memberModified(Value);
    }
}

const int &DuiSliderModel::steps() const
{
    return _steps();
}

void DuiSliderModel::setSteps(const int &steps)
{
    int setSteps = steps;
    if (setSteps < 0)
        setSteps = -steps;

    if (setSteps != _steps()) {
        _steps() = setSteps;
        memberModified(Steps);
    }
}
