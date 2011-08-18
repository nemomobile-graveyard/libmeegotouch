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

#ifndef MNOTIFICATION_P_H
#define MNOTIFICATION_P_H

/*!
 * A private class for MNotification
 */
class MNotificationPrivate
{
public:
    /*!
     * Constructor
     */
    MNotificationPrivate();

    /*!
     * Destructor
     */
    virtual ~MNotificationPrivate();

    /*!
     * Creates a hash containing notification's information (notificationId,
     * eventType, action etc.) in key-value pairs.
     *
     * \return QVariantHash (same as QHash(QString, QVariant)) containing notification's information.
     */
    QVariantHash notificationParameters() const;

    /*!
     * Extracts notification's information from a hash and populates
     * notification's members.
     *
     * \param QVariantHash containing notification's information.
     */
    void extractNotificationParameters(const QVariantHash &parameters);

    //! The ID of the notification
    uint id;

    //! Notification group of the notification.
    uint groupId;

    //! The event type of the notification.
    QString eventType;

    //! The summary text to be used in the notification. Defaults to no summary text.
    QString summary;

    //! The body text to be used in the notification. Defaults to no body text.
    QString body;

    //! The name of the image to be used in the notification. Defaults to no image.
    QString image;

    //! The action to be executed when the notification is activated. Defaults to no action.
    QString action;

    //! The number of items inside this notification
    uint count;

    //! The identifier of the notification set by the application
    QString identifier;

    //! User set timestamp of notification
    uint userSetTimestamp;

    //!  Timestamp that has been previously published
    uint publishedTimestamp;
};

#endif // MNOTIFICATION_P_H
