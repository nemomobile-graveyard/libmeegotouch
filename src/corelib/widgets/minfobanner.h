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

#ifndef MINFOBANNER_H
#define MINFOBANNER_H

#include <mscenewindow.h>
#include <minfobannermodel.h>

class MInfoBannerPrivate;

/*!
    \class MInfoBanner
    \brief A class that represents in-process notification banner.

    \ingroup widgets

    \section MInfoBannerOverview Overview
        - Use MInfoBanner class to launch in-process notifications.
        - Use MNotification class to launch out-of-process system level notifications.

    \section MInfoBannerVariants Variants
        - \link MInfoBannerEventView Event banner \endlink
        - \link MInfoBannerInformationView Information banner \endlink (for status, progress and errors)

    \section MInfoBannerExamples Examples
        Here's how to launch an event banner from code:
        \code
            MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Event);
            infoBanner->setImageID("icon-l-email");
            infoBanner->setBodyText(
                //% "<b>Ida Taipale</b><br/>Have you seen my dog?"
                qtTrId("xx_dialogs_and_notifications_event_banner"));
            infoBanner->setIconID("icon-m-toolbar-send-sms");
            connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(MSceneWindow::DestroyWhenDone);
            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode

        Here's how to launch an information banner from code:
        \code
            MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
            infoBanner->setImageID("icon-l-help");
            infoBanner->setBodyText(
                //% "<b>Battery is running low</b>"
                qtTrId("xx_dialogs_and_notifications_information_banner"));
            connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(MSceneWindow::DestroyWhenDone);
            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode

        Here's how to launch an interactive information banner from code:
        \code
            MInfoBanner *infoBanner = new MInfoBanner(MInfoBanner::Information);
            infoBanner->setImageID("icon-l-office-tools");
            infoBanner->setBodyText(
                //% "<b>Incoming call</b>"
                qtTrId("xx_dialogs_and_notifications_system_information_banner"));
            //% "Accept"
            infoBanner->setButtonText(qtTrId("xx_dialogs_and_notifications_system_information_banner_accept"));
            connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(MSceneWindow::DestroyWhenDone);

            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode
    \sa MNotification

    \deprecated MInfoBanner is deprecated, use MBanner for any component with banner requirements
*/

class M_CORE_EXPORT MInfoBanner : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MInfoBanner)
public:

    /*!
        \property MInfoBanner::iconID
        \brief Icon for banner.
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /*!
        \property MInfoBanner::bodyText
        \brief Body text for banner.
    */
    Q_PROPERTY(QString bodyText READ bodyText WRITE setBodyText)

    /*!
        \property MInfoBanner::imageID
        \brief Image for banner.
    */
    Q_PROPERTY(QString imageID READ imageID WRITE setImageID)

    /*!
        \property MInfoBanner::buttonText
        \brief Button text for interactive information banner.
    */
    Q_PROPERTY(QString buttonText READ buttonText WRITE setButtonText)

public:

    /*!
     * Defines the type of the banner. Event is used for messages & events. Information for
     * i.e. system notifications ("Battery low")
     */
    enum BannerType {
        Information,
        Event
    };

    /*!
        \var MInfoBanner::informationType
        \brief View type for information type banner
    */
    static const MTheme::ViewType informationType;
    /*!
        \var MInfoBanner::informationType
        \brief View type for event type banner
    */
    static const MTheme::ViewType eventType;

    /*!
        \brief Constructs a new banner object of given type.
        \param type          Type of the banner
    */
    MInfoBanner(BannerType type = Information);

    /*!
        Destructor for banner object
    */
    virtual ~MInfoBanner();

    /*!
        \brief Returns the type of the banner.
        \return the type of the banner
    */
    BannerType bannerType();

    /*!
        \brief Sets the image for the banner.
        \param image Image ID for the banner
    */
    void setImageID(const QString &imageID);

    /*!
        \brief Returns the image ID of the banner
    */
    QString imageID() const;

    /*!
        \brief Sets the pixmap for the banner.
        \param user defined pixmap for the banner
    */
    void setPixmap(const QPixmap &pixmap);

    /*!
        \brief Returns the user defined pixmap
    */
    QPixmap pixmap() const;

    /*!
        Sets the body text for the banner.
        \param body Body text for the banner
    */
    void setBodyText(const QString &bodyText);

    /*!
        \brief Returns the body text of the banner
    */
    QString bodyText() const;

    /*!
        \brief Sets the icon for the banner.
        \param iconId Icon ID for the banner (for event banners)
    */
    void setIconID(const QString &iconID);

    /*!
        \brief Returns the icon ID of the banner
    */
    QString iconID() const;

    /*!
        Sets the text for the information banner button.
        \param text for the button
    */
    void setButtonText(const QString &text);

    /*!
     * \brief Returns the text of the information banner button
     */
    QString buttonText() const;

    /*!
        \brief clicks info banner
    */
    void click();

Q_SIGNALS:
    /*!
        \brief This signal is emitted when the banner is activated.
    */
    void clicked();

    /*!
        \brief This signal is emitted when the button in banner is clicked.
    */
    void buttonClicked();

private:
    Q_DISABLE_COPY(MInfoBanner)
    Q_DECLARE_PRIVATE(MInfoBanner)

#ifdef UNIT_TEST
    friend class Ut_MInfoBanner;
#endif
};

#endif
