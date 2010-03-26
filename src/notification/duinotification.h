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

#include <DuiExport>
#include <QtGlobal>
#include <duiremoteaction.h>

class DuiNotificationPrivate;
class DuiNotificationGroup;
class QString;

/*!
    \class DuiNotification
    \brief A class that represents a notification.

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

        A notification is not created or updated until the publish() function is called.

        A list of notifications already created can be requested, but in this case a DuiApplication
        instance must have been created before any notifications were created and before the request
        itself. It should also be noted that currently all properties of such notifications need to be
        set before publishing changes to the notification. Otherwise the properties of the notification
        will be lost.

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
        \li DuiInfoBanner class can be used to show information banners in a single application. However,
            such banners are only shown inside the application using them, no sounds or other feedbacks are
            played and the banners are not aware of the current policy or profile in any way.

    \section DuiNotificationPersistence Persistence
        Notifications created as persistent are stored by the notification system and are returned by the
        DuiNotification::notifications() even after a reboot. It should be noted that currently all properties
        of such notifications need to be set before publishing changes to the notification. Otherwise the
        properties of the notification will be lost.

        \note A DuiApplication instance must be created before creating any persistent notifications.
*/

class DUI_EXPORT DuiNotification : public QObject
{
    Q_OBJECT

    /*!
        \property DuiNotification::summary
        \brief The summary text to be used in the notification.
    */
    Q_PROPERTY(QString summary READ summary WRITE setSummary)

    /*!
        \property DuiNotification::body
        \brief The body text to be used in the notification.
    */
    Q_PROPERTY(QString body READ body WRITE setBody)

    /*!
        \property DuiNotification::image
        \brief The name of the image to be used in the notification.
     */
    Q_PROPERTY(QString image READ image WRITE setImage)

    /*!
        \property DuiNotification::count
        \brief The number of items represented by this notification.
     */
    Q_PROPERTY(uint count READ count WRITE setCount)

public:
    /*!
     * Predefined event types. These are just the known types; applications
     * may use other types as well if the event type string is known.
     */
    //! Event type for SMS & MMS
    static const QString EVENT_MESSAGE;
    //! Event type for instant messages
    static const QString EVENT_CHAT;
    //! Event type for new email alert
    static const QString EVENT_EMAIL;

    /*!
     * Creates a new representation of a notification. The notification will
     * not be published until publish() is called. Only the event type needs
     * to be defined. If no summary or body text is defined the notification
     * will not have a visual representation.
     *
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification. Can be omitted (defaults to no summary text).
     * \param body the body text to be used in the notification. Can be omitted (defaults to no body text).
     */
    explicit DuiNotification(const QString &eventType, const QString &summary = QString(), const QString &body = QString());

    /*!
     * Destroys the class that represents a notification.
     */
    virtual ~DuiNotification();

    /*!
     * Sets the notification group the notification belongs to.
     *
     * \param group the notification group the notification belongs to.
     */
    void setGroup(const DuiNotificationGroup &group);

    /*!
     * Sets the summary text to be used in the notification.
     *
     * \param summary the summary text to be used in the notification.
     */
    void setSummary(const QString &summary);

    /*!
     * Gets the summary text to be used in the notification.
     *
     * \return the summary text to be used in the notification.
     */
    QString summary() const;

    /*!
     * Sets the body text to be used in the notification.
     *
     * \param body the body text to be used in the notification.
     */
    void setBody(const QString &body);

    /*!
     * Gets the body text to be used in the notification.
     *
     * \return the body text to be used in the notification.
     */
    QString body() const;

    /*!
     * Sets the name of the image to be used in the notification. If the name
     * is an absolute path (begins with a /) an image located in that path is
     * used. Otherwise the image name is interpreted to be an icon ID.
     *
     * \param image the name of the image to be used in the notification.
     */
    void setImage(const QString &image);

    /*!
     * Gets the name of the image to be used in the notification.
     *
     * \return the name of the image to be used in the notification.
     */
    QString image() const;

    /*!
     * Sets the action to be executed when the notification is activated.
     *
     * \param action the action to be executed when the notification is activated.
     */
    void setAction(const DuiRemoteAction &action);

    /*!
     * Sets the number of items represented by this notification (cardinality).
     * For example a single notification may represent 5 new e-mails, in which
     * case the count of the notification should be 5.
     *
     * \param count the number of items represented by this notification
     */
    void setCount(uint count);

    /*!
     * Gets the number of items represented by this notification (cardinality).
     *
     * \return the number of items represented by this notification
     */
    uint count() const;

    /*!
     * Publishes the notification. If the notification has not yet been
     * published a notification is created into the given notification
     * group (if any) and is given an ID by the notification manager.
     * Otherwise the existing notification is updated.
     *
     * \return true if the publishing succeeded, false otherwise
     */
    virtual bool publish();

    /*!
     * Removes a notification.
     *
     * \return true if the removal succeeded, false otherwise
     */
    virtual bool remove();

    /*!
     * Returns whether the notification is published
     *
     * \return true if the notification is published, false otherwise
     */
    bool isPublished() const;

    /*!
     * Returns a list of notifications created by this application but which
     * have not been dismissed by the user yet. Caller of this function gets
     * the ownership of the notifications, and is responsible for freeing them.
     *
     * \return list of notifications
     */
    static QList<DuiNotification *> notifications();

protected:
    //! \internal
    //! A pointer to the private implementation class
    DuiNotificationPrivate *d_ptr;

    /*!
     * A constructor to be called by the derived classes.
     * \param dd a derived private class object.
     */
    DuiNotification(DuiNotificationPrivate &dd);

    /*!
     * Creates a representation of an existing notification.
     * Should be used to get a handle to an existing notification
     * with a known ID.
     *
     * \param id the ID of the existing notification
     */
    explicit DuiNotification(uint id);

    //! Returns the ID of the notification.
    uint id() const;

    /*!
     * Sets the event type of the notification.
     *
     * \param eventType the event type of the notification
     */
    void setEventType(const QString &eventType);
    //! \internal_end

    Q_DECLARE_PRIVATE(DuiNotification)
    Q_DISABLE_COPY(DuiNotification)
};

#endif /* DUINOTIFICATION_H_ */
