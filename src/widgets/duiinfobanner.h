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

#ifndef DUIINFOBANNER_H
#define DUIINFOBANNER_H

#include <duiscenewindow.h>
#include <duiinfobannermodel.h>

class DuiInfoBannerPrivate;

/*!
    \class DuiInfoBanner
    \brief A class that represents in-process notification banner.

    \ingroup widgets

    \section DuiInfoBannerOverview Overview
        - Use DuiInfoBanner class to launch in-process notifications.
        - Use DuiNotification class to launch out-of-process system level notifications.

    \section DuiInfoBannerVariants Variants
        - \link DuiInfoBannerEventView Event banner \endlink
        - \link DuiInfoBannerInformationView Information banner \endlink (for status, progress and errors)

    \section DuiInfoBannerExamples Examples
        Here's how to launch an event banner from code:
        \code
            DuiInfoBanner* infoBanner = new DuiInfoBanner(DuiInfoBanner::Event,
                                                    "Image-ida",
                                                    "<b>Ida Taipale</b><br/>Have you seen my dog?",
                                                    "Icon-new-SMS");
            connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(DuiSceneWindow::DestroyWhenDone);
            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode

        Here's how to launch an information banner from code:
        \code
            DuiInfoBanner* infoBanner = new DuiInfoBanner(DuiInfoBanner::Information,
                                                    "Icon-close",
                                                    "<b>Battery is running low</b>",
                                                    "");
            connect(infoBanner, SIGNAL(clicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(DuiSceneWindow::DestroyWhenDone);
            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode

        Here's how to launch an interactive information banner from code:
        \code
            DuiInfoBanner* infoBanner = new DuiInfoBanner(DuiInfoBanner::Information,
                                                    "icon-l-chat",
                                                    "<b>Incoming call</b>",
                                                    "");
            infoBanner->setButtonText("Accept");
            connect(infoBanner, SIGNAL(buttonClicked()), this, SLOT(openMessageBox()));
            infoBanner->appear(DuiSceneWindow::DestroyWhenDone);

            QTimer::singleShot(3000, infoBanner, SLOT(disappear()));
        \endcode
    \sa DuiNotification
*/

class DUI_EXPORT DuiInfoBanner : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiInfoBanner)
public:

    /*!
        \property DuiInfoBanner::iconID
        \brief Icon for banner.
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /*!
        \property DuiInfoBanner::bodyText
        \brief Body text for banner.
    */
    Q_PROPERTY(QString bodyText READ bodyText WRITE setBodyText)

    /*!
        \property DuiInfoBanner::imageID
        \brief Image for banner.
    */
    Q_PROPERTY(QString imageID READ imageID WRITE setImageID)

    /*!
        \property DuiInfoBanner::buttonText
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
        \var DuiInfoBanner::informationType
        \brief View type for information type banner
    */
    static const DuiTheme::ViewType informationType;
    /*!
        \var DuiInfoBanner::informationType
        \brief View type for event type banner
    */
    static const DuiTheme::ViewType eventType;

    /*!
        \brief Constructs a new banner object of given type.
        \param type          Type of the banner
    */
    DuiInfoBanner(BannerType type = Information);

    /*!
        Destructor for banner object
    */
    virtual ~DuiInfoBanner();

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
    Q_DISABLE_COPY(DuiInfoBanner)
    Q_DECLARE_PRIVATE(DuiInfoBanner)

#ifdef UNIT_TEST
    friend class Ut_DuiInfoBanner;
#endif
};

#endif
