/*
 * This file is part of libmeegotouch *
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
#ifndef MVIDEO_H
#define MVIDEO_H

#include <QObject>
#include <QSize>
#include <MExport>

class M_EXPORT MVideo : public QObject
{
    Q_OBJECT

public:

    //! Supported raw video data formats.
    enum DataFormat {
        //! 24bit RGB
        RGB = 0, 
        //BGR,
        //RGBx,
        //BGRx,
        //! 12bit YUV (YV12, I420)
        YUV
    };


    //! States for the video playback
    enum State {
        //! Video is not ready yet, playback cannot be started.
        NotReady = 0,
        //! Video is paused.
        Paused,
        //! Video is playing.
        Playing,
        //! Video is stopped.
        Stopped
    };

    virtual bool open(const QString& filename) = 0;

    virtual bool isReady() const = 0;

    virtual void setVideoState(MVideo::State state) = 0;
    virtual MVideo::State videoState() const = 0;

    virtual void seek(quint64 time) = 0;
    virtual quint64 position() const = 0;
    virtual quint64 length() const = 0;

    virtual void setMuted(bool muted) = 0;
    virtual bool isMuted() const = 0;
    virtual void setVolume(qreal volume) = 0;
    virtual qreal volume() const = 0;

    virtual void setLooping(bool enabled) = 0;
    virtual bool isLooping() const = 0;

    virtual QSize resolution() const = 0;

    virtual uchar* frameData() const = 0;
    virtual MVideo::DataFormat frameDataFormat() const = 0;

    virtual bool lockFrameData() = 0;
    virtual void unlockFrameData() = 0;

Q_SIGNALS:

    void videoReady();
    void frameReady();
    void stateChanged();
};

#endif
