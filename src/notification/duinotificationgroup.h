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
#include <QtGlobal>
#include <QString>
#include <duiremoteaction.h>

class DuiNotificationGroupPrivate;

/*!
 * A class that represents a notification group. When an instance of this
 * class is created a notification group is created unless the instance
 * represents an already existing notification group and is created using a
 * known notification group ID.
 *
 * \sa \ref notifications
 */
class DUI_EXPORT DuiNotificationGroup
{
public:
    /*!
     * Creates a new representation of a notification group. A notification
     * group is automatically created and is given an ID by the notification
     * manager.
     *
     * \param eventType the event type of the notification group
     * \param summary the summary text to be used in the notification group. Defaults to no summary text.
     * \param body the body text to be used in the notification group. Defaults to no body text.
     * \param imageURI the ID of the icon to be used in the notification group. Defaults to no image URI.
     * \param action the action to be executed when the notification group is activated. Defaults to no action.
     * \param count the number of notifications in this group
     * \param persistent \c true if the notifications in this group should be persistent, \c false otherwise
     */
    explicit DuiNotificationGroup(const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &imageURI = QString(), const DuiRemoteAction &action = DuiRemoteAction(), uint count = 1, bool persistent = false);

    /*!
     * Creates a representation of an existing notification group.
     * Should be used to get a handle to an existing notification group
     * with a known ID.
     *
     * \param id the ID of the existing notification group
     */
    explicit DuiNotificationGroup(unsigned int id);

    /*!
     * Destroys the class that represents a notification group.
     */
    virtual ~DuiNotificationGroup();

    /*!
     * Returns the ID of the notification group.
     *
     * \return the ID of the notification group
     */
    unsigned int id() const;

    /*!
     * Updates the notification group.
     *
     * \param eventType the event type of the notification group
     * \param summary the summary text to be used in the notification group. Defaults to no summary text.
     * \param body the body text to be used in the notification group. Defaults to no body text.
     * \param imageURI the ID of the icon to be used in the notification group. Defaults to no image URI.
     * \param count the number of notifications in this group
     * \param action the action to be executed when the notification group is activated. Defaults to no action.
     * \return true if the update succeeded, false otherwise
     */
    bool update(const QString &eventType, const QString &summary = QString(), const QString &body = QString(), const QString &imageURI = QString(), uint count = 1, const DuiRemoteAction &action = DuiRemoteAction());

    /*!
     * Removes the notification group and all notifications in the group.
     *
     * \return true if the removal succeeded, false otherwise
     */
    bool remove();

    /*!
     * Returns whether the notification group is valid
     *
     * \return true if the notification group is valid, false otherwise
     */
    bool isValid() const;

protected:
    /*!
     * A constructor to be called by the derived classes.
     * \param dd a derived private class object.
     */
    DuiNotificationGroup(DuiNotificationGroupPrivate &dd);

    //! A pointer to the private implementation class
    DuiNotificationGroupPrivate *d_ptr;

    Q_DECLARE_PRIVATE(DuiNotificationGroup)

};

#endif /* DUINOTIFICATIONGROUP_H_ */
