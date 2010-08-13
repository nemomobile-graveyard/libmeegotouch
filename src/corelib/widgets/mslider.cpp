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

#include <QString>
#include <QSettings>
#include "mslider.h"
#include "mseekbar.h"
#include "mslider_p.h"
#include "mtheme.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MSlider)

MSliderPrivate::MSliderPrivate()
{
}


MSliderPrivate::~MSliderPrivate()
{
}

int MSliderPrivate::adjustValue(int minimum, int value, int maximum, int steps)
{
    if (steps < 0)
        steps = -steps;

    if ((steps < 1) || (steps > maximum - minimum - 1))
        return value;

    //minimum or maximum are always valid values regardless
    //to the number of steps, values outside of slider range will
    //not be adjusted because model forces them to minimum
    //of maximum values
    if ((value <= minimum) || (value >= maximum))
        return value;

    int numberOfStep = qRound(((qreal(value) - qreal(minimum)) * qreal(steps)) / (qreal(maximum) - qreal(minimum)));
    qreal discreteValue = minimum + (qreal(numberOfStep) * (qreal(maximum) - qreal(minimum)) / qreal(steps));
    return qRound(discreteValue);
}

MSlider::MSlider(QGraphicsItem *parent, const QString &viewType)
    : MWidgetController(new MSliderPrivate(), new MSliderModel, parent)
{
    setViewType(viewType);
}

MSlider::MSlider(MSliderPrivate *dd, MSliderModel *model, QGraphicsItem *parent, const QString &viewType)
    : MWidgetController(dd, model, parent)
{
    setViewType(viewType);
}

MSlider::~MSlider()
{
}

void MSlider::setMinimum(int minimum)
{
    Q_D(MSlider);

    model()->setMinimum(minimum);

    int tmpValue = d->adjustValue(model()->minimum(),
                                  model()->value(),
                                  model()->maximum(),
                                  model()->steps());

    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void MSlider::setMaximum(int maximum)
{
    Q_D(MSlider);

    model()->setMaximum(maximum);

    int tmpValue = d->adjustValue(model()->minimum(),
                                  model()->value(),
                                  model()->maximum(),
                                  model()->steps());

    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void MSlider::setRange(int minimum, int maximum)
{
    Q_D(MSlider);

    setMaximum(maximum);
    setMinimum(minimum);

    int tmpValue = d->adjustValue(model()->minimum(),
                                  model()->value(),
                                  model()->maximum(),
                                  model()->steps());

    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void MSlider::setSteps(int steps)
{
    Q_D(MSlider);

    model()->setSteps(steps);

    int tmpValue = d->adjustValue(model()->minimum(),
                                  model()->value(),
                                  model()->maximum(),
                                  model()->steps());

    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void MSlider::setValue(int value)
{
    Q_D(MSlider);
    int tmpValue = d->adjustValue(model()->minimum(),
                                  value,
                                  model()->maximum(),
                                  model()->steps());
    if (tmpValue != model()->value())
        model()->setValue(tmpValue);
}

void MSlider::updateData(const QList<const char *>& modifications)
{
    MWidgetController::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == MSliderModel::Value) {
            emit valueChanged(model()->value());
        } else if (member == MSliderModel::State) {
            if (model()->state() == MSliderModel::Pressed)
                emit sliderPressed();
            else
                emit sliderReleased();
        }
    }
}

int MSlider::minimum() const
{
    return model()->minimum();
}

int MSlider::maximum() const
{
    return model()->maximum();
}

int MSlider::steps() const
{
    return model()->steps();
}

int MSlider::value() const
{
    return model()->value();
}

void MSlider::setOrientation(Qt::Orientation orientation)
{
    model()->setOrientation(orientation);
}

Qt::Orientation MSlider::orientation() const
{
    return model()->orientation();
}

void MSlider::setMinLabelIconID(const QString &iconID)
{
    model()->setMinLabelIcon(iconID);
}

void MSlider::setMinLabelVisible(bool visible)
{
    model()->setMinLabelVisible(visible);
}

void MSlider::setMinLabel(const QString &text)
{
    model()->setMinLabelText(text);
}

void MSlider::setMaxLabelIconID(const QString &iconID)
{
    model()->setMaxLabelIcon(iconID);
}

void MSlider::setMaxLabelVisible(bool visible)
{
    model()->setMaxLabelVisible(visible);
}

void MSlider::setMaxLabel(const QString &text)
{
    model()->setMaxLabelText(text);
}

void MSlider::setHandleLabelIconID(const QString &iconID)
{
    model()->setHandleLabelIcon(iconID);
}

void MSlider::setHandleLabelVisible(bool visible)
{
    model()->setHandleLabelVisible(visible);
}

void MSlider::setHandleLabel(const QString &text)
{
    model()->setHandleLabelText(text);
}

QString MSlider::minLabelIconID() const
{
    return model()->minLabelIcon();
}

QString MSlider::minLabelText() const
{
    return model()->minLabelText();
}

QString MSlider::maxLabelIconID() const
{
    return model()->maxLabelIcon();
}

QString MSlider::maxLabelText() const
{
    return model()->maxLabelText();
}

QString MSlider::handleLabelIconID() const
{
    return model()->handleLabelIcon();
}

QString MSlider::handleLabelText() const
{
    return model()->handleLabelText();
}

bool MSlider::isMinLabelVisible() const
{
    return model()->minLabelVisible();
}

bool MSlider::isMaxLabelVisible() const
{
    return model()->maxLabelVisible();
}

bool MSlider::isHandleLabelVisible() const
{
    return model()->handleLabelVisible();
}

void MSlider::setState(MSliderModel::SliderState state)
{
    model()->setState(state);
    update();
}

MSliderModel::SliderState MSlider::state() const
{
    return model()->state();
}
