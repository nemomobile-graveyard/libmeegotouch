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

#ifndef DUINOTIFICATIONGROUP_H_
#define DUINOTIFICATIONGROUP_H_

#include <DuiExport>
#include <QString>
#include "duinotification.h"

class DuiNotificationGroupPrivate;

/*!
 * A class that represents a notification group. Notification groups can be
 * used to bundle similar notifications into one so that the individual
 * notifications join a group of notifications on the notification area.
 *
 * \sa \ref notifications
 */
class DUI_EXPORT DuiNotificationGroup : public DuiNotification
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
    explicit DuiNotificationGroup(const QString &eventType, const QString &summary = QString(), const QString &body = QString());

    /*!
     * Destroys the class that represents a notification group.
     */
    virtual ~DuiNotificationGroup();

    //! \reimp
    virtual bool publish();
    virtual bool remove();
    //! \reimp_end

protected:
    //! \internal
    /*!
     * Creates a representation of an existing notification group.
     * Should be used to get a handle to an existing notification group
     * with a known ID.
     *
     * \param id the ID of the existing notification group
     */
    explicit DuiNotificationGroup(uint id);

    Q_DECLARE_PRIVATE(DuiNotificationGroup)
    Q_DISABLE_COPY(DuiNotificationGroup)
};

#endif /* DUINOTIFICATIONGROUP_H_ */
