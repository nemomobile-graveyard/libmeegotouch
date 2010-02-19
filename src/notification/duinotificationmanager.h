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

#ifndef DUINOTIFICATIONMANAGER_H_
#define DUINOTIFICATIONMANAGER_H_

#include "duinotificationmanagerproxy.h"

//! \internal

/*!
 * An interface to the notification manager.
 * Uses the org.maemo.dui.NotificationManager D-Bus interface with which
 * application developers can create and manage notifications.
 */
class DuiNotificationManager
{
private:
    /*!
     * Constructs an interface for the notification manager.
     */
    DuiNotificationManager();

    /*!
     * Destroys an interface for the notification manager.
     */
    virtual ~DuiNotificationManager();

    //! The D-Bus proxy object for the notification manager interface.
    DuiNotificationManagerProxy proxy;

    //! The notification user ID
    uint userId;

public:
    /*!
     * Returns an instance of the notification manager interface.
     *
     * \return an instance of the notification manager interface
     */
    static DuiNotificationManager *instance();

    /*!
     * Adds a new notification group.
     *
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     * \return the ID of the new notification group
     */
    uint addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent);

    /*!
     * Adds a new notification group.
     *
     * \param eventType the event type of the notification
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     * \return the ID of the new notification group
     */
    uint addGroup(const QString &eventType, bool persistent = false);

    /*!
     * Adds a new notification.
     *
     * \param groupId the ID of the notification group to put the notification in
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     * \return the ID of the new notification
     */
    uint addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count = 1, bool persistent = false);

    /*!
     * Adds a new notification.
     *
     * \param groupId the ID of the notification group to put the notification in
     * \param eventType the event type of the notification
     * \param persistent \c true if the notification group should be persistent, \c false otherwise
     * \return the ID of the new notification
     */
    uint addNotification(uint groupId, const QString &eventType, bool persistent = false);

    /*!
     * Removes a notification group and all notifications in the group.
     *
     * \param groupId the ID of the notification group to be removed
     * \return true if the removal succeeded, false otherwise
     */
    bool removeGroup(uint groupId);

    /*!
     * Removes a notification.
     *
     * \param notificationId the ID of the notification to be removed
     * \return true if the removal succeeded, false otherwise
     */
    bool removeNotification(uint notificationId);

    /*!
     * Updates an existing notification group.
     *
     * \param groupId the ID of the notification group to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this group
     * \return true if the update succeeded, false otherwise
     */
    bool updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

    /*!
     * Updates an existing notification group.
     *
     * \param groupId the ID of the notification group to be updated
     * \param eventType the event type of the notification
     * \return true if the update succeeded, false otherwise
     */
    bool updateGroup(uint groupId, const QString &eventType);

    /*!
     * Updates an existing notification.
     *
     * \param notificationId the ID of the notification to be updated
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \return true if the update succeeded, false otherwise
     */
    bool updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count = 1);

    /*!
     * Updates an existing notification.
     *
     * \param notificationId the ID of the notification to be updated
     * \param eventType the event type of the notification
     * \return true if the update succeeded, false otherwise
     */
    bool updateNotification(uint notificationId, const QString &eventType);

    /*!
     * Returns a list of IDs for notifications owned by this application
     *
     * \return list of notification IDs
     */
    QList<uint> notificationIdList();
};

//! \internal_end

#endif /* DUINOTIFICATIONMANAGER_H_ */
