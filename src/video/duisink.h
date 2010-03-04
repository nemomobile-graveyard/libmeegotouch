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

#ifndef _HAVE_DUI_GST_VIDEO_SINK_H
#define _HAVE_DUI_GST_VIDEO_SINK_H

#include <glib-object.h>
#include <gst/base/gstbasesink.h>
#include <gst/gstplugin.h>

//! \cond
G_BEGIN_DECLS

#define DUI_GST_TYPE_VIDEO_SINK dui_gst_video_sink_get_type()

#define DUI_GST_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
                                 DUI_GST_TYPE_VIDEO_SINK, DuiGstVideoSink))

#define DUI_GST_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST ((klass), \
                              DUI_GST_TYPE_VIDEO_SINK, DuiGstVideoSinkClass))

#define DUI_GST_IS_VIDEO_SINK(obj) \
    (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
                                 DUI_GST_TYPE_VIDEO_SINK))

#define DUI_GST_IS_VIDEO_SINK_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE ((klass), \
                              DUI_GST_TYPE_VIDEO_SINK))

#define DUI_GST_VIDEO_SINK_GET_CLASS(obj) \
    (G_TYPE_INSTANCE_GET_CLASS ((obj), \
                                DUI_GST_TYPE_VIDEO_SINK, DuiGstVideoSinkClass))

typedef struct _DuiGstVideoSink        DuiGstVideoSink;
typedef struct _DuiGstVideoSinkClass   DuiGstVideoSinkClass;

/*!
 * \class DuiGstVideoSink
 * \brief DuiGstVideoSink is a simple video sink with a render callback function.
 *
 * DuiGstVideoSink provides callbacks for custom video rendering offering gstbuffer
 * to the frame_cb function.
 */
struct _DuiGstVideoSink {
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

struct _DuiGstVideoSinkClass {
    GstBaseSinkClass parent_class;
};

GType       dui_gst_video_sink_get_type(void) G_GNUC_CONST;

/*!
 * \brief Constructor - creates RGB|BGR sink
 */
GstElement *dui_gst_video_sink_new();

/*!
 * \brief Constructor - creates YUV sink
 */
GstElement *dui_gst_video_sink_yuv_new();


G_END_DECLS
//! \endcond

#endif
