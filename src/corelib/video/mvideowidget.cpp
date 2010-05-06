/* * This file is part of libmeegotouch *
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mvideowidget.h"
#include "private/mwidgetcontroller_p.h"
#include "mdebug.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MVideoWidget)

class MVideoWidgetPrivate : public MWidgetControllerPrivate
{
public:

    Q_DECLARE_PUBLIC(MVideoWidget)

    MVideoWidgetPrivate();
    virtual ~MVideoWidgetPrivate();
    

};

MVideoWidgetPrivate::MVideoWidgetPrivate() 
{
}
    
MVideoWidgetPrivate::~MVideoWidgetPrivate()
{
}

MVideoWidget::MVideoWidget(QGraphicsItem *parent, MVideoWidgetModel* model)
    : MWidgetController(new MVideoWidgetPrivate, model == NULL ? new MVideoWidgetModel : model, parent)
{
}

MVideoWidget::MVideoWidget(MVideoWidgetPrivate* dd, MVideoWidgetModel* model, QGraphicsItem* parent)
    : MWidgetController(dd, model, parent)
{

}

MVideoWidget::~MVideoWidget()
{
}

void MVideoWidget::open(const QString& filename)
{
    model()->setFilename(filename);
}

MVideo::State MVideoWidget::state() const
{
    return model()->state();
}

void MVideoWidget::seek(quint64 time)
{
    if( time <= length() )
        model()->setPosition(time);
}

quint64 MVideoWidget::length() const
{
    return model()->length();
}

quint64 MVideoWidget::position() const
{
    return model()->position();
}

void MVideoWidget::setVolume(qreal volume)
{
    volume = (volume < 0) ? 0.0 : ((volume > 10.0) ? 10.0 : volume);
    model()->setVolume(volume);
}

qreal MVideoWidget::volume() const
{
    return model()->volume();
}

void MVideoWidget::setMuted(bool mute)
{
    model()->setMuted(mute);
}

bool MVideoWidget::isMuted() const
{
    return model()->muted();
}

/*QImage MVideoWidget::currentFrame() const
{
    Q_D(MVideoWidget);
#ifdef M_USE_OPENGL
    MGstVideoSink *msink = M_GST_VIDEO_SINK(d->gst_elem_videosink);
    return QImage(GST_BUFFER_DATA(d->gst_buffer), msink->w, msink->h, QImage::Format_RGB32);
#else
    return d->image->copy();
#endif

    return QImage();
}*/

void MVideoWidget::setScaleMode(MVideoWidgetModel::Scale mode)
{
    model()->setScaleMode(mode);
}

MVideoWidgetModel::Scale MVideoWidget::scaleMode() const
{
    return model()->scaleMode();
}

void MVideoWidget::setAspectRatioMode(MVideoWidgetModel::AspectRatio aspectRatio)
{
    model()->setAspectRatioMode(aspectRatio);
}

MVideoWidgetModel::AspectRatio MVideoWidget::aspectRatioMode() const
{
    return model()->aspectRatioMode();
}

void MVideoWidget::setLooping(bool enabled)
{
    model()->setLooping(enabled);
}

bool MVideoWidget::isLooping() const
{
    return model()->looping();
}

void MVideoWidget::setFullscreen(bool fullscreen)
{
    model()->setFullscreen(fullscreen);
}

bool MVideoWidget::isFullscreen() const
{
    return model()->fullscreen();
}

void MVideoWidget::play()
{
    model()->setState(MVideo::Playing);
}

void MVideoWidget::pause()
{
    model()->setState(MVideo::Paused);
}

void MVideoWidget::stop()
{
    model()->setState(MVideo::Stopped);
}

void MVideoWidget::setupModel()
{
    MWidgetController::setupModel();
}

void MVideoWidget::updateData(const QList<const char*>& modifications)
{
    MWidgetController::updateData(modifications);
    mDebug("MVideoWidget::updateData()") <<  modifications;
 
    const char* member;
    foreach(member, modifications) {
        if( member == MVideoWidgetModel::State ) {
            mDebug("MVideoWidget::updateData()") << "State" << model()->state();
        }
        else if( member == MVideoWidgetModel::Position ) {
            mDebug("MVideoWidget::updateData()") << "Position" << model()->position() << "/" << model()->length();
        }
        else if( member == MVideoWidgetModel::Length ) {
            if( model()->length() > 0 && model()->state() != MVideo::NotReady )
                emit videoReady();
        }
    }
}
