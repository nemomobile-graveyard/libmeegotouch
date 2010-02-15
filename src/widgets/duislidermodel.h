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

#ifndef DUISLIDERMODEL_H
#define DUISLIDERMODEL_H

#include <duiwidgetmodel.h>

/*!
    \class DuiSliderModel
    \brief Model class for DuiSlider.

    \ingroup models
    \sa DuiSlider
*/
class DUI_EXPORT DuiSliderModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiSliderModel)

public:
    /*!
      \brief Defined whether slider handle is pressed (DuiSliderModel::Pressed)
             or released (DuiSliderModel::Released).
    */
    enum SliderState {
        Pressed,
        Released
    };

private:
    /*!
        \property DuiSliderModel::state
        \brief Slider state (it can be either DuiSliderModel::Released or DuiSliderModel::Pressed).
    */
    DUI_MODEL_PROPERTY(DuiSliderModel::SliderState, state, State, true, DuiSliderModel::Released)
    /*!
        \property DuiSliderModel::minimum
        \brief Slider range minimum value.
    */
    DUI_MODEL_PROPERTY(int, minimum, Minimum, false, 0)
    /*!
        \property DuiSliderModel::maximum
        \brief Slider range maximum value.
    */
    DUI_MODEL_PROPERTY(int, maximum, Maximum, false, 100)
    /*!
        \property DuiSliderModel::value
        \brief Slider value (value in slider range). Values are bounded into slider range.
    */
    DUI_MODEL_PROPERTY(int, value, Value, false, 0)
    /*!
        \property DuiSliderModel::steps
        \brief Slider value range subinterval (steps). If 0 there are no steps.
               If there are steps slider values are limited to integer multiples
               of step values.
    */
    DUI_MODEL_PROPERTY(int, steps, Steps, false, 0)
    /*!
        \property DuiSliderModel::orientation
        \brief Slider orientation. Can be Qt::Horizontal or Qt::Vertical.
    */
    DUI_MODEL_PROPERTY(Qt::Orientation, orientation, Orientation, true, Qt::Horizontal)
    /*!
        \property DuiSliderModel::minLabelIcon
        \brief Slider minimum indicator image identifier.
    */
    DUI_MODEL_PROPERTY(QString, minLabelIcon, MinLabelIcon, true, QString())
    /*!
        \property DuiSliderModel::minLabelText
        \brief Slider minimum indicator text.
    */
    DUI_MODEL_PROPERTY(QString, minLabelText, MinLabelText, true, QString())
    /*!
        \property DuiSliderModel::minLabelVisible
        \brief Slider minimum indicator visibility.
    */
    DUI_MODEL_PROPERTY(bool, minLabelVisible, MinLabelVisible, true, false)
    /*!
        \property DuiSliderModel::maxLabelIcon
        \brief Slider maximum indicator image identifier.
    */
    DUI_MODEL_PROPERTY(QString, maxLabelIcon, MaxLabelIcon, true, QString())
    /*!
        \property DuiSliderModel::maxLabelText
        \brief Slider maximum indicator text.
    */
    DUI_MODEL_PROPERTY(QString, maxLabelText, MaxLabelText, true, QString())
    /*!
        \property DuiSliderModel::maxLabelVisible
        \brief Slider maximum indicator visibility.
    */
    DUI_MODEL_PROPERTY(bool, maxLabelVisible, MaxLabelVisible, true, false)
    /*!
        \property DuiSliderModel::handleLabelIcon
        \brief Slider handle indicator image identifier.
    */
    DUI_MODEL_PROPERTY(QString, handleLabelIcon, HandleLabelIcon, true, QString())
    /*!
        \property DuiSliderModel::handleLabelText
        \brief Slider handle indicator text.
    */
    DUI_MODEL_PROPERTY(QString, handleLabelText, HandleLabelText, true, QString())
    /*!
        \property DuiSliderModel::handleLabelVisible
        \brief Slider handle indicator visibility.
    */
    DUI_MODEL_PROPERTY(bool, handleLabelVisible, HandleLabelVisible, true, false)

};

#endif

