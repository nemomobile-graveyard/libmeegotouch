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

#ifndef _HAVE_M_GST_VIDEO_SINK_H
#define _HAVE_M_GST_VIDEO_SINK_H

#include <glib-object.h>
#include <gst/base/gstbasesink.h>
#include <gst/gstplugin.h>

//! \cond
G_BEGIN_DECLS

#define M_GST_TYPE_VIDEO_SINK m_gst_video_sink_get_type()

#define M_GST_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 M_GST_TYPE_VIDEO_SINK, MGstVideoSink))

#define M_GST_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                              M_GST_TYPE_VIDEO_SINK, MGstVideoSinkClass))

#define M_GST_IS_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 M_GST_TYPE_VIDEO_SINK))

#define M_GST_IS_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                              M_GST_TYPE_VIDEO_SINK))

#define M_GST_VIDEO_SINK_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                M_GST_TYPE_VIDEO_SINK, MGstVideoSinkClass))

typedef struct _MGstVideoSink        MGstVideoSink;
typedef struct _MGstVideoSinkClass   MGstVideoSinkClass;

/*!
 * \class MGstVideoSink
 * \brief MGstVideoSink is a simple video sink with a render callback function.
 *
 * MGstVideoSink provides callbacks for custom video rendering offering gstbuffer
 * to the frame_cb function.
 */
struct _MGstVideoSink {
    GstBaseSink parent;

    /*! render frame callback function
     * \param void pointer to GstBuffer
     * \param void pointer to user specified data
     */
    void (*frame_cb)(void *, void *);

    /*! render frame callback function
     * \param void pointer to user specified data
     */
    void (*ready_cb)(void *);

    void *user_data;

    int frameskip;
    guint yuv_mode;

    int w;
    int h;
};

struct _MGstVideoSinkClass {
    GstBaseSinkClass parent_class;
};

GType       m_gst_video_sink_get_type(void) G_GNUC_CONST;

/*!
 * \brief Constructor - creates RGB|BGR sink
 */
GstElement *m_gst_video_sink_new();

/*!
 * \brief Constructor - creates YUV sink
 */
GstElement *m_gst_video_sink_yuv_new();


G_END_DECLS
//! \endcond

#endif
