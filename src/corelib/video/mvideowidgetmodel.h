/* * This file is part of libmeegotouch *
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
#ifndef MVIDEOWIDGETMODEL_H
#define MVIDEOWIDGETMODEL_H

#include <mwidgetmodel.h>
#include "mvideo.h"

/*!
    \class MVideoWidgetModel
    \brief Data model class for MVideoWidget.

    \ingroup models
    \sa MVideoWidget
*/
class M_CORE_EXPORT MVideoWidgetModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MVideoWidgetModel)

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
        \property MVideoWidgetModel::state
        \brief Current state of the video playback. 
    */
    M_MODEL_PROPERTY(MVideo::State, state, State, true, MVideo::NotReady)

    /*!
        \property MVideoWidgetModel::filename
        \brief Name of the opened video file. 
    */
    M_MODEL_PROPERTY(QString, filename, Filename, true, QString())

    /*!
        \property MVideoWidgetModel::length
        \brief Length of the opened video in milliseconds. 
    */
    M_MODEL_PROPERTY(quint64, length, Length, true, 0)

    /*!
        \property MVideoWidgetModel::position
        \brief Current position of the played video in milliseconds. 
    */
    M_MODEL_PROPERTY(quint64, position, Position, true, 0)

    /*!
        \property MVideoWidgetModel::looping
        \brief Boolean flag defining whether video is looping automatically. 
    */
    M_MODEL_PROPERTY(bool, looping, Looping, true, true)

    /*!
        \property MVideoWidgetModel::scaleMode
        \brief Defines how the video is scaled to the canvas.
    */
    M_MODEL_PROPERTY(MVideoWidgetModel::Scale, scaleMode, ScaleMode, true, MVideoWidgetModel::ScaleToFit)

    /*!
        \property MVideoWidgetModel::aspectRatio
        \brief Defines how to set up aspect ratio for the video.
    */
    M_MODEL_PROPERTY(MVideoWidgetModel::AspectRatio, aspectRatioMode, AspectRatioMode, true, MVideoWidgetModel::AspectRatioOriginal)

    /*!
        \property MVideoWidgetModel::keepAspectRatio
        \brief Boolean flag defining if the video should be displayed in 
               fullscreen mode.
    */
    M_MODEL_PROPERTY(bool, fullscreen, Fullscreen, true, false)

    /*!
        \property MVideoWidgetModel::mute
        \brief Boolean flag defining audio of video should be muted. 
    */
    M_MODEL_PROPERTY(bool, muted, Muted, true, false)

    /*!
        \property MVideoWidgetModel::volume
        \brief Current audio volume of played video. 
    */
    M_MODEL_PROPERTY(qreal, volume, Volume, true, 1.0)

public:

    /*!
        \brief Update the MVideoWidgetModel::position property without emitting the change signal. 
    */
    void setPositionNoEmit(const qint64& pos) 
    {
        _position() = pos;
    }
};

#endif

