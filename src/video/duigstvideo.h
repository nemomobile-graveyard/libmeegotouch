/*
 * This file is part of libdui *
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
#ifndef DUIGSTVIDEO_P_H
#define DUIGSTVIDEO_P_H

#include <QObject>
#include <QMutex>

#include <gst/gst.h>
#include <gst/gstvalue.h>
#include <gst/video/video.h>

#include "duivideo.h"

class DuiGstVideo : public DuiVideo
{
    Q_OBJECT

public:

    DuiGstVideo();
    virtual ~DuiGstVideo();

    bool open(const QString& filename);

    bool isReady() const;

    void setVideoState(DuiVideo::State state);
    DuiVideo::State videoState() const;

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
    DuiVideo::DataFormat frameDataFormat() const;

    bool lockFrameData();
    void unlockFrameData();

private:

    static gboolean bus_cb(GstBus *bus, GstMessage *message, void *data);
    static void video_ready_cb(void *user_data);
    static void unknownpad_cb(  GstElement* bin,
                                GstPad* pad,
                                GstCaps* caps,
                                DuiGstVideo* w);
    static void newpad_cb(  GstElement* decodebin,
                            GstPad* pad,
                            gboolean last,
                            DuiGstVideo* w);
    static void render_frame_cb(void* pointer, void* user_data);

    GstElement* makeSink(bool yuv);
    GstElement* makeVolume();
        
    void constructPipeline();
    void destroyPipeline();
        
    void checkSeekable();    
    
    GstElement* gst_elem_pipeline;
    GstElement* gst_elem_source;
    GstElement* gst_elem_decoder;
    GstElement* gst_elem_volume;

    GstElement* gst_elem_videosink;
    GstElement* gst_elem_audiosink;

    GstBus* gst_messagebus;
    GstBuffer* gst_buffer;

    QString m_filename;
    bool m_looping;
    bool m_seekable;
    bool m_ready;
    bool m_muted;
    qreal m_volume;
    DuiVideo::State m_state;
    DuiVideo::DataFormat m_format;

    QMutex m_mutex;
};


#endif
