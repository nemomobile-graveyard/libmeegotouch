/* * This file is part of libdui *
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#ifndef DUIVIDEOWIDGETMODEL_H
#define DUIVIDEOWIDGETMODEL_H

#include <duiwidgetmodel.h>
#include "duivideo.h"

/*!
    \class DuiVideoWidgetModel
    \brief Data model class for DuiVideoWidget.

    \ingroup models
    \sa DuiVideoWidget
*/
class DUI_EXPORT DuiVideoWidgetModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL_INTERNAL(DuiVideoWidgetModel)

public:
    enum Scale
    {
        ScaleDisabled,
        ScaleToFit
    };

    enum AspectRatio
    {
        AspectRatioOriginal,
        AspectRatioScaled
    };

    /*!
        \property DuiVideoWidgetModel::state
        \brief Current state of the video playback. 
    */
    DUI_MODEL_PROPERTY(DuiVideo::State, state, State, true, DuiVideo::NotReady)

    /*!
        \property DuiVideoWidgetModel::filename
        \brief Name of the opened video file. 
    */
    DUI_MODEL_PROPERTY(QString, filename, Filename, true, QString())

    /*!
        \property DuiVideoWidgetModel::length
        \brief Length of the opened video in milliseconds. 
    */
    DUI_MODEL_PROPERTY(quint64, length, Length, true, 0)

    /*!
        \property DuiVideoWidgetModel::position
        \brief Current position of the played video in milliseconds. 
    */
    DUI_MODEL_PROPERTY(quint64, position, Position, true, 0)

    /*!
        \property DuiVideoWidgetModel::looping
        \brief Boolean flag defining whether video is looping automatically. 
    */
    DUI_MODEL_PROPERTY(bool, looping, Looping, true, true)

    /*!
        \property DuiVideoWidgetModel::scaleMode
        \brief Defines how the video is scaled to the canvas.
    */
    DUI_MODEL_PROPERTY(DuiVideoWidgetModel::Scale, scaleMode, ScaleMode, true, DuiVideoWidgetModel::ScaleToFit)

    /*!
        \property DuiVideoWidgetModel::aspectRatio
        \brief Defines how to set up aspect ratio for the video.
    */
    DUI_MODEL_PROPERTY(DuiVideoWidgetModel::AspectRatio, aspectRatioMode, AspectRatioMode, true, DuiVideoWidgetModel::AspectRatioOriginal)

    /*!
        \property DuiVideoWidgetModel::keepAspectRatio
        \brief Boolean flag defining if the video should be displayed in 
               fullscreen mode.
    */
    DUI_MODEL_PROPERTY(bool, fullscreen, Fullscreen, true, false)

    /*!
        \property DuiVideoWidgetModel::mute
        \brief Boolean flag defining audio of video should be muted. 
    */
    DUI_MODEL_PROPERTY(bool, muted, Muted, true, false)

    /*!
        \property DuiVideoWidgetModel::volume
        \brief Current audio volume of played video. 
    */
    DUI_MODEL_PROPERTY(qreal, volume, Volume, true, 1.0)

public:

    /*!
        \brief Update the DuiVideoWidgetModel::position property without emitting the change signal. 
    */
    void setPositionNoEmit(const qint64& pos) 
    {
        _position() = pos;
    }
};

#endif

