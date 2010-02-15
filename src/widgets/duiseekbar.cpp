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

#include "duiseekbar.h"
#include "duislider_p.h"
#include "duitheme.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiSeekBar)

DuiSeekBar::DuiSeekBar(QGraphicsItem *parent, DuiSeekBarModel *model) :
    DuiSlider(new DuiSliderPrivate, model == NULL ? new DuiSeekBarModel : model, parent)
{
}

// Sets the minimum of the active area. Does not have to depend on actual range.
void DuiSeekBar::setLoadedContentMinimum(int minimum)
{
    int mMinimum = model()->loadedContentMin();
    int mMaximum = model()->loadedContentMax();

    // the maximum will be at least the minimum
    if (minimum > mMaximum) {
        mMaximum = minimum;
        model()->setLoadedContentMax(mMaximum);
    }

    if (minimum != mMinimum) {
        mMinimum = minimum;
        model()->setLoadedContentMin(mMinimum);
    }

    if ((model()->value() < model()->loadedContentMin()) ||
            (model()->value() > model()->loadedContentMax()))
        emit outOfLoadedContentRange();
}

// Sets the maximum of the active area. Does not have to depend on actual range.
void DuiSeekBar::setLoadedContentMaximum(int maximum)
{
    int mMinimum = model()->loadedContentMin();
    int mMaximum = model()->loadedContentMax();

    // the minimum will be at most the minimum
    if (maximum < mMinimum) {
        mMinimum = maximum;
        model()->setLoadedContentMin(mMinimum);
    }

    if (maximum != mMaximum) {
        mMaximum = maximum;
        model()->setLoadedContentMax(mMaximum);
    }

    if ((model()->value() < model()->loadedContentMin()) ||
            (model()->value() > model()->loadedContentMax()))
        emit outOfLoadedContentRange();
}

void DuiSeekBar::setLoadedContentRange(int minimum, int maximum)
{
    setLoadedContentMaximum(maximum);
    // If maximum < minimum, both will be set to minimum
    setLoadedContentMinimum(minimum);

    if ((model()->value() < model()->loadedContentMin()) ||
            (model()->value() > model()->loadedContentMax()))
        emit outOfLoadedContentRange();
}

void DuiSeekBar::setValue(int value)
{
    DuiSlider::setValue(value);

    if ((value < model()->loadedContentMin()) ||
            (value > model()->loadedContentMax()))
        emit outOfLoadedContentRange();
}

int DuiSeekBar::loadedContentMinimum() const
{
    return model()->loadedContentMin();
}

int DuiSeekBar::loadedContentMaximum() const
{
    return model()->loadedContentMax();
}
