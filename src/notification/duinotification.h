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

#ifndef DUINOTIFICATION_H_
#define DUINOTIFICATION_H_

#include "duinotificationgroup.h"

#include <DuiExport>
#include <QtGlobal>
#include <duiremoteaction.h>

class DuiNotificationPrivate;
class QString;

/*!
    \class DuiNotification
    \brief A class that represents a system notification.

    \ingroup widgets

    \section DuiNotificationOverview Overview
        Notifications provide means to communicate system information and status updates to the user,
        without blocking the UI or requiring the user to switch to another view. Notifications can be
        done through visual, audio or haptic means.

        Dialogs and notifications are related to each other, but separate components. All Dialogs are modal,
        i.e. elements that do not go away from the screen without requiring user interactions, whereas all
        notifications are temporal elements (i.e. modeless) that do not require interactions from the user
        (although they can sometimes also provide them).

        Certain notifications are related to the communication activities (like SMS, email, feed updates etc.),
        and notifications relating to application and system behaviour (like connection lost, battery low etc.).
        Communication activities display incoming event notifications.

        When an instance of this class is created a notification is created unless the instance represents an
        already existing notification and is created using a known notification ID.

        A list of notifications already created can be requested, but in this case a DuiApplication
        instance must have been created before any notifications were created and before the request
        itself.

        \sa \ref notifications

    \section DuiNotificationInteractions Interactions
        Notifications can be non-interactive (for information only) or interactive: the interactive
        notifications provide information, but they also provide a way for interacting with the notification,
        for example to fix a connectivity problem. Notifications are always modeless components - modal
        "notifications" are specified with Dialogs. Images on this page are for illustrational purposes only.
        In case that images and text conflict each other, the text should be followed.

    \section DuiNotificationUsageGuidelines Usage guidelines
        \li Notifications are intended to communicate information when the foreground application UI is not
            able to do this. If the foreground application UI can already communicate the information, a
            separate Notification shouldn't be shown anymore.
        \li Notifications are temporal elements, i.e. they are dismissed from screen after a certain time
            (or timeout).
        \li Incoming events aim to provide a centralized method for applications to provide persistent
            notifications to the users.
        \li Use non-interactive notifications (status banners) to display feedback/progress/notifications for
            successful actions, if other parts of the foreground UI cannot be directly utilized.
        \li Consider using interactive status banners for notifications in error cases, exceptions etc. that
            can either be dismissed or then reacted upon. The interactive elements are essentially shortcut
            functions, intended to save time from the users. No use case should fail because the user fails to
            press the interactive status banner.
        \li DuiInfoBanner class can be used to show in-process notifications directly without having to use the
            notification framework.

    \section DuiNotificationPersistence Persistence
        Notifications created as persistent are stored by the notification system and are returned by the
        DuiNotification::notifications() even after a reboot.

        \note A DuiApplication instance must be created before creating any persistent notifications.

    \section DuiNotificationVariants Variants
        The notification component has the following variants:
        - \link DuiInfoBannerEventView Event banner \endlink
        - \link DuiInfoBannerInformationView Information banner \endlink (for status, progress and errors)

    \section ExampleWidgetOpenIssues Open issues
        \li Rules about system modal notifications to be added.
        \li Now there is no separation between foreground application status banners vs. background and system
            status banners.
*/

class DUI_EXPORT DuiNotification
{
public:
    /*!
     * Creates a new representation of a notification. A notification is
     * automatically created and is given an ID by the notification manager.
     *
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification. Defaults to no summary text.
     * \param body the body text to be used in the notification. Defaults to no body text.
     * \param imageURI the ID of the icon to be used in the notification. Defaults to no image URI.
     * \param action the action to be executed when the notification is activated. Defaults to no action.
     * \param count the number of items inside this notification
     * \param persistent \c true if the notification should be persistent, \c false otherwise
     */
    explicit DuiNotification(const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &imageURI = QString(), const DuiRemoteAction &action = DuiRemoteAction(), uint count = 1, bool persistent = false);

    /*!
     * Creates a new representation of a notification. A notification is
     * automatically created into the given notification group and is given
     * an ID by the notification manager.
     *
     * \param group the notification group to put the notification in
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification. Defaults to no summary text.
     * \param body the body text to be used in the notification. Defaults to no body text.
     * \param imageURI the ID of the icon to be used in the notification. Defaults to no image URI.
     * \param action the action to be executed when the notification is activated. Defaults to no action.
     */
    explicit DuiNotification(const DuiNotificationGroup &group, const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &imageURI = QString(), const DuiRemoteAction &action = DuiRemoteAction());

    /*!
     * Creates a representation of an existing notification.
     * Should be used to get a handle to an existing notification
     * with a known ID.
     *
     * \param id the ID of the existing notification
     */
    explicit DuiNotification(unsigned int id);

    /*!
     * Destroys the class that represents a notification.
     */
    virtual ~DuiNotification();

    /*!
     * Returns the ID of the notification.
     *
     * \return the ID of the notification
     */
    unsigned int id() const;

    /*!
     * Updates the notification.
     *
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification. Defaults to no summary text.
     * \param body the body text to be used in the notification. Defaults to no body text.
     * \param imageURI the ID of the icon to be used in the notification. Defaults to no image URI.
     * \param count the number of items inside this notification
     * \param action the action to be executed when the notification is activated. Defaults to no action.
     * \return true if the update succeeded, false otherwise
     */
    bool update(const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &imageURI = QString(), uint count = 1, const DuiRemoteAction &action = DuiRemoteAction());

    /*!
     * Removes a notification.
     *
     * \return true if the removal succeeded, false otherwise
     */
    bool remove();

    /*!
     * Returns whether the notification is valid
     *
     * \return true if the notification is valid, false otherwise
     */
    bool isValid() const;

    /*!
     * Returns a list of notifications created by this application but which
     * have not been dismissed by the user yet. Caller of this function gets
     * the ownership of the notifications, and is responsible for freeing them.
     *
     * \return list of notifications
     */
    static QList<DuiNotification *> notifications();

protected:
    /*!
     * A constructor to be called by the derived classes.
     * \param dd a derived private class object.
     */
    DuiNotification(DuiNotificationPrivate &dd);

    //! A pointer to the private implementation class
    DuiNotificationPrivate *d_ptr;

    Q_DECLARE_PRIVATE(DuiNotification)

};

#endif /* DUINOTIFICATION_H_ */
