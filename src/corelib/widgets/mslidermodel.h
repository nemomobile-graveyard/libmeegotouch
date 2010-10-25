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

#ifndef MSLIDERMODEL_H
#define MSLIDERMODEL_H

#include <mwidgetmodel.h>

/*!
    \class MSliderModel
    \brief Model class for MSlider.

    \ingroup models
    \sa MSlider
*/
class M_CORE_EXPORT MSliderModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MSliderModel)

public:
    /*!
      \brief Defined whether slider handle is pressed (MSliderModel::Pressed)
             or released (MSliderModel::Released).
    */
    enum SliderState {
        Pressed,
        Released
    };

private:
    /*!
        \property MSliderModel::state
        \brief Slider state (it can be either MSliderModel::Released or MSliderModel::Pressed).
    */
    M_MODEL_PROPERTY(MSliderModel::SliderState, state, State, true, MSliderModel::Released)
    /*!
        \property MSliderModel::minimum
        \brief Slider range minimum value.
    */
    M_MODEL_PROPERTY(int, minimum, Minimum, false, 0)
    /*!
        \property MSliderModel::maximum
        \brief Slider range maximum value.
    */
    M_MODEL_PROPERTY(int, maximum, Maximum, false, 100)
    /*!
        \property MSliderModel::value
        \brief Slider value (value in slider range). Values are bounded into slider range.
    */
    M_MODEL_PROPERTY(int, value, Value, false, 0)
    /*!
        \property MSliderModel::steps
        \brief Slider value range subinterval (steps). If 0 there are no steps.
               If there are steps slider values are limited to integer multiples
               of step values.
    */
    M_MODEL_PROPERTY(int, steps, Steps, false, 0)
    /*!
        \property MSliderModel::orientation
        \brief Slider orientation. Can be Qt::Horizontal or Qt::Vertical.
    */
    M_MODEL_PROPERTY(Qt::Orientation, orientation, Orientation, true, Qt::Horizontal)
    /*!
        \property MSliderModel::minLabelIcon
        \brief Slider minimum indicator image identifier.
    */
    M_MODEL_PROPERTY(QString, minLabelIcon, MinLabelIcon, true, QString())
    /*!
        \property MSliderModel::minLabelText
        \brief Slider minimum indicator text.
    */
    M_MODEL_PROPERTY(QString, minLabelText, MinLabelText, true, QString())
    /*!
        \property MSliderModel::minLabelVisible
        \brief Slider minimum indicator visibility.
    */
    M_MODEL_PROPERTY(bool, minLabelVisible, MinLabelVisible, true, false)
    /*!
        \property MSliderModel::maxLabelIcon
        \brief Slider maximum indicator image identifier.
    */
    M_MODEL_PROPERTY(QString, maxLabelIcon, MaxLabelIcon, true, QString())
    /*!
        \property MSliderModel::maxLabelText
        \brief Slider maximum indicator text.
    */
    M_MODEL_PROPERTY(QString, maxLabelText, MaxLabelText, true, QString())
    /*!
        \property MSliderModel::maxLabelVisible
        \brief Slider maximum indicator visibility.
    */
    M_MODEL_PROPERTY(bool, maxLabelVisible, MaxLabelVisible, true, false)
    /*!
        \property MSliderModel::handleLabelIcon
        \brief Slider handle indicator image identifier.
    */
    M_MODEL_PROPERTY(QString, handleLabelIcon, HandleLabelIcon, true, QString())
    /*!
        \property MSliderModel::handleLabelText
        \brief Slider handle indicator text.
    */
    M_MODEL_PROPERTY(QString, handleLabelText, HandleLabelText, true, QString())
    /*!
        \property MSliderModel::handleLabelVisible
        \brief Slider handle indicator visibility.
    */
    M_MODEL_PROPERTY(bool, handleLabelVisible, HandleLabelVisible, true, false)

};

#endif

