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

#ifndef MINFOBANNEREVENTVIEW_H
#define MINFOBANNEREVENTVIEW_H

#include "mscenewindowview.h"
#include "minfobannermodel.h"
#include "minfobannerstyle.h"

class MInfoBanner;
class MInfoBannerEventViewPrivate;

/*!
    \class MInfoBannerEventView
    \brief A class that represents a modeless notification banner.

    \ingroup views

    \section MInfoBannerEventViewOverview Overview
        - An event banner is a modeless notification banner that appears on screen on top of the given UI
        (all but in certain exception cases, like full screen video playback).
        - An event banner is used for events that should be persistent: i.e. if the user does not click on
        the event banner when it is shown, they will be shown on the Home screen.
        - The banner disappears from the screen after a timeout, after the timeout the incoming event can
        be found from the Home view.
        - The event banner has an action associated to it: usually tapping the notification opens the item
        in a new task.
        - If the application view where a new event occurs is in the foreground, no extra notification
        should displayed, the update should be shown directly in the foreground UI.
        - If there is several information updates happening at the same time the incoming event banners should
        be shown one after another.
        - The event banners should not overlap the virtual keyboard: if the VKB is
        open, the preview should be positioned above the keyboard.
        - Event banners have (currently) a fixed size.

        \image html eventbanner.png

    \section MInfoBannerEventViewInteractions Interactions
        <TABLE border="1">
        <TR><TH>Event</TH><TH>Action</TH><TH>Transition</TH><TH>Tactile feedback</TH><TH>Audio feedback</TH></TR>
        <TR><TD>-</TD>
            <TD>Appear on screen</TD>
            <TD>Appearing from the background ont the foreground on the defined spot on screen</TD>
            <TD>OPEN: tactile feedback?</TD>
            <TD>OPEN: audio feedback?</TD></TR>
        <TR><TD>Ignore, do nothing</TD>
            <TD>Notification is ignored</TD>
            <TD>Fading on to the background moving at the same time on the Home button -- "droppping on Home"</TD>
            <TD>OPEN: tactile feedback?</TD>
            <TD>OPEN: audio feedback?</TD></TR>
        <TR><TD>Drag notification away from the screen</TD>
            <TD>Notification is dismissed</TD>
            <TD>OPEN: transition?</TD>
            <TD>Cancel</TD>
            <TD>OPEN: audio feedback?</TD></TR>
        <TR><TD>Single tap on the notification</TD>
            <TD>Open item in new task</TD>
            <TD>Transition showing a new task opening</TD>
            <TD>Press, release</TD>
            <TD>Press, release</TD></TR>
        </TABLE>

    \sa MNotification
*/

class M_VIEWS_EXPORT MInfoBannerEventView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MInfoBannerModel, MInfoBannerStyle)

protected:
    MInfoBannerEventViewPrivate *const d_ptr;

public:
    /*!
        \brief Constructor
        \param controller  Pointer to the banner's controller
    */
    MInfoBannerEventView(MInfoBanner *controller);

    /*!
        \brief Destructor
    */
    virtual ~MInfoBannerEventView();

protected:
    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void setupModel();
    virtual void applyStyle();
    //! \reimp_end

    /*!
        \brief Constructor for derived classes
        \param dd  Private class implementation
        \param controller  Pointer to the banner's controller
    */
    MInfoBannerEventView(MInfoBannerEventViewPrivate &dd, MInfoBanner *controller);

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MInfoBannerEventView)
};

#endif

