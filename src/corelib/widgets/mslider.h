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

#ifndef MSLIDER_H
#define MSLIDER_H

#include "mwidgetcontroller.h"
#include <mslidermodel.h>

class MSliderPrivate;

/*!
    \class MSlider
    \brief MSlider is used for continuous set of values, among which user can choose one value.

    \ingroup widgets

    \section MSliderOverview Overview
        Slider defines a given value range from where user can choose one given value by moving
        the slider thumb to required position along the slider groove. Sliders are interactive
        elements.

        MSliders can be oriented horizontally or vertically, and they support left-to-right or
        right-to-left UI direction.

        Sliders have minimum and maximum labels located at the 2 endings of slider groove and thumb
        label located at thumb position.

    \section MSliderGuidelines Usage guidelines
        MSliders allow setting and getting of minimum and maximum values (value range) and
        current value and setting of number of steps.

        MSlider labels can be visible or hidden.

        MSliders can have margins and paddings with thickness defined in CSS.

    \section MSliderExampleCodes Example codes
        Slider value displayed in handle indicator:
        \code
            //connect valueChanged signal to modifySliderValue local slot
            QObject::connect(slider, SIGNAL(valueChanged(int)), this, SLOT(modifySliderValue(int)));

            //modifySliderValue
            void SliderTesterPage::modifySliderValue(int newValue)
            {
                slider->setHandleLabel(QString::number(newValue));
            }
        \endcode

    \sa MSliderModel MSliderStyle
*/
class M_CORE_EXPORT MSlider : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MSlider)

    /*!
        \property MSlider::state
        \brief Slider state (pressed or released).

        See MSliderModel::state for details.
    */
    Q_PROPERTY(MSliderModel::SliderState state READ state WRITE setState DESIGNABLE false)

    /*!
        \property MSlider::orientation
        \brief Slider orientation (horizontal or vertical).

        See MSliderModel::orientation for details.
    */
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)

    /*!
        \property MSlider::minLabelIconID
        \brief Slider minimum indicator image identifier.

        See MSliderModel::minLabelIcon for details.
    */
    Q_PROPERTY(QString minLabelIconID READ minLabelIconID WRITE setMinLabelIconID)

    /*!
        \property MSlider::maxLabelIconID
        \brief Slider maximum indicator image identifier.

        See MSliderModel::maxLabelIcon for details.
    */
    Q_PROPERTY(QString maxLabelIconID READ maxLabelIconID WRITE setMaxLabelIconID)

    /*!
        \property MSlider::handleLabelIconID
        \brief Slider handle indicator image identifier.

        See MSliderModel::handleLabelIcon for details.
    */
    Q_PROPERTY(QString handleLabelIconID READ handleLabelIconID WRITE setHandleLabelIconID)

    /*!
        \property MSlider::minLabelText
        \brief Slider minimum indicator text.

        See MSliderModel::minLabelText for details.
    */
    Q_PROPERTY(QString minLabelText READ minLabelText WRITE setMinLabel)

    /*!
        \property MSlider::maxLabelText
        \brief Slider maximum indicator text.

        See MSliderModel::maxLabelText for details.
    */
    Q_PROPERTY(QString maxLabelText READ maxLabelText WRITE setMaxLabel)

    /*!
        \property MSlider::handleLabelText
        \brief Slider handle indicator text.

        See MSliderModel::handleLabelText for details.
    */
    Q_PROPERTY(QString handleLabelText READ handleLabelText WRITE setHandleLabel)

    /*!
        \property MSlider::minLabelVisible
        \brief Slider minimum indicator visibility.

        See MSliderModel::minLabelVisible for details.
    */
    Q_PROPERTY(bool minLabelVisible READ isMinLabelVisible WRITE setMinLabelVisible)

    /*!
        \property MSlider::maxLabelVisible
        \brief Slider maximum indicator visibility.

        See MSliderModel::maxLabelVisible for details.
    */
    Q_PROPERTY(bool maxLabelVisible READ isMaxLabelVisible WRITE setMaxLabelVisible)

    /*!
        \property MSlider::handleLabelVisible
        \brief Slider handle indicator visibility, when slider is pressed (handle indicator is always hidden when slider is released).

        See MSliderModel::handleLabelVisible for details.
    */
    Q_PROPERTY(bool handleLabelVisible READ isHandleLabelVisible WRITE setHandleLabelVisible)

    /*!
        \property MSlider::minimum
        \brief Minimum value in slider value range.

        See MSliderModel::minimum for details.
    */
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum)

    /*!
        \property MSlider::maximum
        \brief Maximum value in slider value range.

        See MSliderModel::maximum for details.
    */
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum)

    /*!
        \property MSlider::steps
        \brief Values steps in slider value range.

        See MSliderModel::steps for details.
    */
    Q_PROPERTY(int steps READ steps WRITE setSteps)

    /*!
        \property MSlider::value
        \brief Slider value.

        See MSliderModel::value for details.
    */
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged USER true)
public:

    /*!
        \brief Constructs a slider.
        \param parent Parent m widget
        \param viewType Optional view type
     */
    explicit MSlider(QGraphicsItem *parent = 0, const QString &viewType = QString());

    /*!
       \brief Destructor
     */
    virtual ~MSlider();

    /*!
        \brief Gets minimum value in slider value range.
        \return minimum value in slider value range
     */
    int minimum() const;

    /*!
        \brief Gets maximum value in slider value range.
        \return maximum value in slider value range
     */
    int maximum() const;

    /*!
        \brief Gets number of values steps.
        \return number of value steps
     */
    int steps() const;

    /*!
        \brief Gets slider value.
        \return slider value
     */
    int value() const;

    /*!
        \brief Sets slider orientation.
        \param direction Slider orientation (Qt::Horizontal for horizontal sliders,\n
               Qt::Vertical for vertical sliders)
     */
    void setOrientation(Qt::Orientation direction);

    /*!
        \brief Gets slider current orientation.
        \return current orientation of the slider (Qt::Horizontal for horizontal sliders,\n
                Qt::Vertical for vertical sliders)
     */
    Qt::Orientation orientation() const;

    //newly added public methods
    /*!
        \brief Sets minimum indicator image identifier.
        \param text Minimum indicator image identifier
     */
    void setMinLabelIconID(const QString &iconID);

    /*!
        \brief Sets minimum indicator visibility.
        \param visible True if minimum indicator is visible false if it is not visible.
     */
    void setMinLabelVisible(bool visible);

    /*!
        \brief Sets minimum indicator text.
        \param text Minimum indicator text
     */
    void setMinLabel(const QString &text);

    /*!
        \brief Sets maximum indicator image identifier.
        \param text Maximum indicator image identifier
     */
    void setMaxLabelIconID(const QString &iconID);

    /*!
        \brief Sets maximum indicator visibility.
        \param visible True if maximum indicator is visible false if it is not visible.
     */
    void setMaxLabelVisible(bool visible);

    /*!
        \brief Sets maximum indicator text.
        \param text Maximum indicator text
     */
    void setMaxLabel(const QString &text);

    /*!
        \brief Sets thumb indicator image identifier.
        \param iconID Image identifier
     */
    void setHandleLabelIconID(const QString &iconID);

    /*!
        \brief Sets handle indicator visibility.
        \param visible True to set handle indicator visible false to set it invisible
     */
    void setHandleLabelVisible(bool visible);

    /*!
        \brief Sets handle indicator text.
        \param text The text string
     */
    void setHandleLabel(const QString &text);

    /*!
        \brief Gets minimum indicator image identifier
        \return minimum indicator image identifier
     */
    QString minLabelIconID() const;

    /*!
        \brief Gets minimum indicator text.
        \return minimum indicator text
     */
    QString minLabelText() const;

    /*!
        \brief Gets maximum indicator image identifier.
        \return maximum indicator image identifier
     */
    QString maxLabelIconID() const;

    /*!
        \brief Gets maximum indicator text.
        \return maximum indicator text
     */
    QString maxLabelText() const;

    /*!
        \brief Gets handle indicator image identifier.
        \return handle indicator image identifier
     */
    QString handleLabelIconID() const;

    /*!
        \brief Gets handle indicator text.
        \return handle indicator text
     */
    QString handleLabelText() const;

    /*!
        \brief Returns true if minimum indicator is visible and false if it is not visible.
        \return minimum indicator visibility
     */
    bool isMinLabelVisible() const;

    /*!
        \brief Returns true if maximum indicator is visible and false if is not visible.
        \return maximum indicator visibility
     */
    bool isMaxLabelVisible() const;

    /*!
        \brief Returns true if handle indicator is visible and false if is not visible.
        \return handle indicator visibility
     */
    bool isHandleLabelVisible() const;

    /*!
       \brief Set slider state programmatically.
       \param state going to be set
    */
    void setState(MSliderModel::SliderState state);

    /*!
       \brief Returns slider state.
       \return slider state
    */
    MSliderModel::SliderState state() const;

public Q_SLOTS:
    /*!
       \brief Sets the slider's minimum value.
       \param minimum value in slider value range
     */
    void setMinimum(int minimum);

    /*!
        \brief Sets the new maximum value for slider.
        \param maximum value in slider value range
     */
    void setMaximum(int maximum);

    /*!
        \brief Sets the value range for slider.
        \param minimum value in slider value range
        \param maximum value in slider value range
     */
    void setRange(int minimum, int maximum);

    /*!
        \brief Set number of value steps for slider.
        \param value steps to be set (for 0 there will not be steps)\n
               If there are steps slider will work in discrete mode being steps the number of allowed positions (besides min value).
               For example, setting 4 steps to a slider with a minimum value of 0 and a maximum value of 100 will make it to
               move only among the (0, 25, 50, 75, 100) set of values.
     */
    void setSteps(int steps);

    /*!
        \brief Set a new value for slider.
        \param value a value to be set (value is bounded into slider value range)
     */
    virtual void setValue(int value);

Q_SIGNALS:
    /*!
        \brief Signal emitted when slider value is changed.
        \param newValue new value
     */
    void valueChanged(int newValue);

    /*!
        \brief Emitted when the user presses the slider.
     */
    void sliderPressed();

    /*!
        \brief Emitted when the user releases the slider.
     */
    void sliderReleased();

protected Q_SLOTS:
    /*!
        \brief Signal emitted when some \sa MModel attribute is modified.
     */
    virtual void updateData(const QList<const char *>& modifications);

protected:
    /*!
        \brief Constructs a slider with private class and model.
        \param dd Private class instance
        \param model Model instance
        \param parent Parent m widget
        \param viewType Optional view type
     */
    MSlider(MSliderPrivate *dd, MSliderModel *model, QGraphicsItem *parent, const QString &viewType = QString());

private:
    Q_DISABLE_COPY(MSlider)
    Q_DECLARE_PRIVATE(MSlider)
};

#endif
