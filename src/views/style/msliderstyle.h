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
class M_EXPORT MSliderStyle : public MWidgetStyle
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
};

class M_EXPORT MSliderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSliderStyle)
};

#endif

