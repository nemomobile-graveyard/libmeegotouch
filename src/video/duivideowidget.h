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

#include <duiwidget.h>

#include <gst/gst.h>
#include <gst/gstvalue.h>
#include <gst/video/video.h>

#include "duisink.h"

class DuiVideoWidgetPrivate;

// Temporarily removed from build
//! \internal

/*!
 * \class DuiVideoWidget
 * \brief DuiVideoWidget is a graphical widget displaying videostream from gstreamer sink.
 *
 * Video can be paused, seeked and scaled from original size.
 * Please note that every new DuiVideoWidget creates another worker thread
 * for video processing and may cause performance overhead is several videos
 * are running at once.
 *
 * Note that in you application main you *must* initialize gstreamer
 * subsystem by calling gst_init(argc, argv); and gst_deinit() after app.exec()
 *
 * Example:
 * \verbatim
 *
 * // gst_init called with the arguments given to application
 * gst_init(&argc, &argv);
 *
 * DuiVideoWidget video;
 * video.open("videos/ninja.avi");
 * layout()->addItem(video, x, y)
 * seek(video.length()/2); // seek to the middle of video
 *
 * app.exec();
 *
 * // gst_deinit called before returning from main
 * gst_deinit();
 *
 * \endverbatim
 */
class DUI_EXPORT DuiVideoWidget : public DuiWidget
{
    Q_OBJECT
public:

    /*!
     * \brief Constructor
     * \param parent Parent widget
     */
    DuiVideoWidget(QGraphicsItem *parent = NULL);

    /*!
     * \brief Destructor
     */
    virtual ~DuiVideoWidget();

    /*!
     * Open a video file to be streamed
     * \param filename a relative path to the file
     * \param autoPlay If true playback of the video is started automatically
     *                 when the video has been successfully opened. If false
     *                 only the first frame of the video is buffered.
     * \return boolean value to check if method succeeded.
     */
    bool open(const QString &filename, bool autoPlay = false);

    /*!
     * Start playback
     */
    void play();

    /*!
     * Pause playback
     */
    void pause();

    /*!
     * Stop playback
     */
    void stop();

    /*!
     * Reset playback (seeks to start of the video stream)
     * \return boolean value if method succeeded.
     */
    bool reset();

    /*!
     * Check the playback state
     * \return boolean value if widget is playing a video
     */
    bool isPlaying();

    /*!
     * Seek to a defined position in the video stream
     * \param time time in msecs
     * \return boolean value if method succeeded.
     */
    bool seek(qint64 time);

    /*!
     * Check the length of the video stream
     * \return length of the stream in msecs.
     */
    qint64 length();

    /*!
     * Check the current position of the video.
     * \return Current position of the video.
     */
    qint64 currentPos();

    /*!
     * Set the volume for the video.
     * \param volume Volume for the video in the range of 0.0 -> 1.0.
     */
    void setVolume(qreal volume);

    /*!
     * Get the current volume.
     * \return Current volume.
     */
    qreal volume();

    /*!
     * Mute/unmute audio of the video.
     * \param mute If true the audio is muted, if false the audio is unmuted.
     */
    void mute(bool mute);

    /*!
     * Check mute status.
     * \returns True if the audio is muted and false if the audio is unmuted.
     */
    bool muted();

    /*!
     * Snapshot the current video frame.
     * \returns image of current video frame
     */
    QImage currentFrame();

    /*!
     * Enables/disables scaling of the video.
     * \param scaling If true the video is scaled depending of the size
     *                      of the widget. If false the video is always rendered
     *                      in it's native resolution.
     */
    void setScaling(bool scaling);

    /*!
     * \returns boolean state of scaling
     */
    bool scaling();

    /*!
     * Enables/disables locking of the aspect ratio.
     * \param keepAspectRatio If true the original aspect ratio of the video will
     *                        preserved when scaling. If false the aspect ratio
     *                        is not preserved and the video is scaled to match
     *                        exactly the size of the widget.
     */
    void setKeepAspectRatio(bool keepAspectRatio);

    /*!
     * \returns boolean state of keepAspectRatio
     */
    bool keepAspectRatio();

    /*!
     * Enables / disables filling of background of the widget. Background is filled
     * only if the size of the video (after scaling) is smaller than the widget
     * itself.
     * \param fillBg If true the background of the widget will be filled to the
     *               wanted color when the video is not filling the whole widget
     *               area. If false only the video frame is rendered.
     */
    void setBackgroundFill(bool fillBg);

    /*!
     * \returns boolean status of background filling
     */
    bool backgroundFill();

    /*!
     * Set the background color. The color is black by default.
     * \param color  New color for the background.
     */
    void setBackgroundColor(const QColor &color);

    /*!
     * \returns the current background color
     */
    QColor backgroundColor();

    /*!
     * Enable / disable automatica repeating of the video. Repeating is disabled
     * by default.
     * \param repeat If true video will be restarted from the beginning automatically.
                     If false video will be stopped when the end is reached.
     */
    void setRepeat(bool repeat);

    /*!
     * \ returns boolean state of repeat
     */
    bool repeat();

    /*!
     * Get the name of the video that has been previously opened with open() method.
     * The returned filename is absolute path to the video file.
     * \return Filename of the previously opened video.
     */
    QString videoFilename();

    //TODO figure workaround for this
    //(emit signal from private class and route it to videoReady() signal)
    void emitVideoReady();

Q_SIGNALS:
    void clicked();
    void videoReady();

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void constructPipeline();

    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    //! \reimp_end

private:

    //! Pointer to private implementation class object.
    DuiVideoWidgetPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(DuiVideoWidget)

#ifdef UNIT_TEST
    friend class ft_duivideowidget;
#endif
};
//! \internal_end

#endif // DUIVIDEOWIDGET_H
