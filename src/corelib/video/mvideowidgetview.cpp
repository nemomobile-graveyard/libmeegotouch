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

#include "mvideowidgetview.h"
#include "mvideowidgetview_p.h"
#include "mvideowidget.h"

#include "mapplication.h"
#include "mapplicationwindow.h"

#include <gst/gst.h>
#include <gst/gstvalue.h>
#include <gst/video/video.h>

#include "mgles2renderer.h"
#include "mgstvideo.h"

#include "msink.h"
#include "mdebug.h"

#include <mscene.h>

MVideoWidgetViewPrivate::MVideoWidgetViewPrivate()
    :   m_useSingleYuvTexture(false),
        image(NULL),
        bits(NULL),
        scaleOffsets(NULL),
        m_needFillBg(false),
        m_fullscreen(false),
        m_gstVideo(new MGstVideo())
{
    qRegisterMetaType< QList<const char*> >("QList<const char*>");
#ifdef M_USE_OPENGL
    MGLES2Renderer* r = MGLES2Renderer::instance();
    if( r ) {
        glGenTextures(3, &m_textures[0]);
        yuv1 = r->getShaderProgram(M_SHADER_SOURCE_DIR "/yuv1.frag" );
        yuv3 = r->getShaderProgram(M_SHADER_SOURCE_DIR "/yuv3.frag");
    }    
    else
        mWarning("MVideoWidgetViewPrivate::MVideoWidgetViewPrivate()") << "MGLES2Renderer not ready yet, cannot init shaders.";
#endif
}

MVideoWidgetViewPrivate::~MVideoWidgetViewPrivate()
{
    delete image;
    delete bits;
    delete scaleOffsets;
    
    delete m_gstVideo;

#ifdef M_USE_OPENGL
    glDeleteTextures(3, &m_textures[0]);
#endif
}

const MVideoWidgetModel* MVideoWidgetViewPrivate::model() const
{
    Q_Q(const MVideoWidgetView);
    return q->model();
}

MVideoWidgetModel* MVideoWidgetViewPrivate::model()
{
    Q_Q(MVideoWidgetView);
    return q->model();
}

void MVideoWidgetViewPrivate::update()
{
    Q_Q(MVideoWidgetView);
    q->update();
}

void MVideoWidgetViewPrivate::updateGeometry() 
{
    Q_Q(MVideoWidgetView);
    q->updateGeometry();
}

void MVideoWidgetViewPrivate::videoReady()
{
    model()->setLength(m_gstVideo->length());

    updateVideoGeometry();
    updateGeometry();
    update();
}

void MVideoWidgetViewPrivate::frameReady()
{
    model()->setPositionNoEmit(m_gstVideo->position());

    if( MApplication::softwareRendering()) {
        blitSwFrame();
    } else {
#ifdef M_USE_OPENGL
        blitGLFrame();
#else
        blitSwFrame();
#endif
    }

    //FIXME This is a workaround for a situation where Qt's animations/timers 
    //get broken when there are too much update calls coming (multiple video 
    //widgets visible and playing at a same time). Do not call update() for 
    //each videowidget's frame separately. This can be replaced with a single 
    //update() call when Qt side is fixed.
    static QTimer timer;
    if( !timer.isActive() ) {
        Q_Q(MVideoWidgetView);
        timer.setSingleShot(true);
        if( MApplication::activeApplicationWindow() )
            q->connect(&timer, SIGNAL(timeout()),
                       MApplication::activeApplicationWindow()->viewport(), SLOT(update()),
                       Qt::UniqueConnection);
        timer.start(0);
    }
    //update();
}

void MVideoWidgetViewPrivate::stateChanged()
{
    model()->setState(m_gstVideo->videoState());
}


void MVideoWidgetViewPrivate::updateVideoGeometry()
{
    Q_Q(MVideoWidgetView);

    QSize videoSize = m_gstVideo->resolution();
    QSize widgetSize = q->size().toSize();
    
    if( model()->scaleMode() == MVideoWidgetModel::ScaleToFit ) {
        if( model()->aspectRatioMode() == MVideoWidgetModel::AspectRatioOriginal ) {
            //scale but preserve the original aspect ratio
            videoSize.scale(widgetSize, Qt::KeepAspectRatio);
            qreal dx = (widgetSize.width() - videoSize.width()) / 2.0;
            qreal dy = (widgetSize.height() - videoSize.height()) / 2.0;
            m_scaledVideoRect.setRect(dx, dy, videoSize.width(), videoSize.height());
            m_needFillBg = widgetSize != videoSize;
        }
        else {
            //scale the video to match exactly the widget's size
            m_scaledVideoRect.setRect(0, 0, widgetSize.width(), widgetSize.height());
            m_needFillBg = false;
        }
    }
    else {
        //do not scale just center the video inside the widget
        qreal dx = (widgetSize.width() - videoSize.width()) / 2.0;
        qreal dy = (widgetSize.height() - videoSize.height()) / 2.0;
        m_scaledVideoRect.setRect(dx, dy, videoSize.width(), videoSize.height());
        m_needFillBg = widgetSize != videoSize;
    }
}

void MVideoWidgetViewPrivate::blitGLFrame()
{
    if( !m_gstVideo->frameData() )
        return;
        
    //try to lock the frame data if the we cannot lock it
    //a new frame is currently being received and there will update
    //call when the new frame has been completely received so we
    //can safely skip the new frame texture creation for now
    if( !m_gstVideo->lockFrameData() ) {
        mWarning("MVideoWidgetViewPrivate::blitGLFrame()") << "MUTEX LOCK CONFLICT!";
        return;
    }

    //create texture from the data
    if( m_gstVideo->frameDataFormat() == MGstVideo::RGB ) {
        blitRgbTexture(m_gstVideo->frameData(), m_gstVideo->resolution().width(), m_gstVideo->resolution().height());
    } else {
        m_useSingleYuvTexture ? blitSingleYuvTexture(m_gstVideo->frameData(), m_gstVideo->resolution().width(), m_gstVideo->resolution().height()) :
                                blitMultiYuvTexture(m_gstVideo->frameData(), m_gstVideo->resolution().width(), m_gstVideo->resolution().height());
    }

    m_gstVideo->unlockFrameData();
}

void MVideoWidgetViewPrivate::blitRgbTexture(const uchar* data, int  width, int height)
{
#ifdef M_USE_OPENGL
    GLuint target = GL_TEXTURE_2D;
    GLuint format = GL_RGB;
    glBindTexture(target, m_textures[0]);
    glTexImage2D(target, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
#else
    Q_UNUSED(data)
    Q_UNUSED(width)
    Q_UNUSED(height)
#endif
}

void MVideoWidgetViewPrivate::blitMultiYuvTexture(const uchar* data, int  width, int height)
{
#ifdef M_USE_OPENGL
    GLuint target = GL_TEXTURE_2D;
    GLuint format = GL_LUMINANCE;

    int w[3] = {width, width / 2, width / 2};
    int h[3] = {height, height / 2, height / 2};
    int offs[3] = {0, width*height, width*height*5 / 4 };
    for (int i = 0; i < 3; ++i) {
        glBindTexture(target, m_textures[i]);
        glTexImage2D(target, 0, format, w[i], h[i], 0,
                     GL_LUMINANCE, GL_UNSIGNED_BYTE, data + offs[i]);
    }
#else
    Q_UNUSED(data)
    Q_UNUSED(width)
    Q_UNUSED(height)    
#endif
}

void MVideoWidgetViewPrivate::blitSingleYuvTexture(const uchar* data, int width, int height)
{
#ifdef M_USE_OPENGL
    const uchar *sp = data;
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
#else
    Q_UNUSED(data)
    Q_UNUSED(width)
    Q_UNUSED(height)
#endif
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

void MVideoWidgetViewPrivate::blitSwFrame()
{
    if( !m_gstVideo->frameData() )
        return;
        
    //try to lock the frame data if the we cannot lock it
    //a new frame is currently being received and there will update
    //call when the new frame has been completely received so we
    //can safely skip the new frame texture creation for now
    if( !m_gstVideo->lockFrameData() ) {
        mWarning("MVideoWidgetViewPrivate::blitSwFrame()") << "MUTEX LOCK CONFLICT!";
        return;
    }

    blit(m_gstVideo->frameData(), m_gstVideo->resolution().width(), m_gstVideo->resolution().height());

    m_gstVideo->unlockFrameData();
}

/*
 * Prepare software blit, set correct image size, create
 * buffers & scaling offset table.
 */
 
void MVideoWidgetViewPrivate::prepareBlit(int w, int h)
{
    if (image && (image->width() != w || image->height() != h)) {
        delete image;
        delete bits;
        image = NULL;
        bits = NULL;
    }

    if (!image) {
        if (scaleOffsets)
            delete scaleOffsets;

        scaleOffsets = calc_linear_scale(m_gstVideo->resolution().width(), m_gstVideo->resolution().height(), w, h);

        bits = new uchar[w * h * 4];
        image = new QImage(bits, w, h, QImage::Format_RGB32);
        image->fill(0);
    }
}

/*
 * Blits the video frame using software rendering. Uses the
 * scale offset table precalculated in prepareBlit()
 */
void MVideoWidgetViewPrivate::blit(const uchar* data, int w, int h)
{
    QSize videoSize = m_gstVideo->resolution();
    
    //FIXME: non scaling method crashes currently.
    /*if( videoSize.width() == w && videoSize.height() == h ) {
        //blit without scaling
        blit(data);
        return;
    }else*/
    if (w > videoSize.width() || h > videoSize.height()) {
        // only downscaling supported
        mWarning("MVideoWidgetPrivate::blit()") << "error - upscaling not supported with sw rendering";
        return;
    }

    prepareBlit(w, h);

    // for each pixel in video frame
    uchar *sp, *dp;
    dp = bits;
    for (int i = 0, x = 0, y = 0; i < w*h; ++i, ++x, dp += 4) {
        sp = (uchar*)data + scaleOffsets[i];
        if (m_gstVideo->frameDataFormat() == MGstVideo::RGB) {
            // BGR -> RGB
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
        } else {
            int xx = (x * videoSize.width()) / w;
            int yy = (y * videoSize.height()) / h;

            // YUV420->RGB implementation (http://en.wikipedia.org/wiki/YUV#Y.27UV444)
            const uchar *srcp = data;
            // find yuv components from yuv420
            int y_val = srcp[yy * videoSize.width() + xx];
            int u_val = srcp[(yy/2) * (videoSize.width()/2) + xx/2 + videoSize.width()*videoSize.height()];
            int v_val = srcp[(yy/2) * (videoSize.width()/2) + xx/2 + videoSize.width()*videoSize.height() + ((videoSize.width()*videoSize.height())/4)];

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
    
    //force change of QImage::cacheKey(), to make sure the gl texture caching works.
    image->setPixel(0,0, image->pixel(0,0));    
}

/*
 * Blits the video frame using software rendering
 * without any scaling.
 */
void MVideoWidgetViewPrivate::blit(const uchar* data)
{
    QSize videoSize = m_gstVideo->resolution();
    prepareBlit(videoSize.width(), videoSize.height());

    const uchar *sp = data;
    uchar *dp = bits;
    for (int x = 0, y = 0; y < videoSize.height(); ++x, sp += 3, dp += 4) {

        if (m_gstVideo->frameDataFormat() == MGstVideo::RGB) {
            // BGR -> RGB
            dp[0] = sp[2];
            dp[1] = sp[1];
            dp[2] = sp[0];
        } else {
            // YUV420->RGB implementation (http://en.wikipedia.org/wiki/YUV#Y.27UV444)

            // find yuv components from yuv420
            int y_val = sp[y * videoSize.width() + x];
            int u_val = sp[(y/2) * (videoSize.width()/2) + x/2 + videoSize.width()*videoSize.height()];
            int v_val = sp[(y/2) * (videoSize.width()/2) + x/2 + videoSize.width()*videoSize.height() + ((videoSize.width()*videoSize.height())/4)];

            // fixed point yuv -> rgb conversion
            int r = (298 * (y_val - 16) + 409 * (v_val - 128) + 128) >> 8;
            int g = (298 * (y_val - 16) - 100 * (u_val - 128) - 208 * (v_val - 128) + 128) >> 8;
            int b = (298 * (y_val - 16) + 516 * (u_val - 128) + 128) >> 8;

            dp[0] = CLAMP(b, 0, 255);
            dp[1] = CLAMP(g, 0, 255);
            dp[2] = CLAMP(r, 0, 255);
        }

        if (x == videoSize.width()) {
            x = 0;
            ++y;
        }
    }
}

MVideoWidgetView::MVideoWidgetView(MVideoWidget* controller) :
    MWidgetView(* new MVideoWidgetViewPrivate, controller)
{
    Q_D(MVideoWidgetView);
    connect(d->m_gstVideo, SIGNAL(videoReady()),
            SLOT(videoReady()));
    connect(d->m_gstVideo, SIGNAL(frameReady()),
            SLOT(frameReady()));
    connect(d->m_gstVideo, SIGNAL(stateChanged()),
            SLOT(stateChanged()));
}

MVideoWidgetView::MVideoWidgetView(MVideoWidgetViewPrivate &dd, MVideoWidget *controller) :
    MWidgetView(dd, controller)
{
    Q_D(MVideoWidgetView);
    connect(d->m_gstVideo, SIGNAL(videoReady()),
            SLOT(videoReady()));
    connect(d->m_gstVideo, SIGNAL(frameReady()),
            SLOT(frameReady()));
    connect(d->m_gstVideo, SIGNAL(stateChanged()),
            SLOT(stateChanged()));
}

MVideoWidgetView::~MVideoWidgetView()
{
}

void MVideoWidgetView::drawContents(QPainter* painter, const QStyleOptionGraphicsItem* option) const
{
    Q_UNUSED(option);

    Q_D(const MVideoWidgetView);

    if( d->m_gstVideo->renderTarget() == MGstVideo::MSink )  {
        if (d->m_needFillBg && style()->backgroundColor().isValid()) {
            painter->fillRect(boundingRect(), style()->backgroundColor());
        }

        if( !d->m_gstVideo->isReady() )
            return;

        MGLES2Renderer* r = MGLES2Renderer::instance();
        if( r ) {
            bool yuv = d->m_gstVideo->frameDataFormat() == MGstVideo::YUV;
            if( yuv ) {
                if( d->m_useSingleYuvTexture ) {
                    r->begin(painter, d->yuv1);
                    r->bindTexture(d->m_textures[0], QSize(-1,-1), 0, "textureYUV");
                } else {
                    r->begin(painter, d->yuv3);
                    r->bindTexture(d->m_textures[0], QSize(-1,-1), 0, "textureY");
                    r->bindTexture(d->m_textures[1], QSize(-1,-1), 1, "textureU");
                    r->bindTexture(d->m_textures[2], QSize(-1,-1), 2, "textureV");
                }
            } else {
                r->begin(painter);
                r->bindTexture(d->m_textures[0]);
            }
            r->setInvertTexture(true);
            r->draw(d->m_scaledVideoRect.toRect());
            r->end();
        }
        else if( d->image ) {
            // SW rendering
            painter->drawImage(d->m_scaledVideoRect.toRect(), *d->image);
        }
    } else {
        painter->fillRect(boundingRect(), style()->colorKey());
        //QCoreApplication::flush();
        //d->m_gstVideo->expose();
    }
}

QSizeF MVideoWidgetView::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
    Q_D(const MVideoWidgetView);
    Q_UNUSED(constraint);
 
    if( d->m_gstVideo->isReady() ) {
        if (which == Qt::PreferredSize) {
            MWindow* w = MApplication::activeWindow();
            return model()->fullscreen() ? w->visibleSceneSize() : d->m_gstVideo->resolution();
        }
    }
    return QSizeF(-1, -1);
}

void MVideoWidgetView::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    Q_UNUSED(event);
    Q_D(MVideoWidgetView);
    d->updateVideoGeometry();
}

void MVideoWidgetView::applyStyle()
{
    Q_D(MVideoWidgetView);

    if( model()->fullscreen() ) {
        style().setModeFullscreen();
    }
    else {
        style().setModeDefault();
    }
    
    d->m_gstVideo->setColorKey(style()->colorKey());
    MWidgetView::applyStyle();

    //d->refreshStyleMode();

    update();
}

void MVideoWidgetView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MVideoWidgetView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MVideoWidgetView::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
}

void MVideoWidgetView::updateData(const QList<const char*>& modifications)
{
    Q_D(MVideoWidgetView);

    MWidgetView::updateData(modifications);

    const char* member;
    foreach(member, modifications) {
        if(member == MVideoWidgetModel::Filename ) {
            model()->setLength(0);
            if( !d->m_gstVideo->open(model()->filename()) ) {
            }
        }
        else if( member == MVideoWidgetModel::State ) {
            d->m_gstVideo->setVideoState(model()->state());
        }
        else if( member == MVideoWidgetModel::Position ) {
            d->m_gstVideo->seek(model()->position());
            update();
        }
        else if( member == MVideoWidgetModel::Looping ) {
            d->m_gstVideo->setLooping(model()->looping());
        }
        else if( member == MVideoWidgetModel::Muted ) {
            d->m_gstVideo->setMuted(model()->muted());
        }
        else if( member == MVideoWidgetModel::Volume ) {
            d->m_gstVideo->setVolume(model()->volume());
        }
        else if( member == MVideoWidgetModel::Fullscreen ) {
            if( model()->fullscreen() ) {
                d->m_gstVideo->setWinId(MApplication::activeApplicationWindow()->viewport()->winId());
                d->m_gstVideo->setRenderTarget(MGstVideo::XvSink);
            }
            else {
                d->m_gstVideo->setRenderTarget(MGstVideo::MSink);
            }
            
            applyStyle();
            updateGeometry();
            update();
        }
    }
}

void MVideoWidgetView::setupModel()
{
    MWidgetView::setupModel();

    Q_D(MVideoWidgetView);
    if( model()->fullscreen() ) {
        d->m_gstVideo->setWinId(MApplication::activeApplicationWindow()->viewport()->winId());
        d->m_gstVideo->setRenderTarget(MGstVideo::XvSink);
    }
    else {
        d->m_gstVideo->setRenderTarget(MGstVideo::MSink);
    }

    d->m_gstVideo->setLooping(model()->looping());
    d->m_gstVideo->open(model()->filename());

    applyStyle();
    updateGeometry();
    update();
}

M_REGISTER_VIEW_NEW(MVideoWidgetView, MVideoWidget)
#include "moc_mvideowidgetview.cpp"
