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

#ifndef MVIDEOWIDGET_H
#define MVIDEOWIDGET_H

#include <mwidgetcontroller.h>
#include <mvideowidgetmodel.h>

class MVideoWidgetPrivate;


/*!
    \class MVideoWidget
    \brief MVideoWidget provides functionality for playing videos.

    \ingroup widgets

    \section MVideoWidgetOverview Overview
        MVideoWidget supports following features:
        
        - Standard playback controlling through play(), pause(), stop(), seek() 
        and setLooping() methods.
        - Volume controlling through the setVolume() and setMute() methods.
        - Automatic fitting and appearence of the video can be changed using the 
        setScaleToFit(), setKeepAspectRatio() and setFullscreen()
        methods.

    \section MVideoWidgetUseGuidelines Usage guidelines

    \section MVideoWidgetVariants Variants
        \li \link MVideoWidgetView Default view. \endlink Standard view for 
            displaying video frames without any visual controls.

    \section MVideoWidgetOpenIssues Open issues

    \section MVideoWidgetExamples Examples

    \sa MVideoWidgetModel
*/
class M_CORE_EXPORT MVideoWidget : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MVideoWidget)

public:

    /*!
      \brief Constructs a video widget.
    */
    MVideoWidget(QGraphicsItem *parent = 0, MVideoWidgetModel* model = 0);

    /*!
      \brief Destroys the video widget.
    */
    virtual ~MVideoWidget();

    /*!
        \brief Opens a video from a file.
    */
    void open(const QString& filename);

    /*!
        \brief Return the playback state.
    */
    MVideo::State state() const;

    /*!
        \brief Seek video to a wanted position/time.

        \a time is defined as milliseconds and it should be in a range of 0 to 
        lenght of video.
    */
    void seek(quint64 time);

    /*!
        \brief Query length of the video.

        \return Length of video in milliseconds.
    */
    quint64 length() const;

    /*!
        \brief Query the current position of the video.

        \return Position of the video in milliseconds.
    */
    quint64 position() const;

    /*!
        \brief Set volume factor for the video's audio track.

        \a volume should be in a range of 0.0 -> 10.0. 1.0 means 100% volume. 
        1.0 -> 10.0 are gain through amplification.
    */
    void setVolume(qreal volume);

    /*!
        \brief Query current volume factor of the video's audio track.

        \returns Audio volume. 0.0 if video does not contain audio track.
    */
    qreal volume() const;

    /*!
        \brief Mute the video's audio.
    */
    void setMuted(bool mute);

    /*!
        \brief Query current mute status.

        \returns True if audio is muted or no audio track exists, false is audio is 
                 not muted.
    */
    bool isMuted() const;

    /*!
     * Snapshot the current video frame.
     * \returns image of current video frame
     */
    //QImage currentFrame() const;

    /*!
        \brief Set scaling mode to video.

        If \a mode is MVideoWidgetModel::ScaleToFit the video scaled to fit the widget size. If \a mode 
        is MVideoWidgetModel::ScaleDisabled the video is rendered in it's native resolution into the middle 
        of the widget.
    */
    void setScaleMode(MVideoWidgetModel::Scale mode);

    /*!
        \brief Returns video scaling mode.
    */
    MVideoWidgetModel::Scale scaleMode() const;

    /*!
        \brief Set aspect ratio

        If \a aspectRatio is MVideoWidgetModel::AspectRatioOriginal, the video is scaled without breaking the 
        native aspect ratio. If \a aspectRatio is MVideoWidgetModel::AspectRatioScaled, the native aspect 
        ratio is not preserved and the video is scaled to match the size of the widget.
    */
    void setAspectRatioMode(MVideoWidgetModel::AspectRatio aspectRatio);

    /*!
        \brief Return type of the aspect ratio.
    */
    MVideoWidgetModel::AspectRatio aspectRatioMode() const;

    /*!
        \brief Enable / disable automatic looping of the video.
    */
    void setLooping(bool enabled);

    /*!
        \brief Query looping mode.

        \returns looping mode.
    */
    bool isLooping() const;

    /*!
        \brief Enable / disable fullscreen mode.
    */
    void setFullscreen(bool fullscreen);
    
    /*!
        \brief Query fullscreen mode.
    */    
    bool isFullscreen() const;

Q_SIGNALS:

    void videoReady();

public Q_SLOTS:
    /*!
        \brief Start video playback.
    */
    void play();

    /*!
        \brief Pause video playback.

        When resuming the video playback with call to MVideoWidget::play(), 
        video continues to play from the position it was left when pausing.
    */
    void pause();

    /*!
        \brief Stop video playback.

        The video is rewinded to the beginning.
    */
    void stop();

protected Q_SLOTS:

    //! \reimp
    virtual void updateData(const QList<const char*>& modifications);
    //! \reimp_end

protected:

    //! \cond
    MVideoWidget(MVideoWidgetPrivate* dd, MVideoWidgetModel* model, QGraphicsItem *parent);
    //! \endcond

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

private:

    Q_DECLARE_PRIVATE(MVideoWidget)
    Q_DISABLE_COPY(MVideoWidget)
};
#endif // MVIDEOWIDGET_H
