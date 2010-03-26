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

#ifndef DUINOTIFICATION_P_H
#define DUINOTIFICATION_P_H

/*!
 * A private class for DuiNotification
 */
class DuiNotificationPrivate
{
public:
    /*!
     * Constructor
     */
    DuiNotificationPrivate();

    /*!
     * Destructor
     */
    virtual ~DuiNotificationPrivate();

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
};

#endif // DUINOTIFICATION_P_H
