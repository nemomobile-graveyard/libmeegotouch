/* * This file is part of libdui *
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
#include "duivideowidget.h"
#include "private/duiwidgetcontroller_p.h"
#include "duidebug.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiVideoWidget)

class DuiVideoWidgetPrivate : public DuiWidgetControllerPrivate
{
public:

    Q_DECLARE_PUBLIC(DuiVideoWidget)

    DuiVideoWidgetPrivate();
    virtual ~DuiVideoWidgetPrivate();
    

};

DuiVideoWidgetPrivate::DuiVideoWidgetPrivate() 
{
}
    
DuiVideoWidgetPrivate::~DuiVideoWidgetPrivate()
{
}

DuiVideoWidget::DuiVideoWidget(QGraphicsItem *parent, DuiVideoWidgetModel* model)
    : DuiWidgetController(new DuiVideoWidgetPrivate, model == NULL ? new DuiVideoWidgetModel : model, parent)
{
}

DuiVideoWidget::DuiVideoWidget(DuiVideoWidgetPrivate* dd, DuiVideoWidgetModel* model, QGraphicsItem* parent)
    : DuiWidgetController(dd, model, parent)
{

}

DuiVideoWidget::~DuiVideoWidget()
{
}

void DuiVideoWidget::open(const QString& filename)
{
    model()->setFilename(filename);
}

DuiVideo::State DuiVideoWidget::state() const
{
    return model()->state();
}

void DuiVideoWidget::seek(quint64 time)
{
    if( time <= length() )
        model()->setPosition(time);
}

quint64 DuiVideoWidget::length() const
{
    return model()->length();
}

quint64 DuiVideoWidget::position() const
{
    return model()->position();
}

void DuiVideoWidget::setVolume(qreal volume)
{
    volume = (volume < 0) ? 0.0 : ((volume > 10.0) ? 10.0 : volume);
    model()->setVolume(volume);
}

qreal DuiVideoWidget::volume() const
{
    return model()->volume();
}

void DuiVideoWidget::setMuted(bool mute)
{
    model()->setMuted(mute);
}

bool DuiVideoWidget::isMuted() const
{
    return model()->muted();
}

/*QImage DuiVideoWidget::currentFrame() const
{
    Q_D(DuiVideoWidget);
#ifdef DUI_USE_OPENGL
    DuiGstVideoSink *duisink = DUI_GST_VIDEO_SINK(d->gst_elem_videosink);
    return QImage(GST_BUFFER_DATA(d->gst_buffer), duisink->w, duisink->h, QImage::Format_RGB32);
#else
    return d->image->copy();
#endif

    return QImage();
}*/

void DuiVideoWidget::setScaleMode(DuiVideoWidgetModel::Scale mode)
{
    model()->setScaleMode(mode);
}

DuiVideoWidgetModel::Scale DuiVideoWidget::scaleMode() const
{
    return model()->scaleMode();
}

void DuiVideoWidget::setAspectRatioMode(DuiVideoWidgetModel::AspectRatio aspectRatio)
{
    model()->setAspectRatioMode(aspectRatio);
}

DuiVideoWidgetModel::AspectRatio DuiVideoWidget::aspectRatioMode() const
{
    return model()->aspectRatioMode();
}

void DuiVideoWidget::setLooping(bool enabled)
{
    model()->setLooping(enabled);
}

bool DuiVideoWidget::isLooping() const
{
    return model()->looping();
}

void DuiVideoWidget::setFullscreen(bool fullscreen)
{
    model()->setFullscreen(fullscreen);
}

bool DuiVideoWidget::isFullscreen() const
{
    return model()->fullscreen();
}

void DuiVideoWidget::play()
{
    model()->setState(DuiVideo::Playing);
}

void DuiVideoWidget::pause()
{
    model()->setState(DuiVideo::Paused);
}

void DuiVideoWidget::stop()
{
    model()->setState(DuiVideo::Stopped);
}

void DuiVideoWidget::setupModel()
{
    DuiWidgetController::setupModel();
}

void DuiVideoWidget::updateData(const QList<const char*>& modifications)
{
    DuiWidgetController::updateData(modifications);
    duiDebug("DuiVideoWidget::updateData()") <<  modifications;
 
    const char* member;
    foreach(member, modifications) {
        if( member == DuiVideoWidgetModel::State ) {
            duiDebug("DuiVideoWidget::updateData()") << "State" << model()->state();
        }
        else if( member == DuiVideoWidgetModel::Position ) {
            duiDebug("DuiVideoWidget::updateData()") << "Position" << model()->position() << "/" << model()->length();
        }
        else if( member == DuiVideoWidgetModel::Length ) {
            if( model()->length() > 0 && model()->state() != DuiVideo::NotReady )
                emit videoReady();
        }
    }
}
