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

#ifndef DUIVIDEOWIDGET_H
#define DUIVIDEOWIDGET_H

#include <duiwidgetcontroller.h>
#include <duivideowidgetmodel.h>

class DuiVideoWidgetPrivate;


/*!
    \class DuiVideoWidget
    \brief DuiVideoWidget provides functionality for playing videos.

    \ingroup widgets

    \section DuiVideoWidgetOverview Overview
        DuiVideoWidget supports following features:
        
        - Standard playback controlling through play(), pause(), stop(), seek() 
        and setLooping() methods.
        - Volume controlling through the setVolume() and setMute() methods.
        - Automatic fitting and appearence of the video can be changed using the 
        setScaleToFit(), setKeepAspectRatio() and setFullscreen()
        methods.

    \section DuiVideoWidgetUseGuidelines Usage guidelines

    \section DuiVideoWidgetVariants Variants
        \li \link DuiVideoWidgetView Default view. \endlink Standard view for 
            displaying video frames without any visual controls.

    \section DuiVideoWidgetOpenIssues Open issues

    \section DuiVideoWidgetExamples Examples

    \sa DuiVideoWidgetModel
*/
class DUI_EXPORT DuiVideoWidget : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiVideoWidget)

public:

    /*!
      \brief Constructs a video widget.
    */
    DuiVideoWidget(QGraphicsItem *parent = 0, DuiVideoWidgetModel* model = 0);

    /*!
      \brief Destroys the video widget.
    */
    virtual ~DuiVideoWidget();

    /*!
        \brief Opens a video from a file.
    */
    void open(const QString& filename);

    /*!
        \brief Return the playback state.
    */
    DuiVideo::State state() const;

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

        If \a mode is DuiVideoWidgetModel::ScaleToFit the video scaled to fit the widget size. If \a mode 
        is DuiVideoWidgetModel::ScaleDisabled the video is rendered in it's native resolution into the middle 
        of the widget.
    */
    void setScaleMode(DuiVideoWidgetModel::Scale mode);

    /*!
        \brief Returns video scaling mode.
    */
    DuiVideoWidgetModel::Scale scaleMode() const;

    /*!
        \brief Set aspect ratio

        If \a aspectRatio is DuiVideoWidgetModel::AspectRatioOriginal, the video is scaled without breaking the 
        native aspect ratio. If \a aspectRatio is DuiVideoWidgetModel::AspectRatioScaled, the native aspect 
        ratio is not preserved and the video is scaled to match the size of the widget.
    */
    void setAspectRatioMode(DuiVideoWidgetModel::AspectRatio aspectRatio);

    /*!
        \brief Return type of the aspect ratio.
    */
    DuiVideoWidgetModel::AspectRatio aspectRatioMode() const;

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

        When resuming the video playback with call to DuiVideoWidget::play(), 
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
    DuiVideoWidget(DuiVideoWidgetPrivate* dd, DuiVideoWidgetModel* model, QGraphicsItem *parent);
    //! \endcond

    //! \reimp
    virtual void setupModel();
    //! \reimp_end

private:

    Q_DECLARE_PRIVATE(DuiVideoWidget)
    Q_DISABLE_COPY(DuiVideoWidget)
};
#endif // DUIVIDEOWIDGET_H
