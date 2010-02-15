/****************************************************************************
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
#include "duivideowidget.h"
#include "duideviceprofile.h"
#include "duisink.h"

#include <DuiDebug>
#include <QFileInfo>
#include <QGraphicsSceneResizeEvent>
#include <QMutex>

#include "duiapplication.h"

#include <gst/gst.h>
#include <gst/gstvalue.h>
#include <gst/video/video.h>

#ifdef QT_OPENGL_LIB
#ifdef QT_OPENGL_ES_2
#define DUI_USE_OPENGL
#endif
#endif

#ifdef DUI_USE_OPENGL
#include "duivideowidgetshaders.h"
#include "duiglshader.h"
#include "duiglshaderprogram.h"
#include "duiglshaderuniform.h"
#include <QtOpenGL>
#endif

class DuiVideoWidgetPrivate
{
public:

    DuiVideoWidgetPrivate(DuiVideoWidget *videoWidget);
    virtual ~DuiVideoWidgetPrivate();

    // These methods are public but only meant to be used by the gstreamer
    // sink callbacks and internal implementation.
    bool prepareBlit(const void *data, int w, int h);
    void blit(const void *data);
    void blit(const void *data, int w, int h);

    bool isReady() const {
        return sinkReady;
    }
    void setReady() {
        sinkReady = true;
        m_videoWidget->emitVideoReady();
    }

    void updateGeom();

    void setBuffer(GstBuffer *data) {
        gst_buffer = data;
    }
    GstBuffer *getBuffer() {
        return gst_buffer;
    }

    int videoWidth() const {
        return DUI_GST_VIDEO_SINK(gst_elem_videosink)->w;
    }
    int videoHeight() const {
        return DUI_GST_VIDEO_SINK(gst_elem_videosink)->h;
    }

    uint readyForBlit() {
        return blit_ready;
    }
    void setBlitReady(uint val) {
        blit_ready = val;
    }

    void mute(bool mute) {
        m_muted = mute;

        if (gst_elem_volume) {
            g_object_set(gst_elem_volume, "mute", mute, NULL);
        }
    }

    void setVolume(qreal volume) {
        m_volume = volume;

        if (gst_elem_volume) {
            duiDebug("DuiVideoWidgetPrivate") << "setVolume()" << volume;
            g_object_set(gst_elem_volume, "volume", volume, NULL);
        }
    }

    GstElement *makeSink(bool yuv);
    GstElement *makeVolume();
    GstElement *getPipeline();

    void checkSeekable();

#ifdef DUI_USE_OPENGL
    class DuiVideoShaderManager
    {
    public:

        DuiVideoShaderManager();
        virtual ~DuiVideoShaderManager();

        void init(QGLWidget *gl);

        DuiGLShader *m_vertShader;
        DuiGLShader *m_rgbFragShader;
        DuiGLShader *m_yuvFragShader;

        DuiGLShaderProgram *m_rgbShaderProgram;
        DuiGLShaderProgram *m_yuvShaderProgram;

        bool m_ready;

        bool m_combineMatricesInShader;
        bool m_useSingleYuvTexture;
    };
    static DuiVideoShaderManager DUI_VIDEO_SHADER_MANAGER;

    void initGL(QGLWidget *gl);
    void setupShader(qreal opacity);
    void blitGLFrame();
    void fillRgbTexture(const void *data, int  width, int height);
    void fillMultiYuvTexture(const void *data, int  width, int height);
    void fillSingleYuvTexture(const void *data, int  width, int height);

    GLfloat m_matProj[4][4];
    GLfloat m_matWorld[4][4];

    GLfloat m_vertices[8];
    GLfloat m_texCoords[8];

    GLuint m_textures[3];

    bool m_glReady;
#endif

    /*!
     * Gstreamer components for video playback
     */
    GstElement *gst_elem_pipeline;
    GstElement *gst_elem_source;
    GstElement *gst_elem_volume;
    GstElement *gst_elem_decoder;

    GstElement *gst_elem_videosink;
    GstElement *gst_elem_audiosink;

    GstBus *messagebus;
    GstBuffer *gst_buffer;

    /*!
     * Boolean value to indicate that sink has prepared
     * and can start video playback.
     */
    bool sinkReady;

    QMutex m_mutex;

    QImage *image;
    uchar *bits;
    const int *scaleOffsets;
    uint blit_ready;

    bool m_enableScale;
    bool m_keepAspectRatio;
    bool m_needFillBg;
    bool m_fillBg;
    bool m_autoStartPlayback;
    QColor m_bgColor;
    bool m_repeat;
    bool m_seekable;
    bool m_muted;
    qreal m_volume;

    bool m_geomDirty;

    QString m_videoFilename;

    QRectF m_scaledVideoRect;

    DuiVideoWidget *m_videoWidget;
};

/*
 * Gstreamer message callback for catching general
 * messages and errors in stream.
 */
static gboolean
bus_callback(GstBus *bus, GstMessage *message, void *data)
{
    Q_UNUSED(bus);

    DuiVideoWidgetPrivate *widget = (DuiVideoWidgetPrivate *) data;

    // video stream has reached end
    if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_EOS) {
        if (widget->m_repeat) {
            if (widget->m_seekable) {
                widget->m_videoWidget->reset();
            } else {
                //TODO: Reset the video by some other means than seeking to 0
            }
        }
        //else
        //{
        //    widget->m_videoWidget->stop();
        //}
    }

    // some error has happened
    else if (GST_MESSAGE_TYPE(message) == GST_MESSAGE_ERROR) {
        GError *error = NULL;
        gst_message_parse_error(message, &error, NULL);
        duiDebug("duivideowidget.cpp") << "gstreamer : " << error->message;
        g_error_free(error);

        // stop pipeline, we are in trouble
        return false;
    }
    return true;
}

/*
 * Called from duisink to see if duivideowidget is
 * ready to blit another frame or should we skip the frame.
 */
static uint
blit_ready_cb(void *user_data)
{
    DuiVideoWidgetPrivate *widget = (DuiVideoWidgetPrivate *) user_data;
    return widget->readyForBlit();
}

/*
 * Called from duisink when the natural size of video is known.
 */
static void
video_ready_cb(void *user_data)
{
    DuiVideoWidgetPrivate *widget = (DuiVideoWidgetPrivate *) user_data;

    widget->setReady();
    widget->m_geomDirty = true;

    widget->setBlitReady(1);

    widget->checkSeekable();
    //widget->m_videoWidget->setPreferredSize(QSizeF(widget->videoWidth(), widget->videoHeight()));
    //widget->m_videoWidget->setMinimumSize(QSizeF(widget->videoWidth(), widget->videoHeight()));

    //automatically stop after buffering the first frame if the
    //user does not want to start the playback automatically
    if (!widget->m_autoStartPlayback) {
        if (widget->gst_elem_pipeline) {
            gst_element_set_state(widget->gst_elem_pipeline, GST_STATE_PAUSED);
        }
    }
}

/*
 * Called from duisink to render the current frame
 * in video stream.
 */
static void
render_frame_cb(void *pointer, void *user_data)
{
    DuiVideoWidgetPrivate *w = (DuiVideoWidgetPrivate *) user_data;

    w->m_mutex.lock();

    w->setBlitReady(0);

    //release old buffer
    if (w->getBuffer()) {
        gst_buffer_unref(w->getBuffer());
        w->setBuffer(NULL);
    }

    w->setBuffer(GST_BUFFER(pointer));

    if (DuiApplication::softwareRendering())
        w -> blit(pointer, w->videoWidth(), w->videoHeight());

    w->m_mutex.unlock();

    w->m_videoWidget->update();
}

#ifdef DUI_USE_OPENGL
DuiVideoWidgetPrivate::DuiVideoShaderManager DuiVideoWidgetPrivate::DUI_VIDEO_SHADER_MANAGER;

DuiVideoWidgetPrivate::DuiVideoShaderManager::DuiVideoShaderManager()
    : m_vertShader(NULL), m_rgbFragShader(NULL), m_yuvFragShader(NULL),
      m_rgbShaderProgram(NULL), m_yuvShaderProgram(NULL),
      m_ready(false), m_combineMatricesInShader(true), m_useSingleYuvTexture(true)
{
}

DuiVideoWidgetPrivate::DuiVideoShaderManager::~DuiVideoShaderManager()
{
    delete m_vertShader;
    delete m_rgbFragShader;
    delete m_yuvFragShader;
    delete m_rgbShaderProgram;
    delete m_yuvShaderProgram;
}

void DuiVideoWidgetPrivate::DuiVideoShaderManager::init(QGLWidget *gl)
{
    QGLContext *ctx = (QGLContext *)gl->context();

    //compile common vertex shader
    m_vertShader = new DuiGLShader(DuiGLShader::VertexShader, "", ctx);
    if (m_combineMatricesInShader)
        m_vertShader->addSource(QLatin1String(DuiVideoVertShaderSource2));
    else
        m_vertShader->addSource(QLatin1String(DuiVideoVertShaderSource));
    if (!m_vertShader->compile())
        duiWarning("DuiVideoWidgetPrivate") << "Image/Pixmap vertex shader failed to compile:" << m_vertShader->log();

    //compile frag shader for rendering rgb video
    m_rgbFragShader = new DuiGLShader(DuiGLShader::FragmentShader, "", ctx);
    m_rgbFragShader->addSource(QLatin1String(DuiRgbVideoFragShaderSource));
    if (!m_rgbFragShader->compile())
        duiWarning("DuiVideoWidgetPrivate") << "Image/Pixmap fragment shader failed to compile:" << m_rgbFragShader->log();

    //compile frag shader for rendering yuv video
    m_yuvFragShader = new DuiGLShader(DuiGLShader::FragmentShader, "", ctx);
    if (m_useSingleYuvTexture)
        m_yuvFragShader->addSource(QLatin1String(DuiYuvVideoFragShaderSource2));
    else
        m_yuvFragShader->addSource(QLatin1String(DuiYuvVideoFragShaderSource));
    if (!m_yuvFragShader->compile())
        duiWarning("DuiVideoWidgetPrivate") << "Image/Pixmap fragment shader failed to compile:" << m_yuvFragShader->log();

    //create shader program for rendering rgb video
    m_rgbShaderProgram = new DuiGLShaderProgram(ctx);
    m_rgbShaderProgram->addShader(m_vertShader);
    m_rgbShaderProgram->addShader(m_rgbFragShader);
    //glBindAttribLocation(m_rgbShaderProgram, 0, "inputVertex");
    //glBindAttribLocation(m_rgbShaderProgram, 0, "textureCoord");
    if (!m_rgbShaderProgram->link())
        duiWarning("DuiVideoWidgetPrivate") << "Image/Pixmap shader program failed to link:" << m_rgbShaderProgram->log();

    //create shader program for rendering yuv video
    m_yuvShaderProgram = new DuiGLShaderProgram(ctx);
    m_yuvShaderProgram->addShader(m_vertShader);
    m_yuvShaderProgram->addShader(m_yuvFragShader);
    //glBindAttribLocation(m_yuvShaderProgram, 0, "inputVertex");
    //glBindAttribLocation(m_yuvShaderProgram, 0, "textureCoord");
    if (!m_yuvShaderProgram->link())
        duiWarning("DuiVideoWidgetPrivate") << "Image/Pixmap shader program failed to link:" << m_yuvShaderProgram->log();

    m_ready = true;
}
#endif

DuiVideoWidgetPrivate::DuiVideoWidgetPrivate(DuiVideoWidget *videoWidget) :
#ifdef DUI_USE_OPENGL
    m_glReady(false),
#endif
    gst_buffer(NULL),
    sinkReady(false),
    image(NULL),
    bits(NULL),
    scaleOffsets(NULL),
    blit_ready(0),
    m_enableScale(true),
    m_keepAspectRatio(true),
    m_needFillBg(false),
    m_fillBg(false),
    m_autoStartPlayback(false),
    m_bgColor(Qt::black),
    m_repeat(false),
    m_seekable(false),
    m_muted(false),
    m_volume(1.0),
    m_geomDirty(false),
    m_videoWidget(videoWidget)
{
    gst_elem_pipeline  = NULL;
    gst_elem_decoder   = NULL;
    gst_elem_source    = NULL;
    gst_elem_volume    = NULL;
    gst_elem_videosink = NULL;
    gst_elem_audiosink = NULL;

#ifdef DUI_USE_OPENGL
    //init the texture coordinates (reverse texture y coordinates because the
    //gstreamer outputs the frame updside down)
    m_texCoords[0] = 0.0f; m_texCoords[1] = 0.0f;
    m_texCoords[2] = 0.0f; m_texCoords[3] = 1.0f;
    m_texCoords[4] = 1.0f; m_texCoords[5] = 1.0f;
    m_texCoords[6] = 1.0f; m_texCoords[7] = 0.0f;
#endif
}

DuiVideoWidgetPrivate::~DuiVideoWidgetPrivate()
{
    delete image;
    delete bits;
    delete scaleOffsets;

    if (gst_elem_pipeline) {
        gst_element_set_state(gst_elem_pipeline, GST_STATE_NULL);
        gst_object_unref(GST_OBJECT(gst_elem_pipeline));
    }

#ifdef DUI_USE_OPENGL
    glDeleteTextures(3, &m_textures[0]);
#endif
}

GstElement *DuiVideoWidgetPrivate::makeVolume()
{
    gst_elem_volume = gst_element_factory_make("volume", "volume");
    g_object_set(gst_elem_volume, "volume", m_volume, NULL);
    g_object_set(gst_elem_volume, "mute", m_muted, NULL);

    return gst_elem_volume;
}

GstElement *DuiVideoWidgetPrivate::makeSink(bool yuv)
{
    if (gst_elem_videosink) {
        return NULL;
    }

    gst_elem_videosink = yuv ? dui_gst_video_sink_yuv_new() : dui_gst_video_sink_new();

    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);

    duisink->user_data = this;
    duisink->frame_cb = render_frame_cb;
    duisink->ready_cb  = video_ready_cb;
    duisink->blit_ready_cb = blit_ready_cb;

    return gst_elem_videosink;
}

GstElement *DuiVideoWidgetPrivate::getPipeline()
{
    return gst_elem_pipeline;
}
void DuiVideoWidgetPrivate::checkSeekable()
{
    if (gst_elem_pipeline) {
        GstQuery *query;
        gboolean result;
        query = gst_query_new_seeking(GST_FORMAT_TIME);
        result = gst_element_query(gst_elem_pipeline, query);
        if (result) {
            gboolean seekable;
            gst_query_parse_seeking(query, NULL, &seekable, NULL, NULL);
            m_seekable = seekable;
        }
        gst_query_unref(query);
    }
}

DuiVideoWidget::DuiVideoWidget(QGraphicsItem *parent)
    : DuiWidget(parent),
      d_ptr(new DuiVideoWidgetPrivate(this))
{
}

DuiVideoWidget::~DuiVideoWidget()
{
    delete d_ptr;
}


/*
 * Precalculates a linear scale table for software blit.
 */
static const int*
calc_linear_scale(int src_w, int src_h, int dst_w, int dst_h)
{
    int *table = new int[dst_w * dst_h];
    for (int y = 0, i = 0; y < dst_h; ++y) {
        int s_scan = ((y * src_h) / dst_h) * (src_w * 3);
        for (int x = 0; x < dst_w; ++x, ++i) {
            table[i] = s_scan + ((x * src_w) / dst_w) * 3;
        }
    }
    return table;
}

/*
 * Prepare software blit, set correct image size, create
 * buffers & scaling offset table.
 */
bool
DuiVideoWidgetPrivate::prepareBlit(const void *data, int w, int h)
{
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);
    GstBuffer *buffer = GST_BUFFER(data);

    if (buffer == NULL || G_UNLIKELY(!GST_IS_BUFFER(buffer))) {
        duiDebug("DuiVideoWidgetPrivate") << "error with buffer";
        return false;
    }

    if (!sinkReady)
        return false;

    if (image && (image->width() != w || image->height() != h)) {
        delete image;
        delete bits;
        image = NULL;
        bits = NULL;
    }

    if (!image) {
        if (scaleOffsets)
            delete scaleOffsets;

        scaleOffsets = calc_linear_scale(duisink->w, duisink->h, w, h);

        bits = new uchar[w * h * 4];
        image = new QImage(bits, w, h, QImage::Format_RGB32);
        image->fill(0);
    }

    return true;
}

/*
 * Blits the video frame using software rendering. Uses the
 * scale offset table precalculated in prepareBlit()
 */
void
DuiVideoWidgetPrivate::blit(const void *data, int w, int h)
{
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);

    if (!prepareBlit(data, w, h))
        return;

    // only downscaling supported
    if (w > duisink->w || h > duisink->h) {
        duiDebug("DuiVideoWidgetPrivate") << "error - upscaling not supported with sw rendering";
        return;
    }

    uchar *sp, *dp;
    dp = bits;

    // for each pixel in video frame
    for (int i = 0, x = 0, y = 0; i < w * h; ++i, ++x, dp += 4) {

        sp = GST_BUFFER_DATA(data) + scaleOffsets[i];

        if (!duisink->yuv_mode) {
            // BGR -> RGB
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
        } else {
            int xx = (x * duisink->w) / w;
            int yy = (y * duisink->h) / h;

            // YUV420->RGB implementation (http://en.wikipedia.org/wiki/YUV#Y.27UV444)
            uchar *srcp = GST_BUFFER_DATA(data);

            // find yuv components from yuv420
            int y_val = srcp[yy * duisink->w + xx];
            int u_val = srcp[(yy/2) * (duisink->w/2) + xx/2 + duisink->w*duisink->h];
            int v_val = srcp[(yy/2) * (duisink->w/2) + xx/2 + duisink->w*duisink->h + ((duisink->w*duisink->h)/4)];

            // fixed point yuv -> rgb conversion
            int r = (298 * (y_val - 16) + 409 * (v_val - 128) + 128) >> 8;
            int g = (298 * (y_val - 16) - 100 * (u_val - 128) - 208 * (v_val - 128) + 128) >> 8;
            int b = (298 * (y_val - 16) + 516 * (u_val - 128) + 128) >> 8;

            dp[0] = CLAMP(b, 0, 255);
            dp[1] = CLAMP(g, 0, 255);
            dp[2] = CLAMP(r, 0, 255);
        }

        if (x == w) {
            x = 0;
            ++y;
        }
    }

}

/*
 * Blits the video frame using software rendering
 * without any scaling.
 */
void
DuiVideoWidgetPrivate::blit(const void *data)
{
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);

    if (!prepareBlit(data, duisink->w, duisink->h))
        return;

    uchar *sp = GST_BUFFER_DATA(data);
    uchar *dp = bits;

    for (int x = 0, y = 0; y < duisink->h; ++x, sp += 3, dp += 4) {

        if (!duisink->yuv_mode) {
            // BGR -> RGB
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
        } else {
            // YUV420->RGB implementation (http://en.wikipedia.org/wiki/YUV#Y.27UV444)
            sp = GST_BUFFER_DATA(data);

            // find yuv components from yuv420
            int y_val = sp[y * duisink->w + x];
            int u_val = sp[(y/2) * (duisink->w/2) + x/2 + duisink->w*duisink->h];
            int v_val = sp[(y/2) * (duisink->w/2) + x/2 + duisink->w*duisink->h + ((duisink->w*duisink->h)/4)];

            // fixed point yuv -> rgb conversion
            int r = (298 * (y_val - 16) + 409 * (v_val - 128) + 128) >> 8;
            int g = (298 * (y_val - 16) - 100 * (u_val - 128) - 208 * (v_val - 128) + 128) >> 8;
            int b = (298 * (y_val - 16) + 516 * (u_val - 128) + 128) >> 8;

            dp[0] = CLAMP(b, 0, 255);
            dp[1] = CLAMP(g, 0, 255);
            dp[2] = CLAMP(r, 0, 255);
        }

        if (x == duisink->w) {
            x = 0;
            ++y;
        }
    }
}

void DuiVideoWidgetPrivate::updateGeom()
{
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);

    if (!duisink || !m_geomDirty)
        return;

    QSizeF videoSize = QSizeF(duisink->w, duisink->h);
    QSizeF widgetSize = m_videoWidget->size();

    //QRect m_scaledVideoRect;
    if (m_enableScale) {
        //scale but preserve the original aspect ratio
        if (m_keepAspectRatio) {
            videoSize.scale(widgetSize, Qt::KeepAspectRatio);

            qreal dx = (widgetSize.width() - videoSize.width()) / 2.0;
            qreal dy = (widgetSize.height() - videoSize.height()) / 2.0;

            m_scaledVideoRect.setRect(dx, dy, videoSize.width(), videoSize.height());

            m_needFillBg = widgetSize != videoSize;
        }
        //scale the video to match exactly the widget's size
        else {
            m_scaledVideoRect.setRect(0, 0, widgetSize.width(), widgetSize.height());
            m_needFillBg = false;
        }
    }
    //do not scale just center the video inside the widget
    else {
        qreal dx = (widgetSize.width() - videoSize.width()) / 2.0;
        qreal dy = (widgetSize.height() - videoSize.height()) / 2.0;

        m_scaledVideoRect.setRect(dx, dy, videoSize.width(), videoSize.height());

        m_needFillBg = widgetSize != videoSize;
    }

#ifdef DUI_USE_OPENGL
    //update the vertices here already if using opengl for painting
    if (!DuiApplication::softwareRendering()) {
        qreal x1, x2, y1, y2;
        m_scaledVideoRect.getCoords(&x1, &y1, &x2, &y2);
        m_vertices[0] = x1; m_vertices[1] = y1;
        m_vertices[2] = x1; m_vertices[3] = y2;
        m_vertices[4] = x2; m_vertices[5] = y2;
        m_vertices[6] = x2; m_vertices[7] = y1;
    }
#endif

    m_geomDirty = false;
}

void DuiVideoWidget::play()
{
    Q_D(DuiVideoWidget);
    if (d->gst_elem_pipeline) {
        gst_element_set_state(d->gst_elem_pipeline, GST_STATE_PLAYING);
    }
}

void DuiVideoWidget::pause()
{
    Q_D(DuiVideoWidget);
    GstState current_state;

    if (d->gst_elem_pipeline) {
        gst_element_get_state(d->gst_elem_pipeline, &current_state, NULL, 0);

        if (current_state == GST_STATE_PLAYING) {
            gst_element_set_state(d->gst_elem_pipeline, GST_STATE_PAUSED);
        } else {
            gst_element_set_state(d->gst_elem_pipeline, GST_STATE_PLAYING);
        }
    }
}

void DuiVideoWidget::stop()
{
    Q_D(DuiVideoWidget);
    if (d->gst_elem_pipeline) {
        gst_element_set_state(d->gst_elem_pipeline, GST_STATE_PAUSED);
    }
}

bool DuiVideoWidget::reset()
{
    return seek(0);
}

bool DuiVideoWidget::isPlaying()
{
    Q_D(DuiVideoWidget);
    GstState current_state;

    if (d->gst_elem_pipeline) {
        gst_element_get_state(d->gst_elem_pipeline, &current_state, NULL, 0);
        return current_state == GST_STATE_PLAYING;
    }
    return false;
}

bool DuiVideoWidget::seek(qint64 time)
{
    Q_D(DuiVideoWidget);
    if (time < 0) {
        return false;
    }

    if (d->gst_elem_pipeline) {
        return gst_element_seek(d->gst_elem_pipeline, 1.0,
                                GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                                GST_SEEK_TYPE_SET, time * GST_MSECOND,
                                GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE);
    }

    return false;
}

qint64 DuiVideoWidget::length()
{
    Q_D(DuiVideoWidget);
    GstFormat format = GST_FORMAT_TIME;
    gint64 length;

    if (d->gst_elem_pipeline) {
        if (gst_element_query_duration(d->gst_elem_pipeline, &format, &length)) {
            return (qint64)(length / 1000000);
        }
    }
    return 0;
}

qint64 DuiVideoWidget::currentPos()
{
    Q_D(DuiVideoWidget);
    GstFormat format = GST_FORMAT_TIME;
    gint64 pos;

    if (d->gst_elem_pipeline) {
        if (gst_element_query_position(d->gst_elem_pipeline, &format, &pos)) {
            return (qint64)(pos / 1000000);
        }
    }
    return 0;
}

static void
callback_newpad(GstElement       *decodebin,
                GstPad           *pad,
                gboolean          last,
                DuiVideoWidgetPrivate   *w)
{
    GstPad       *sinkpad;
    GstCaps      *caps;
    GstElement   *sink;
    GstStructure *type;
    GstElement   *bin;

    Q_UNUSED(decodebin);
    Q_UNUSED(last);

    caps = gst_pad_get_caps(pad);
    type = gst_caps_get_structure(caps, 0);

    if (g_strrstr(gst_structure_get_name(type), "video")) {

        bool yuv = false;
        if (g_strrstr(gst_structure_get_name(type), "yuv"))
            yuv = true;

        sink = w->makeSink(yuv);

        if (!yuv) {
            bin = gst_bin_new("video bin");
            GstElement *colorspace = gst_element_factory_make("ffmpegcolorspace", NULL);

            gst_bin_add(GST_BIN(bin), colorspace);
            gst_bin_add(GST_BIN(bin), sink);
            gst_element_link_pads(colorspace, "src", sink, "sink");

            GstPad *pad = gst_element_get_pad(colorspace, "sink");

            gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
            gst_object_unref(pad);

            sink = bin;
        }
    } else if (g_strrstr(gst_structure_get_name(type), "audio")) {
        GstElement *vol;

        bin = gst_bin_new("audio bin");
        vol = w->makeVolume();

        sink = gst_element_factory_make("autoaudiosink", "sink");

        gst_bin_add(GST_BIN(bin), vol);
        gst_bin_add(GST_BIN(bin), sink);
        gst_element_link_pads(vol, "src", sink, "sink");

        GstPad *pad = gst_element_get_pad(vol, "sink");

        gst_element_add_pad(bin, gst_ghost_pad_new("sink", pad));
        gst_object_unref(pad);

        sink = bin;

    } else {
        duiDebug("duivideowidget.cpp") << "error creating sink for " << gst_structure_get_name(type);
        sink = NULL;
    }

    if (w->getPipeline() && sink) {
        gst_caps_unref(caps);
        gst_bin_add(GST_BIN_CAST(w->getPipeline()), sink);
        gst_element_set_state(sink, GST_STATE_PAUSED);

        sinkpad = gst_element_get_pad(sink, "sink");

        if (sinkpad) {
            gst_pad_link(pad, sinkpad);
            gst_object_unref(sinkpad);
        }

    }
}


void DuiVideoWidget::constructPipeline()
{
    Q_D(DuiVideoWidget);

    d->gst_elem_pipeline = gst_pipeline_new("dui-video-widget");
    d->gst_elem_source   = gst_element_factory_make("filesrc", "file-source");
    d->gst_elem_decoder  = gst_element_factory_make("decodebin2", "decoder");

    g_signal_connect(d->gst_elem_decoder, "new-decoded-pad", G_CALLBACK(callback_newpad), d_ptr);

    g_object_set(G_OBJECT(d->gst_elem_source), "location", d->m_videoFilename.toStdString().c_str(), NULL);

    gst_bin_add_many(GST_BIN(d->gst_elem_pipeline), d->gst_elem_source, d->gst_elem_decoder, NULL);
    gst_element_link_many(d->gst_elem_source, d->gst_elem_decoder, NULL);

    gst_element_set_state(d->gst_elem_pipeline, GST_STATE_READY);

    d->messagebus = gst_pipeline_get_bus(GST_PIPELINE(d->gst_elem_pipeline));
    gst_bus_add_watch(d->messagebus, bus_callback, d_ptr);
    gst_object_unref(d->messagebus);
}

bool DuiVideoWidget::open(const QString &filename, bool autoPlay)
{
    Q_D(DuiVideoWidget);
    QFileInfo info(filename);
    QString filepath(info.absoluteFilePath());

    d->m_videoFilename = filepath;

    if (!info.exists() || !info.isReadable()) {
        return false;
    }

    d->m_autoStartPlayback = autoPlay;

    stop();

    if (!d->gst_elem_pipeline) {
        constructPipeline();
    }

    gst_element_set_state(d->gst_elem_pipeline, GST_STATE_PLAYING);

    return true;
}

void DuiVideoWidget::setVolume(qreal volume)
{
    Q_D(DuiVideoWidget);
    if (volume < 0) {
        volume = 0;
    } else if (volume > 1.0) {
        volume = 1.0;
    }

    d->setVolume(volume);
}

qreal DuiVideoWidget::volume()
{
    Q_D(DuiVideoWidget);
    return d->m_volume;
    /*
        GValue val;
        qreal result = 0.0;

        if (!d->gst_elem_volume) {
            return result;
        }

        memset(&val, 0, sizeof(GValue));

        g_value_init(&val, G_TYPE_FLOAT);
        g_object_get_property(G_OBJECT(d->gst_elem_volume), "volume", &val);
        result = g_value_get_float(&val);
        g_value_unset(&val);

        return result;
    */
}

void DuiVideoWidget::mute(bool mute)
{
    Q_D(DuiVideoWidget);
    d->mute(mute);
}

bool DuiVideoWidget::muted()
{
    Q_D(DuiVideoWidget);
    return d->m_muted;
    /*
        GValue val;
        bool result = true;

        if (!d->gst_elem_volume) {
            return result;
        }

        memset(&val, 0, sizeof(gboolean));

        g_value_init(&val, G_TYPE_BOOLEAN);
        g_object_get_property(G_OBJECT(d->gst_elem_volume), "mute", &val);
        result = g_value_get_boolean(&val);
        g_value_unset(&val);

        return result;
    */
}

QImage DuiVideoWidget::currentFrame()
{
    Q_D(DuiVideoWidget);
#ifdef DUI_USE_OPENGL
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(d->gst_elem_videosink);
    return QImage(GST_BUFFER_DATA(d->gst_buffer), duisink->w, duisink->h, QImage::Format_RGB32);
#else
    return d->image->copy();
#endif
}

bool DuiVideoWidget::scaling()
{
    Q_D(DuiVideoWidget);
    return d->m_enableScale;
}

void DuiVideoWidget::setScaling(bool scaling)
{
    Q_D(DuiVideoWidget);
    d->m_geomDirty = true;
    d->m_enableScale = scaling;
}

bool DuiVideoWidget::keepAspectRatio()
{
    Q_D(DuiVideoWidget);
    return d->m_keepAspectRatio;
}

void DuiVideoWidget::setKeepAspectRatio(bool keepAspectRatio)
{
    Q_D(DuiVideoWidget);
    d->m_geomDirty = true;
    d->m_keepAspectRatio = keepAspectRatio;
}

bool DuiVideoWidget::backgroundFill()
{
    Q_D(DuiVideoWidget);
    return d->m_fillBg;
}

void DuiVideoWidget::setBackgroundFill(bool fillBg)
{
    Q_D(DuiVideoWidget);
    d->m_fillBg = fillBg;
}

QColor DuiVideoWidget::backgroundColor()
{
    Q_D(DuiVideoWidget);
    return d->m_bgColor;
}

void DuiVideoWidget::setBackgroundColor(const QColor &color)
{
    Q_D(DuiVideoWidget);
    d->m_bgColor = color;
}

bool DuiVideoWidget::repeat()
{
    Q_D(DuiVideoWidget);
    return d->m_repeat;
}

void DuiVideoWidget::setRepeat(bool repeat)
{
    Q_D(DuiVideoWidget);
    d->m_repeat = repeat;
}


QString DuiVideoWidget::videoFilename()
{
    Q_D(DuiVideoWidget);
    return d->m_videoFilename;
}


void DuiVideoWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_D(DuiVideoWidget);
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!d->sinkReady)
        return;

    //update the geometry if needed (scaling properties, video size or widget size has changed)
    //updateGeometry();
    d->updateGeom();

    if (d->m_needFillBg && d->m_fillBg) {
        painter->fillRect(boundingRect(), d->m_bgColor);
    }

#ifdef DUI_USE_OPENGL
    if (painter->paintEngine()->type() == QPaintEngine::OpenGL && !DuiApplication::softwareRendering()) {
        //TODO Check is it possible to init the gl stuff before the first paint call?
        //Need to get the GLWidget/Painter/GLContext somehow from somewhere to be able to init.
        //init GL stuff ones in the first paint call
        QGLWidget *gl = (QGLWidget *) painter->device();


        //init the orthogonal projection matrix
        //glOrtho(0, w, h, 0, -1, 1):
        //2.0/w,  0.0,    0.0, -1.0
        //0.0,   -2.0/h,  0.0,  1.0
        //0.0,    0.0,   -1.0,  0.0
        //0.0,    0.0,    0.0,  1.0
        //TODO: don't recreate the matrix always when rendering a frame
        //      recreate matrix only when the gl widget size has actually changed
        GLfloat w = gl->width();
        GLfloat h = gl->height();
        d->m_matProj[0][0] =  2.0 / w; d->m_matProj[1][0] =  0.0;   d->m_matProj[2][0] =  0.0; d->m_matProj[3][0] = -1.0;
        d->m_matProj[0][1] =  0.0;   d->m_matProj[1][1] = -2.0 / h; d->m_matProj[2][1] =  0.0; d->m_matProj[3][1] =  1.0;
        d->m_matProj[0][2] =  0.0;   d->m_matProj[1][2] =  0.0;   d->m_matProj[2][2] = -1.0; d->m_matProj[3][2] =  0.0;
        d->m_matProj[0][3] =  0.0;   d->m_matProj[1][3] =  0.0;   d->m_matProj[2][3] =  0.0; d->m_matProj[3][3] =  1.0;


        if (!d->m_glReady) {
            d->initGL(gl);
        }

        //FIXME generate the matrix only when the transromation has changed, with fullscreen video the transformation
        //can be totally forgotten, maybe rotations should not be allowed either??
        //GLfloat matWorld[4][4]  =
        //{
        //    {transform.m11(), transform.m12(), 0.0, transform.m13()},
        //    {transform.m21(), transform.m22(), 0.0, transform.m23()},
        //    {            0.0,             0.0, 1.0,             0.0},
        //    {transform.dx() + 0.5, transform.dy() + 0.5, 0.0, transform.m33()}
        //};
        //generate trasformation matrix to be given to opengl
        const QTransform &transform = painter->combinedTransform();
        d->m_matWorld[0][0] = transform.m11();
        d->m_matWorld[0][1] = transform.m12();
        d->m_matWorld[0][2] = 0.0;
        d->m_matWorld[0][3] = transform.m13();
        d->m_matWorld[1][0] = transform.m21();
        d->m_matWorld[1][1] = transform.m22();
        d->m_matWorld[1][2] = 0.0;
        d->m_matWorld[1][3] = transform.m23();
        d->m_matWorld[2][0] = 0.0;
        d->m_matWorld[2][1] = 0.0;
        d->m_matWorld[2][2] = 1.0;
        d->m_matWorld[2][3] = 0.0;
        d->m_matWorld[3][0] = transform.dx() + 0.5;
        d->m_matWorld[3][1] = transform.dy() + 0.5;
        d->m_matWorld[3][2] = 0.0;
        d->m_matWorld[3][3] = transform.m33();

        //create new gl texture from gstreamer data if there is data available
        d->blitGLFrame();

        //activate a proper shader and setup the uniforms
        d->setupShader(effectiveOpacity());

        //setup vertex and texture coord arrays
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, d->m_vertices);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, d->m_texCoords);

        glEnable(GL_BLEND);

        //render
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glDisable(GL_BLEND);

        //disable vertex and texture coord arrays
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        //activate the first texture unit so that qt still renders correctly
        //for some reason they never activate the unit after initialization.
        glActiveTexture(GL_TEXTURE0);
    } else {
#endif
        // SW rendering
        if (d->image) {
            painter->drawImage(d->m_scaledVideoRect, *d->image);
        }
#ifdef DUI_USE_OPENGL
    }
#endif

    d->setBlitReady(1);
}

QSizeF DuiVideoWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const DuiVideoWidget);
    Q_UNUSED(constraint);
    if (d->isReady()) {
        if (which == Qt::PreferredSize) {
            return QSizeF(d->videoWidth(), d->videoHeight());
        }
    }
    return QSizeF(-1, -1);
}

void DuiVideoWidget::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_D(DuiVideoWidget);
    Q_UNUSED(event);

    duiDebug("DuiVideoWidget") << "resizeEvent" << event->widget()/*->metaObject()->className()*/ << event->newSize() << event->oldSize();
    d->m_geomDirty = true;
}

void DuiVideoWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DuiVideoWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked();
}

void DuiVideoWidget::emitVideoReady()
{
    emit videoReady();
}

#ifdef DUI_USE_OPENGL
void DuiVideoWidgetPrivate::initGL(QGLWidget *gl)
{
    //init the static shader manager class if not init yet by
    //some other dui widget video object
    if (!DUI_VIDEO_SHADER_MANAGER.m_ready) {
        DUI_VIDEO_SHADER_MANAGER.init(gl);
    }

    glGenTextures(3, &m_textures[0]);
    m_glReady = true;
}

void DuiVideoWidgetPrivate::setupShader(qreal opacity)
{
    bool yuv = DUI_GST_VIDEO_SINK(gst_elem_videosink)->yuv_mode;

    DuiGLShaderProgram *program = NULL;
    if (yuv) {
        program = DUI_VIDEO_SHADER_MANAGER.m_yuvShaderProgram;
        program->use();

        if (DUI_VIDEO_SHADER_MANAGER.m_useSingleYuvTexture) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textures[0]);
            program->uniforms()[QLatin1String("textureYUV")];// = (quint32)0;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textures[0]);
            program->uniforms()[QLatin1String("textureY")] = (GLuint)0;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_textures[1]);
            program->uniforms()[QLatin1String("textureU")] = (GLuint)1;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_textures[2]);
            program->uniforms()[QLatin1String("textureV")] = (GLuint)2;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    } else {
        program = DUI_VIDEO_SHADER_MANAGER.m_rgbShaderProgram;
        program->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[0]);
        program->uniforms()[QLatin1String("texture")] = (GLuint)0;
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    if (DUI_VIDEO_SHADER_MANAGER.m_combineMatricesInShader) {
        //give the matrices to gl and combine them in the shader
        program->uniforms()[QLatin1String("matWorld")] = m_matWorld;
        program->uniforms()[QLatin1String("matProj")] = m_matProj;
    } else {
        //combine matrices so that the final matrix can be given to gl (column major order)
        GLfloat pmvMatrix[4][4];
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                pmvMatrix[col][row] = 0.0;
                for (int n = 0; n < 4; ++n)
                    pmvMatrix[col][row] += m_matProj[n][row] * m_matWorld[col][n];
            }
        }
        program->uniforms()[QLatin1String("pmvMatrix")] = pmvMatrix;
    }

    GLfloat o = opacity;
    program->uniforms()[QLatin1String("opacity")] = o;
}

void DuiVideoWidgetPrivate::blitGLFrame()
{
    //check if we have data for a new frame
    if (!gst_buffer) //|| G_UNLIKELY (!GST_IS_BUFFER (gst_buffer)) )
        return;

    //try to lock the mutex if possible if the we cannot lock it
    //a new frame is currently being received and there will update
    //call when the new frame has been completely received so we
    //can safely skip the new frame texture creation for now
    if (!m_mutex.tryLock())
        return;

    //create texture from the gstreamer data
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(gst_elem_videosink);
    if (!duisink->yuv_mode) {
        fillRgbTexture(GST_BUFFER_DATA(gst_buffer), duisink->w, duisink->h);
    } else {
        DUI_VIDEO_SHADER_MANAGER.m_useSingleYuvTexture ? fillSingleYuvTexture(GST_BUFFER_DATA(gst_buffer), duisink->w, duisink->h) :
        fillMultiYuvTexture(GST_BUFFER_DATA(gst_buffer), duisink->w, duisink->h);
    }

    //done with buffer, release it and unlock the mutex
    gst_buffer_unref(gst_buffer);
    setBuffer(NULL);
    m_mutex.unlock();
}

void DuiVideoWidgetPrivate::fillRgbTexture(const void *data, int  width, int height)
{
    GLuint target = GL_TEXTURE_2D;
    GLuint format = GL_RGB;
    glBindTexture(target, m_textures[0]);
    glTexImage2D(target, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
}

void DuiVideoWidgetPrivate::fillMultiYuvTexture(const void *data, int  width, int height)
{
    GLuint target = GL_TEXTURE_2D;
    GLuint format = GL_LUMINANCE;

    int w[3] = {width, width / 2, width / 2};
    int h[3] = {height, height / 2, height / 2};
    int offs[3] = {0, width *height, width *height * 5 / 4 };

    uchar *buf = (uchar *)data;
    for (int i = 0; i < 3; ++i) {
        glBindTexture(target, m_textures[i]);
        glTexImage2D(target, 0, format, w[i], h[i], 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, buf + offs[i]);
    }
}

void DuiVideoWidgetPrivate::fillSingleYuvTexture(const void *data, int width, int height)
{
    uchar *sp = (uchar *)data;
    uchar *texdata = new uchar[width * height * 3];
    uchar *dp = texdata;

    //do some precalculations for indexing the yuv data buffer
    int w_x_h = width * height;
    int w_x_h_div_4 = w_x_h / 4;
    int w_div_2 = width / 2;

    for (int y = 0; y < height; ++y) {
        int idx = y / 2 * w_div_2;

        for (int x = 0; x < width; ++x, dp += 3) {
            int x_div_2 = x / 2;

            // find yuv components from yuv420
            uchar y_val = sp[y * width + x];
            uchar u_val = sp[idx + x_div_2 + w_x_h];
            uchar v_val = sp[idx + x_div_2 + w_x_h + w_x_h_div_4];

            //fill texture data buffer
            dp[0] = y_val;
            dp[1] = u_val;
            dp[2] = v_val;
        }
    }

    //create the actual gl texture from the data
    GLuint target = GL_TEXTURE_2D;
    GLuint format = GL_RGB;
    glBindTexture(target, m_textures[0]);
    glTexImage2D(target, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, texdata);
    delete[] texdata;
}

#endif

