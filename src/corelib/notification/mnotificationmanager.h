/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MNOTIFICATIONMANAGER_H_
#define MNOTIFICATIONMANAGER_H_

#include "mnotificationmanagerproxy.h"
#include "mnotification.h"
#include "mnotificationgroup.h"

//! \internal

/*!
 * An interface to the notification manager.
 * Uses the com.meego.core.MNotificationManager D-Bus interface with which
 * application developers can create and manage notifications.
 */
class MNotificationManager
{
private:
    /*!
     * Constructs an interface for the notification manager.
     */
    MNotificationManager();

    /*!
     * Destroys an interface for the notification manager.
     */
    virtual ~MNotificationManager();

    //! The D-Bus proxy object for the notification manager interface.
    MNotificationManagerProxy proxy;

    //! The notification user ID
    uint userId;

public:
    /*!
     * Returns an instance of the notification manager interface.
     *
     * \return an instance of the notification manager interface
     */
    static MNotificationManager *instance();

    /*!
     * Returns a list of IDs for notifications owned by this application
     *
     * \deprecated this function has been deprecated in favor of notificationList() and will be removed in libmeegotouch 0.21.
     * \return list of notification IDs
     */
    QList<uint> notificationIdList();

    /*!
     * Returns a list of notifications owned by this application
     *
     * \return list of notifications
     */
    QList<MNotification> notificationList();

    /*!
     * Returns a list of notification groups owned by this application
     *
     * \return list of notification groups
     */
    QList<MNotificationGroup> notificationGroupList();

    /*!
     * Returns amount of notifications in a given group
     *
     * \param groupId the group ID
     * \return amount of notifications in given group
     */
    uint notificationCountInGroup(uint groupId);

    /*!
     * Add a new notification.
     *
     * If groupKey is defined notification is added to group.
     *
     * \param parameters a hash of key-value pairs containing information of notification.
     * \return id of created notification. If notification creation succeeded id is bigger than 0.
     */
    uint addNotification(uint groupId, const QVariantHash  &parameters);

    /*!
     * Updates a notification.
     *
     * notificationIdKey for the notification to be updated must be defined.
     * Replaces all defined keys. Keys that are not defined won't be replaced.
     * timestampKey is updated to system's current time if not defined.
     *
     * \param parameters a hash of key-value pairs containing information of notification.
     * \return true is returned if succeeded, false otherwise
     */
    bool updateNotification(uint notificationId, const QVariantHash  &parameters);

    /*!
     * Removes a notification
     *
     * notificationId of the removable notification must be defined
     *
     * \param parameters a hash of key-value pairs containing information of notification.
     * \return true is returned if succeeded, false otherwise
     */
    bool removeNotification(uint notificationId);

    /*!
     * Add a new group.
     *
     * \param parameters a hash of key-value pairs containing information of group.
     * \return id of created group. If group creation succeeded id is bigger than 0.
     */
    uint addGroup(const QVariantHash  &parameters);

    /*!
     * Updates a group.
     *
     * groupIdKey for the group to be updated must be defined.
     * Replaces all defined keys. Keys that are not defined won't be replaced.
     *
     * \param parameters a hash of key-value pairs containing information of group.
     * \return true is returned if succeeded, false otherwise
     */
    bool updateGroup(uint groupId, const QVariantHash  &parameters);

    /*!
     * Removes a group and all notifications in group.
     *
     * groupId of the removable group must be defined
     *
     * \param parameters a hash of key-value pairs containing information of group.
     * \return true is returned if succeeded, false otherwise
     */
    bool removeGroup(uint groupId);

    //! Predefined notification parameter keys

    //! eventType the event type of the notification
    static const QString eventTypeKey;
    //! summary is text to be used in the notification
    static const QString summaryKey;
    //! body is text to be used in the notification
    static const QString bodyKey;
    //! action is the ID of the content to be used in the notification
    static const QString actionKey;
    //! imageURI is the ID of the icon to be used in the notification
    static const QString imageKey;
    //! count is the number of items inside this group
    static const QString countKey;
    //! identifier is the identifier string of the notification
    static const QString identifierKey;
    //! timestamp is the userspecified timestamp of notification
    static const QString timestampKey;

};
#if QT_VERSION < QT_VERSION_CHECK(4,8,0)
    QDBusArgument &operator<<(QDBusArgument &, const QVariantHash &);
#endif //QT_VERSION < QT_VERSION_CHECK(4,8,0)
    const QDBusArgument &operator>>(const QDBusArgument &, QVariantHash &);
//! \internal_end

#endif /* MNOTIFICATIONMANAGER_H_ */
