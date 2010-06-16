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
        - Use MBanner class to launch in-process notifications.
        - Use MNotification class to launch out-of-process system level notifications.

    \section MBannerExamples Examples

        Here's how to launch an event banner from code:
        \code
           MBanner *infoBanner = new MBanner();
           infoBanner->setTitle("New updates waiting to install");
           infoBanner->setSubtitle("130 files");
           infoBanner->appear(MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch an information banner from code:
        \code
           MBanner *infoBanner = new MBanner();
           infoBanner->setSubtitle("Info banner with so much information that the text wraps in portrait");
           infoBanner->appear(MSceneWindow::DestroyWhenDone);
        \endcode

        Here's how to launch an system banner from code:
        \code
          MBanner *infoBanner = new MBanner();
          infoBanner->setIconID("icon-m-telephony-call-answer");
          infoBanner->setSubtitle("System banner");
          infoBanner->appear(MSceneWindow::DestroyWhenDone);
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
    friend class Ut_MInfoBanner;
#endif
};

#endif
