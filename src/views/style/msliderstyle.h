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

#ifndef MSLIDERSTYLE_H
#define MSLIDERSTYLE_H

#include <QSize>
#include <QFont>
#include <QColor>

#include <mwidgetstyle.h>

class QPixmap;
class MScalableImage;

/*!
    \class MSliderStyle
    \brief Style class for MSlider and MSeekBar.

    \ingroup styles
    \sa MSliderStyleContainer MWidgetStyle \ref styling MSliderView
*/
class M_VIEWS_EXPORT MSliderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MSliderStyle)

    /*!
        \property MSliderStyle::handlePixmap
        \brief Handle image for released status of horizontal slider.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, handlePixmap, HandlePixmap)
    /*!
        \property MSliderStyle::handlePressedPixmap
        \brief Handle image for pressed status of horizontal slider.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, handlePressedPixmap, HandlePessedPixmap)
    /*!
        \property MSliderStyle::handleVerticalPixmap
        \brief Handle image for relesed status of vertical slider.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, handleVerticalPixmap, HandleVerticalPixmap)
    /*!
      \property MSliderStyle::handleLabelArrowLeftPixmap
      \brief Handle label arrow left image
      */
    M_STYLE_PTR_ATTRIBUTE(QPixmap*, handleLabelArrowLeftPixmap, HandleLabelArrowLeftPixmap)
    /*!
      \property MSliderStyle::handleLabelArrowRightPixmap
      \brief Handle label arrow right image
      */
    M_STYLE_PTR_ATTRIBUTE(QPixmap*, handleLabelArrowRightPixmap, HandleLabelArrowRightPixmap)
    /*!
      \property MSliderStyle::handleLabelArrowUpPixmap
      \brief Handle label arrow up image
      */
    M_STYLE_PTR_ATTRIBUTE(QPixmap*, handleLabelArrowUpPixmap, HandleLabelArrowUpPixmap)
    /*!
      \property MSliderStyle::handleLabelArrowDownPixmap
      \brief Handle label arrow down image
      */
    M_STYLE_PTR_ATTRIBUTE(QPixmap*, handleLabelArrowDownPixmap, HandleLabelArrowDownPixmap)
    /*!
        \property MSliderStyle::handleVerticalPressedPixmap
        \brief Handle image for pressed status of vertical slider.
    */
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, handleVerticalPressedPixmap, HandleVerticalPessedPixmap)
    /*!
        \property MSliderStyle::backgroundBaseImage
        \brief Background image for slider rail for horizontal slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundBaseImage, BackgroundBaseImage)
    /*!
        \property MSliderStyle::backgroundElapsedImage
        \brief Background image for elapsed value range of slider rail for horizontal slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundElapsedImage, BackgroundElapsedImage)
    /*!
        \property MSliderStyle::backgroundReceivedImage
        \brief Background image for received value range of slider rail for horizontal slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundReceivedImage, BackgroundReceivedImage)
    /*!
        \property MSliderStyle::backgroundBaseImage
        \brief Background image for slider rail for vertical slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundVerticalBaseImage, BackgroundVerticalBaseImage)
    /*!
        \property MSliderStyle::backgroundElapsedImage
        \brief Background image for elapsed value range of slider rail for vertical slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundVerticalElapsedImage, BackgroundVerticalElapsedImage)
    /*!
        \property MSliderStyle::backgroundReceivedImage
        \brief Background image for received value range of slider rail for vertical slider.
    */
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, backgroundVerticalReceivedImage, BackgroundVerticalReceivedImage)
    /*!
        \property MSliderStyle::grooveThickness
        \brief Thickness of slider groove.
    */
    M_STYLE_ATTRIBUTE(qreal, grooveThickness, GrooveThickness)
    /*!
    \property MSlider::groovePreferredLength
    \brief Preferred length of slider groove (measured along it)
          */
    M_STYLE_ATTRIBUTE(qreal, groovePreferredLength, GroovePreferredLength)
    /*!
        \property MSlider::grooveMinimumLength
        \brief Minimum length of slider groove (measured along it)
      */
    M_STYLE_ATTRIBUTE(qreal, grooveMinimumLength, GrooveMinimumLength)
    /*!
        \property MSlider::grooveMaximumLength
        \brief Maximum length of slider groove (measured along it)
      */
    M_STYLE_ATTRIBUTE(qreal, grooveMaximumLength, GrooveMaximumLength)
    /*!
        \property MSlider::moveFeedback
        \brief Feedback given when slider is moved
      */
    M_STYLE_ATTRIBUTE(MFeedback, moveFeedback, MoveFeedback)
    /*!
        \property MSlider::minimumFeedbackInterval
        \brief Minimum time between haptic feedbacks when moving slider (in milliseconds)
      */
    M_STYLE_ATTRIBUTE(int, minimumFeedbackInterval, MinimumFeedbackInterval)
    /*!
        \property MSlider::stepsPerFeedback
        \brief Steps needed before giving haptic feedback when moving slider
      */
    M_STYLE_ATTRIBUTE(int, stepsPerFeedback, StepsPerFeedback)
    /*!
        \property MSlider::handleLabelMargin
        \brief Minimum margin between handle indicator and slider edge
      */
    M_STYLE_ATTRIBUTE(qreal, handleLabelMargin, HandleLabelMargin)
    /*!
        \property MSlider::handleLabelArrowMargin
        \brief Minimum margin between arrow and handle indicator edge
      */
    M_STYLE_ATTRIBUTE(qreal, handleLabelArrowMargin, HandleLabelArrowMargin)
    /*!
        \property MSlider::elapsedOffset
        \brief How much the elapsed background draws over the value point
      */
    M_STYLE_ATTRIBUTE(qreal, elapsedOffset, ElapsedOffset)
    /*!
        \property MSlider::grooveMargin
        \brief Margin between the ends of the groove and widget
      */
    M_STYLE_ATTRIBUTE(qreal, grooveMargin, GrooveMargin)
    /*!
        \property MSlider::indicatorOffset
        \brief Adjusts the position of the indicator handle
      */
    M_STYLE_ATTRIBUTE(qreal, indicatorOffset, IndicatorOffset)
    /*!
        \property MSlider::indicatorStyleName
        \brief Style name of the indicator label
      */
    M_STYLE_ATTRIBUTE(QString, indicatorStyleName, IndicatorStyleName)
    /*!
        \property MSlider::minLabelFixedWidth
        \brief Fixed width of the minimum label of the slider, if not empty
      */
    M_STYLE_ATTRIBUTE(qreal, minLabelFixedWidth, MinLabelFixedWidth)
    /*!
        \property MSlider::maxLabelFixedWidth
        \brief Fixed width of the maximum label of the slider, if not empty
      */
    M_STYLE_ATTRIBUTE(qreal, maxLabelFixedWidth, MaxLabelFixedWidth)
    /*!
        \property MSlider::feedbackSpeedLimit
        \brief When slider is moved over this speed there will be no feedback (in mm/s)
      */
    M_STYLE_ATTRIBUTE(int, feedbackSpeedLimit, FeedbackSpeedLimit)
};

class M_VIEWS_EXPORT MSliderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSliderStyle)
};

#endif

