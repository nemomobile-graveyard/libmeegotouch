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
#ifndef MGSTVIDEO_P_H
#define MGSTVIDEO_P_H

#include <QObject>
#include <QMutex>
#include <QColor>

#include <gst/gst.h>
#include <gst/gstvalue.h>
#include <gst/video/video.h>

#include "mvideo.h"

//! \internal

class MGstVideo : public MVideo
{
    Q_OBJECT

public:

    enum RenderTarget
    {
        MSink,
        XvSink
    };

    MGstVideo();
    virtual ~MGstVideo();

    bool open(const QString& filename);

    bool isReady() const;

    void setVideoState(MVideo::State state);
    MVideo::State videoState() const;

    void seek(quint64 time);
    quint64 position() const;
    quint64 length() const;

    void setMuted(bool muted);
    bool isMuted() const;
    void setVolume(qreal volume);
    qreal volume() const;

    void setLooping(bool enabled) {m_looping = enabled;}
    bool isLooping() const {return m_looping;}

    QSize resolution() const;

    uchar* frameData() const;
    MVideo::DataFormat frameDataFormat() const;

    bool lockFrameData();
    void unlockFrameData();

    void setRenderTarget(MGstVideo::RenderTarget targetSink);
    MGstVideo::RenderTarget renderTarget();

    void expose();
    void setWinId(unsigned long id);
    unsigned long winId();
    void setColorKey(const QColor& key);
    QColor colorKey();

private:

    static gboolean bus_cb(GstBus *bus, GstMessage *message, void *data);
    static void video_ready_cb(void *user_data);
    static void unknownpad_cb(  GstElement* bin,
                                GstPad* pad,
                                GstCaps* caps,
                                MGstVideo* w);
    static void newpad_cb(  GstElement* decodebin,
                            GstPad* pad,
                            gboolean last,
                            MGstVideo* w);
    static void render_frame_cb(void* pointer, void* user_data);

    GstElement* makeSinks(bool yuv);
    GstElement* activeSink();
    
    GstElement* makeVolume();

    bool constructPipeline();
    void destroyPipeline();

    void checkSeekable();

    GstElement* gst_elem_pipeline;
    GstElement* gst_elem_source;
    GstElement* gst_elem_decoder;
    GstElement* gst_elem_volume;

    GstElement* gst_elem_audiosink;

    GstElement* gst_elem_videosink;
    GstElement* gst_elem_xvimagesink;

    GstBus* gst_messagebus;
    GstBuffer* gst_buffer;

    QString m_filename;
    bool m_looping;
    bool m_seekable;
    bool m_ready;
    bool m_muted;
    qreal m_volume;
    MVideo::State m_state;
    MVideo::DataFormat m_format;

    QMutex m_mutex;

    MGstVideo::RenderTarget m_renderTarget;
    MVideo::State m_storedState;
    quint64 m_storedPosition;

    unsigned long m_winId;
    QColor m_colorKey;
};

//! \internal_end

#endif
