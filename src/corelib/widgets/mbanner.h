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

class MBannerPrivate;


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
    \li An information banner is a MBanner with a title
    \li An system banner is a MBanner with a title and an icon


     You should set the stylename through setStyleName() with one of the following names:
    \li "ShortEventBanner": used for events as emails, sms, etc. Icon, title and subtitle.
    \li "InformationBanner": used to show generic information for apps with icons.
    \li "SystemBanner": reserved for system notifications, only text.

    If you create a MBanner without styleName it will have a default style, and it will have the
    capability to show any property: title, subtitle, etc.
    In case that your banner contains any combination above mentioned, the styleName will
    be configured automatically by the library.

    Due to the MBanner has the capability to be shown at the events/lock screen, it comes with two
    styles for this use case:

    \li FullEventBanner
    \li PrivateEventBanner

    The difference between FullEventBanner and PrivateEventBanner is that the second doesn't show
    the subtitle due to privacy reasons.

    MBanner can switch to different styles in anytime only switching the stylename.
    Note that some styles contain some specific layouts in which some properties are not show.

    For instance if you create a Short Event Banner:
       \code
           MBanner *eventBanner = new MBanner();
           eventBanner->setStyleName("ShortEventBanner");
           eventBanner->setIconID("icon-l-settings");
           eventBanner->setTitle("New updates waiting to install");
           eventBanner->setSubtitle("130 files");
           eventBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
       \endcode

    If you want show the look and feel of this MBanner in the event screen you only need switch
    the stylename:
       \code
           eventBanner->setStyleName("FullEventBanner");
       \endcode

    The MBanner layout will be styled with the FullEventBanner's layout automatically.

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
           eventBanner->setStyleName("ShortEventBanner");
           eventBanner->setIconID("icon-l-settings");
           eventBanner->setTitle("New updates waiting to install");
           eventBanner->setSubtitle("130 files");
           eventBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch an information banner from code:
        \code
           MBanner *infoBanner = new MBanner();
           infoBanner->setStyleName("InformationBanner");
           infoBanner->setTitle("Info banner with so much information that the text wraps in portrait");
           infoBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch a system banner from code:
        \code
          MBanner *systemBanner = new MBanner();
          systemBanner->setStyleName("SystemBanner");
          systemBanner->setIconID("icon-m-common-usb");
          systemBanner->setTitle("System banner");
          systemBanner->appear(scene(), MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch a event full banner from code:
        \code
          MBanner *fullEventBanner = new MBanner();
          fullEventBanner->setStyleName("FullEventBanner");
          fullEventBanner->setIconID("icon-m-camera-scene-landscape-selected");
          fullEventBanner->setTitle("Call");
          fullEventBanner->setSubtitle("Brianna Edwards");
          fullEventBanner->setPrefixTimeStamp("Yesterday:");
          fullEventBanner->setBannerTimeStamp(QDateTime::currentDateTime());
        \endcode

    \sa MNotification
*/

class M_CORE_EXPORT MBanner : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MBanner)
public:

    /*!
        \property MBanner::iconID
        \brief Icon for banner.
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /*!
        \property MBanner::title
        \brief MBanner has a title, is the primary text
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \property MBanner::subtitle
        \brief MBanner can have a subtitle, is the secondary text
    */
    Q_PROPERTY(QString subtitle READ subtitle WRITE setSubtitle)

    /*!
        \property MBanner::bannerTimeStamp
        \brief MBanner can have a timestamp
    */
    Q_PROPERTY(QDateTime bannerTimeStamp READ bannerTimeStamp WRITE setBannerTimeStamp)

    /*!
        \property MBanner::prefixTimeStamp
        \brief MBanner can have the capability to setup a prefix before the timestamp
         Example: Yesterday: 18:00 PM
    */
    Q_PROPERTY(QString prefixTimeStamp READ prefixTimeStamp WRITE setPrefixTimeStamp)

    /*!
        \property MBanner::down
        \brief See MBannerModel::down
    */
    Q_PROPERTY(bool down READ isDown WRITE setDown DESIGNABLE false)

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
        \brief clicks mbanner
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

    /*!
        \brief Get the timestamp.
     */
    QDateTime bannerTimeStamp() const;

    /*!
        \brief Get the prefix of the timestamp.
     */
    QString prefixTimeStamp() const;

    /*!
        \brief Get the pixmap of the mbanner
    */
    QPixmap pixmap() const;

    /*!
      \brief Returns true if the banner is pressed down.
    */
    bool isDown() const;

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
        \param Title text.
     */
    void setTitle(const QString &text);

    /**
        \brief Set subtitle text. This is second line.
        \param Subtitle text.
     */
    void setSubtitle(const QString &text);

    /**
        \brief Set timestamp text. This is third line.
        \param Timestamp QDateTime.
     */
    void setBannerTimeStamp(const QDateTime &date);

    /**
        \brief Set prefix timestamp text. This is third line before the timestamp.
        \param Prefix text.
     */
    void setPrefixTimeStamp(const QString &text);

    /**
        \brief Sets a pixmap for the banner.
        \param pixmap
    */
    void setPixmap(const QPixmap &pixmap);

    /*!
      \brief Set the banner down state.
    */

    void setDown(bool);

private:
    Q_DISABLE_COPY(MBanner)
    Q_DECLARE_PRIVATE(MBanner)

#ifdef UNIT_TEST
    friend class Ut_MBanner;
#endif
};

#endif
