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

#ifndef MBANNER_H
#define MBANNER_H

#include <mscenewindow.h>
#include <mbannermodel.h>

/*!
    \class MBanner
    \brief A class that represents in-process notification banner.

    \ingroup widgets

    \section BannerOverview Overview

    MBanner is a component used by the system for different purposes related with
    the notifications. Depending on the elements that you use when setting up an MBanner,
    the component will have different appearances.

    Although we haven't defined different views for the MBanner, we consider those three types:

    \li An event banner is a MBanner with an icon, title and subtitle.
    \li An information banner is a MBanner with a subtitle (only one label)
    \li An system banner is a MBanner with an icon an subtitle

     You should set the stylename through setStyleName() with one of the following names:
        -"EventBanner": used for events as emails, sms, etc.
        -"InformationBanner": used to show generic information without icons.
        -"SystemBanner": reserved for system notifications as battery, USB, updates, etc.

    If you create a MBanner without styleName it will have a default style.
    In case that your banner contains any combination above mentioned, the styleName will
    be configured automatically by the library.

    Is highly recommended use a styleName contemplated in the mentioned categories. As
    a good practice <b>you should setup one of the mentioned stylenames</b> to avoid conflicts
    in the look and feel.

    By default MBanner is dismissed automatically after a certain amount of time. This
    parameter is defined in mbannerstyle.css under the name of disappear-timeout.

    The whole MBanner is interactive. If you click the MBanner before the diappear-timeout
    is reached it will be dismissed.

    \section notification_vs_banner MNotification versus MBanner

    Use MBanner class to launch in-process notifications.
    Use MNotification class to launch out-of-process system level notifications.

    \section MBannerExamples Examples

        Here's how to launch an event banner from code:
        \code
           MBanner *eventBanner = new MBanner();
           eventBanner->setStyleName("EventBanner");
           eventBanner->setIconID("icon-l-settings");
           eventBanner->setTitle("New updates waiting to install");
           eventBanner->setSubtitle("130 files");
           eventBanner->appear(MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch an information banner from code:
        \code
           MBanner *infoBanner = new MBanner();
           infoBanner->setStyleName("InformationBanner");
           infoBanner->setSubtitle("Info banner with so much information that the text wraps in portrait");
           infoBanner->appear(MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch a system banner from code:
        \code
          MBanner *systemBanner = new MBanner();
          systemBanner->setStyleName("SystemBanner");
          systemBanner->setIconID("icon-m-telephony-call-answer");
          systemBanner->setSubtitle("System banner");
          systemBanner->appear(MSceneWindow::DestroyWhenDone);
        \endcode

    \sa MNotification
*/

class M_EXPORT MBanner : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MBanner)
public:

    /*!
        \property MInfoBanner::iconID
        \brief Icon for banner.
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /*!
        \property MContentItem::title
        \brief See MContentItemModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \property MContentItem::subtitle
        \brief See MContentItemModel::subtitle
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

public:

    /*!
        \brief Constructs a new banner
    */

    MBanner();

    /*!
        Destructor for banner object
    */
    virtual ~MBanner();

    /*!
        \brief Returns the icon ID of the banner
    */
    QString iconID() const;

    /*!
        \brief clicks info banner
    */
    void click();

    /*!
        \brief Get the title.
     */
    QString title() const;

    /*!
        \brief Get the subtitle.
     */
    QString subtitle() const;

Q_SIGNALS:

    /*!
        \brief This signal is emitted when the banner is activated.
    */
    void clicked();

public Q_SLOTS:

    /*!
        \brief Sets the icon for the banner.
        \param iconId Icon ID for the banner (for event banners)
    */
    void setIconID(const QString &iconID);

    /**
        \brief Set title text. This is first line.
        \param text text.
     */
    void setTitle(const QString &text);

    /**
        \brief Set subtitle text. This is second line.
        \param text text.
     */
    void setSubtitle(const QString &text);

private:
    Q_DISABLE_COPY(MBanner)

#ifdef UNIT_TEST
    friend class Ut_MBanner;
#endif
};

#endif
