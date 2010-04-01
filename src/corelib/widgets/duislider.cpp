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

#include <QString>
#include <QSettings>
#include "duislider.h"
#include "duiseekbar.h"
#include "duislider_p.h"
#include "duitheme.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiSlider)

DuiSliderPrivate::DuiSliderPrivate()
{
}


DuiSliderPrivate::~DuiSliderPrivate()
{
}

int DuiSliderPrivate::adjustValue(int minimum, int value, int maximum, int steps)
{
    if (steps < 0)
        steps = -steps;

    if ((steps < 1) || (steps > maximum - minimum - 1))
        return value;

    int numberOfSteps = qRound(((qreal(value) - qreal(minimum)) * qreal(steps)) / (qreal(maximum) - qreal(minimum)));
    return minimum + (numberOfSteps * qRound((qreal(maximum) - qreal(minimum)) / qreal(steps)));
}

DuiSlider::DuiSlider(QGraphicsItem *parent, const QString &viewType)
    : DuiWidgetController(new DuiSliderPrivate(), new DuiSliderModel, parent)
{
    setViewType(viewType);
}

DuiSlider::DuiSlider(DuiSliderPrivate *dd, DuiSliderModel *model, QGraphicsItem *parent, const QString &viewType)
    : DuiWidgetController(dd, model, parent)
{
    setViewType(viewType);
}

DuiSlider::~DuiSlider()
{
}

void DuiSlider::setMinimum(int minimum)
{
    model()->setMinimum(minimum);
}

void DuiSlider::setMaximum(int maximum)
{
    model()->setMaximum(maximum);
}

void DuiSlider::setRange(int minimum, int maximum)
{
    setMaximum(maximum);
    setMinimum(minimum);
}

void DuiSlider::setSteps(int steps)
{
    Q_D(DuiSlider);

    model()->setSteps(steps);

    int tmpValue = d->adjustValue(model()->minimum(),
                                  model()->value(),
                                  model()->maximum(),
                                  model()->steps());

    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void DuiSlider::setValue(int value)
{
    Q_D(DuiSlider);
    int tmpValue = d->adjustValue(model()->minimum(),
                                  value,
                                  model()->maximum(),
                                  model()->steps());
    model()->setValue(tmpValue);
}

void DuiSlider::updateData(const QList<const char *>& modifications)
{
    DuiWidgetController::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == DuiSliderModel::Value) {
            emit valueChanged(model()->value());
        } else if (member == DuiSliderModel::State) {
            if (model()->state() == DuiSliderModel::Pressed)
                emit sliderPressed();
            else
                emit sliderReleased();
        }
    }
}

int DuiSlider::minimum() const
{
    return model()->minimum();
}

int DuiSlider::maximum() const
{
    return model()->maximum();
}

int DuiSlider::steps() const
{
    return model()->steps();
}

int DuiSlider::value() const
{
    return model()->value();
}

void DuiSlider::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

Qt::Orientation DuiSlider::orientation() const
{
    return model()->orientation();
}

void DuiSlider::setMinLabelIconID(const QString &iconID)
{
    model()->setMinLabelIcon(iconID);
}

void DuiSlider::setMinLabelVisible(bool visible)
{
    model()->setMinLabelVisible(visible);
}

void DuiSlider::setMinLabel(const QString &text)
{
    model()->setMinLabelText(text);
}

void DuiSlider::setMaxLabelIconID(const QString &iconID)
{
    model()->setMaxLabelIcon(iconID);
}

void DuiSlider::setMaxLabelVisible(bool visible)
{
    model()->setMaxLabelVisible(visible);
}

void DuiSlider::setMaxLabel(const QString &text)
{
    model()->setMaxLabelText(text);
}

void DuiSlider::setHandleLabelIconID(const QString &iconID)
{
    model()->setHandleLabelIcon(iconID);
}

void DuiSlider::setHandleLabelVisible(bool visible)
{
    model()->setHandleLabelVisible(visible);
}

void DuiSlider::setHandleLabel(const QString &text)
{
    model()->setHandleLabelText(text);
}

QString DuiSlider::minLabelIconID() const
{
    return model()->minLabelIcon();
}

QString DuiSlider::minLabelText() const
{
    return model()->minLabelText();
}

QString DuiSlider::maxLabelIconID() const
{
    return model()->maxLabelIcon();
}

QString DuiSlider::maxLabelText() const
{
    return model()->maxLabelText();
}

QString DuiSlider::handleLabelIconID() const
{
    return model()->handleLabelIcon();
}

QString DuiSlider::handleLabelText() const
{
    return model()->handleLabelText();
}

bool DuiSlider::isMinLabelVisible() const
{
    return model()->minLabelVisible();
}

bool DuiSlider::isMaxLabelVisible() const
{
    return model()->maxLabelVisible();
}

bool DuiSlider::isHandleLabelVisible() const
{
    return model()->handleLabelVisible();
}

void DuiSlider::setState(DuiSliderModel::SliderState state)
{
    model()->setState(state);
    update();
}

DuiSliderModel::SliderState DuiSlider::state() const
{
    return model()->state();
}
