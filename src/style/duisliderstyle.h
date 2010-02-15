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

#ifndef DUISLIDERSTYLE_H
#define DUISLIDERSTYLE_H

#include <QSize>
#include <QFont>
#include <QColor>

#include <duiwidgetstyle.h>

class QPixmap;
class DuiScalableImage;

/*!
    \class DuiSliderStyle
    \brief Style class for DuiSlider and DuiSeekBar.

    \ingroup styles
    \sa DuiSliderStyleContainer DuiWidgetStyle \ref styling DuiSliderView
*/
class DUI_EXPORT DuiSliderStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiSliderStyle)

    /*!
        \property DuiSliderStyle::handlePixmap
        \brief Handle image for released status of horizontal slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, handlePixmap, HandlePixmap)
    /*!
        \property DuiSliderStyle::handlePressedPixmap
        \brief Handle image for pressed status of horizontal slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, handlePressedPixmap, HandlePessedPixmap)
    /*!
        \property DuiSliderStyle::handleVerticalPixmap
        \brief Handle image for relesed status of vertical slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, handleVerticalPixmap, HandleVerticalPixmap)
    /*!
        \property DuiSliderStyle::handleVerticalPressedPixmap
        \brief Handle image for pressed status of vertical slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(QPixmap *, handleVerticalPressedPixmap, HandleVerticalPessedPixmap)
    /*!
        \property DuiSliderStyle::backgroundBaseImage
        \brief Background image for slider rail for horizontal slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundBaseImage, BackgroundBaseImage)
    /*!
        \property DuiSliderStyle::backgroundElapsedImage
        \brief Background image for elapsed value range of slider rail for horizontal slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundElapsedImage, BackgroundElapsedImage)
    /*!
        \property DuiSliderStyle::backgroundReceivedImage
        \brief Background image for received value range of slider rail for horizontal slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundReceivedImage, BackgroundReceivedImage)
    /*!
        \property DuiSliderStyle::backgroundBaseImage
        \brief Background image for slider rail for vertical slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundVerticalBaseImage, BackgroundVerticalBaseImage)
    /*!
        \property DuiSliderStyle::backgroundElapsedImage
        \brief Background image for elapsed value range of slider rail for vertical slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundVerticalElapsedImage, BackgroundVerticalElapsedImage)
    /*!
        \property DuiSliderStyle::backgroundReceivedImage
        \brief Background image for received value range of slider rail for vertical slider.
    */
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, backgroundVerticalReceivedImage, BackgroundVerticalReceivedImage)
    /*!
        \property DuiSliderStyle::grooveThickness
        \brief Thickness of slider groove.
    */
    DUI_STYLE_ATTRIBUTE(qreal, grooveThickness, GrooveThickness)
    /*!
    \property DuiSlider::groovePreferredLength
    \brief Preferred length of slider groove (measured along it)
          */
    DUI_STYLE_ATTRIBUTE(qreal, groovePreferredLength, GroovePreferredLength)
    /*!
        \property DuiSlider::grooveMinimumLength
        \brief Minimum length of slider groove (measured along it)
      */
    DUI_STYLE_ATTRIBUTE(qreal, grooveMinimumLength, GrooveMinimumLength)
    /*!
        \property DuiSlider::grooveMaximumLength
        \brief Maximum length of slider groove (measured along it)
      */
    DUI_STYLE_ATTRIBUTE(qreal, grooveMaximumLength, GrooveMaximumLength)
};

class DUI_EXPORT DuiSliderStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiSliderStyle)
};

#endif

