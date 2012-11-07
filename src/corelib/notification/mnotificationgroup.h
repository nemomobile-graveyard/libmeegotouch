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

#ifndef MNOTIFICATIONGROUP_H_
#define MNOTIFICATIONGROUP_H_

#include <MExport>
#include <QString>
#include "mnotification.h"

class MNotificationGroupPrivate;
class MNotificationManagerProxy;

extern MNotificationManagerProxy *notificationManager();

/*!
    \class MNotificationGroup
    \brief A class that represents a notification group.

    \ingroup widgets

    \section MNotificationOverview Overview
        A class that represents a notification group. Notification groups can be
        used to bundle similar notifications into one so that the individual
        notifications join a group of notifications on the notification area.

        A notification group is not created or updated until the publish() function
        is called. A notification group must be published before notifications are
        placed into it.

        A list of notification groups already created can be requested.
        A QCoreApplication must be created before doing the request.

        \sa \ref MNotification
        \sa \ref notifications
 */
class M_CORE_EXPORT MNotificationGroup : public MNotification
{
public:
    /*!
     * Creates a new representation of a notification group. The notification
     * group will not be published until publish() is called. Only the event
     * type needs to be defined. If no summary or body text is defined the
     * notification group will not have a visual representation.
     *
     * \param eventType the event type of the notification group
     * \param summary the summary text to be used in the notification. Can be omitted (defaults to no summary text).
     * \param body the body text to be used in the notification. Can be omitted (defaults to no body text).
     */
    explicit MNotificationGroup(const QString &eventType, const QString &summary = QString(), const QString &body = QString());

    /*!
     * Destroys the class that represents a notification group.
     */
    virtual ~MNotificationGroup();

    /*!
     * Returns a list of notification groups created by this application but
     * which have not been dismissed by the user yet. Caller of this function
     * gets the ownership of the notification groups, and is responsible for
     * freeing them.
     *
     * \return list of notifications
     */
    static QList<MNotificationGroup *> notificationGroups();

    //! \internal
    /*!
     * Creates a new uninitialized representation of a notification group.
     * This constructor should only be used for populating the notification group
     * list from D-Bus structures.
     */
    MNotificationGroup();

    /*!
     * Creates a copy of an existing representation of a notification group.
     * This constructor should only be used for populating the notification
     * group list from D-Bus structures.
     *
     * \param group the notification representation to a create copy of
     */
    explicit MNotificationGroup(const MNotificationGroup &group);
    //! \internal_end

    /*!
     * Returns amount of notifications in a given group
     *
     * \param groupId the group ID
     * \return amount of notifications in given group
     */
    uint notificationCount();

    //! \internal
    /*!
     * Timestamp cannot be set for MotificationGroup.
     * This is to disable setTimestamp inherited from
     * MNotification.
     */
    void setTimestamp(const QDateTime &timestamp);
    //! \internal_end

protected:
    /*!
     * Creates a representation of an existing notification group.
     * Should be used to get a handle to an existing notification group
     * with a known ID.
     *
     * \param id the ID of the existing notification group
     */
    explicit MNotificationGroup(uint id);

    Q_DECLARE_PRIVATE(MNotificationGroup)
};

Q_DECLARE_METATYPE(MNotificationGroup)

#endif /* MNOTIFICATIONGROUP_H_ */
