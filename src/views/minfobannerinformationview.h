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

#ifndef MINFOBANNERINFORMATIONVIEW_H
#define MINFOBANNERINFORMATIONVIEW_H

#include "mscenewindowview.h"
#include "minfobannermodel.h"
#include "minfobannerstyle.h"

class MInfoBanner;
class MInfoBannerInformationViewPrivate;

/*!
    \class MInfoBannerInformationView
    \brief A class that represents a modeless notification banner.

    \ingroup views

    \section MInfoBannerInformationViewOverview Overview
        - Information banners are UI elements that allow the device to communicate information that the
          foreground UI is not capable of communicating.
        - All information banners are modeless and temporal elements, disappearing after a timeout (or a
          given time period).
        - Information banners can include Progress notification elements within themselves.
        - Information banners differ from Incoming event banners that they do not appear on the Home screen
          notification area.
        - Information banners are not interactive, any click onto it dismisses the status banner.
        - Information banners can include Labels and Images.
        - Information banners determine their size by the amount of contents that they need to display.
        - Information banner should not overlap the virtual keyboard: if the VKB is open, the status banner
          should be positioned above the keyboard.

        \image html infobanner.png

    \section MInfoBannerInformationViewInteractions Interactions
        <TABLE border="1">
        <TR><TH>Event</TH><TH>Action</TH><TH>Transition</TH><TH>Tactile feedback</TH><TH>Audio feedback</TH></TR>
        <TR><TD>-</TD>
            <TD>Appear on screen</TD>
            <TD>Appearing from the background ont the foreground on the defined spot on screen</TD>
            <TD>(optional)</TD>
            <TD>(optional)</TD></TR>
        <TR><TD>Ignore, do nothing</TD>
            <TD>Up to the designer, default: disappear from screen</TD>
            <TD>Fading on to the background</TD>
            <TD>OPEN: tactile feedback?</TD>
            <TD>OPEN: audio feedback?</TD></TR>
        <TR><TD>Click on the status banner</TD>
            <TD>Disappear from the screen</TD>
            <TD>Fading to the background</TD>
            <TD>No feedback</TD>
            <TD>No feedback</TD></TR>
        </TABLE>

    \section MInfoBannerInteractiveOverview Interactive information banner
        - An interactive information banner extends the status banner by providing a functionality associated
          with the status banner.
        - An interactive information banner can have only one function associated to it. The button can be
          enabled using MInfoBanner::setButtonText method.
        - Interactive information banners determine their size by the amount of contents that they need to display.
        - An interactive information banner may also be used for cancellable operations, containing a "Cancel"
          button for canceling the process.
            - Note that the interactive information banner is still modeless: if you need to have a modal
              progress operation, use a variant of the Dialog component.
            - Do not hack the interactive information banner to be modal!
        - Interactive information banner should not overlap the virtual keyboard: if the VKB is open, the
          interactive information banner should be positioned above the keyboard.

        \image html interactiveinfobanner.png

    \section MInfoBannerInteractiveInteractions Interactions
        <TABLE border="1">
        <TR><TH>Event</TH><TH>Action</TH><TH>Transition</TH><TH>Tactile feedback</TH><TH>Audio feedback</TH></TR>
        <TR><TD>-</TD>
            <TD>Appear on screen (location up to designer)</TD>
            <TD>OPEN: Transition?</TD>
            <TD>OPEN: tactile feedback?</TD>
            <TD>OPEN: audio feedback?</TD></TR>
        <TR><TD>Ignore, do nothing</TD>
            <TD>Remain in the screen until the process is finished (no defined timeout time)</TD>
            <TD>-</TD>
            <TD>None, or if the process takes long time a tactile feedback is used to show user that it has finished</TD>
            <TD>-</TD></TR>
        <TR><TD>Single tap on button inside notification</TD>
            <TD>The functionality has to be defined by application designer</TD>
            <TD>Depending on the function</TD>
            <TD>Press, release</TD>
            <TD>Press, release</TD></TR>
        <TR><TD>Click on the interactive status banner (outside the button)</TD>
            <TD>Disappear from the screen</TD>
            <TD>Fading to the background</TD>
            <TD>No feedback</TD>
            <TD>No feedback</TD></TR>
        </TABLE>

    \sa MNotification
*/
class M_VIEWS_EXPORT MInfoBannerInformationView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MInfoBannerModel, MInfoBannerStyle)

protected:
    MInfoBannerInformationViewPrivate *const d_ptr;

public:
    /*!
        \brief Constructor
        \param controller  Pointer to the banner's controller
    */
    MInfoBannerInformationView(MInfoBanner *controller);

    /*!
        \brief Destructor
    */
    virtual ~MInfoBannerInformationView();

    //! \reimp
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

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
    MInfoBannerInformationView(MInfoBannerInformationViewPrivate &dd, MInfoBanner *controller);

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end
private:
    Q_DISABLE_COPY(MInfoBannerInformationView)
    Q_DECLARE_PRIVATE(MInfoBannerInformationView)
};

#endif

