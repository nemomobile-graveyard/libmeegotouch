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

#ifndef MNOTIFICATION_H_
#define MNOTIFICATION_H_

#include <MExport>
#include <mremoteaction.h>

class MNotificationPrivate;
class MNotificationGroup;
class QString;
class QDBusArgument;

/*!
    \class MNotification
    \brief A class that represents a notification.

    \ingroup widgets

    \section MNotificationOverview Overview
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

        A list of notifications already created can be requested. A QCoreApplication must be created before
        doing the request.

        \sa \ref notifications

    \section MNotificationInteractions Interactions
        Notifications can be non-interactive (for information only) or interactive: the interactive
        notifications provide information, but they also provide a way for interacting with the notification,
        for example to fix a connectivity problem. Notifications are always modeless components - modal
        "notifications" are specified with Dialogs. Images on this page are for illustrational purposes only.
        In case that images and text conflict each other, the text should be followed.

    \section MNotificationUsageGuidelines Usage guidelines
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
        \li MInfoBanner class can be used to show information banners in a single application. However,
            such banners are only shown inside the application using them, no sounds or other feedbacks are
            played and the banners are not aware of the current policy or profile in any way.

    \section MNotificationPersistence Persistence
        Notifications created as persistent are stored by the notification system and are returned by the
        MNotification::notifications() even after a reboot. It should be noted that currently all properties
        of such notifications need to be set before publishing changes to the notification. Otherwise the
        properties of the notification will be lost.

        \note A QCoreApplication instance must be created before creating any persistent notifications.
*/

class M_CORE_EXPORT MNotification : public QObject
{
    Q_OBJECT

    /*!
        \property MNotification::summary
        \brief The summary text to be used in the notification.
    */
    Q_PROPERTY(QString summary READ summary WRITE setSummary)

    /*!
        \property MNotification::body
        \brief The body text to be used in the notification.
    */
    Q_PROPERTY(QString body READ body WRITE setBody)

    /*!
        \property MNotification::image
        \brief The name of the image to be used in the notification.
     */
    Q_PROPERTY(QString image READ image WRITE setImage)

    /*!
        \property MNotification::count
        \brief The number of items represented by this notification.
     */
    Q_PROPERTY(uint count READ count WRITE setCount)

    /*!
      \property MNotification::identifier
      \brief The identifier string of the notification. The identifier is empty by default.
    */
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier)

public:
    /*!
     * Predefined event types. These are just the known types; applications
     * may use other types as well if the event type string is known.
     */
    //! A generic device-related notification that doesn't fit into any other category.
    static const QString DeviceEvent;
    //! A device, such as a USB device, was added to the system.
    static const QString DeviceAddedEvent;
    //! A device had some kind of error.
    static const QString DeviceErrorEvent;
    //! A device, such as a USB device, was removed from the system.
    static const QString DeviceRemovedEvent;
    //! A generic e-mail-related notification that doesn't fit into any other category.
    static const QString EmailEvent;
    //! A new e-mail notification.
    static const QString EmailArrivedEvent;
    //! A notification stating that an e-mail has bounced.
    static const QString EmailBouncedEvent;
    //! A generic instant message-related notification that doesn't fit into any other category.
    static const QString ImEvent;
    //! An instant message error notification.
    static const QString ImErrorEvent;
    //! A received instant message notification.
    static const QString ImReceivedEvent;
    //! A generic network notification that doesn't fit into any other category.
    static const QString NetworkEvent;
    //! A network connection notification, such as successful sign-on to a network service. This should not be confused with device.added for new network devices.
    static const QString NetworkConnectedEvent;
    //! A network disconnected notification. This should not be confused with device.removed for disconnected network devices.
    static const QString NetworkDisconnectedEvent;
    //! A network-related or connection-related error.
    static const QString NetworkErrorEvent;
    //! A generic presence change notification that doesn't fit into any other category, such as going away or idle.
    static const QString PresenceEvent;
    //! An offline presence change notification.
    static const QString PresenceOfflineEvent;
    //! An online presence change notification.
    static const QString PresenceOnlineEvent;
    //! A generic file transfer or download notification that doesn't fit into any other category.
    static const QString TransferEvent;
    //! A file transfer or download complete notification.
    static const QString TransferCompleteEvent;
    //! A file transfer or download error.
    static const QString TransferErrorEvent;

    /*!
     * Creates a new representation of a notification. The notification will
     * not be published until publish() is called. Only the event type needs
     * to be defined. If no summary or body text is defined the notification
     * will not have a visual representation.
     *
     * \param eventType the event type of the notification. Types are in the format defined in Desktop Notifications Specification. Known constants (such as EmailArrivedEvent) are provided by the MNotification class but custom strings can also be used.
     * \param summary the summary text to be used in the notification. Can be omitted (defaults to no summary text).
     * \param body the body text to be used in the notification. Can be omitted (defaults to no body text).
     */
    explicit MNotification(const QString &eventType, const QString &summary = QString(), const QString &body = QString());

    /*!
     * Destroys the class that represents a notification.
     */
    virtual ~MNotification();

    /*!
     * Sets the notification group the notification belongs to.
     * The notification group must be published before this function is called.
     *
     * \param group the notification group the notification belongs to.
     */
    void setGroup(const MNotificationGroup &group);

    /*!
     * Gets the event type of the notification.
     *
     * \return the event type of the notification.
     */
    QString eventType() const;

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
    void setAction(const MRemoteAction &action);

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
     * Sets an identifier for the notification. Can be any string the application
     * needs to identify this particular notification.
     *
     * \param id the identifier
     */
    void setIdentifier(const QString &identifier);

    /*!
     * Gets the identifier of the notification.
     *
     * \return the identifier.
     */
    QString identifier() const;

    /*!
     * Sets a user specified timestamp for notification.
     *
     * If user has not set the timestamp with setTimestamp() since the previous
     * publish(), system current time is used as the notification's timestamp
     * the next time the notification is published with publish().
     *
     * If timestamp provided is invalid, notification's timestamp is null.
     *
     * Use timestamp() to get the actual published timestamp.
     *
     * NOTE! Setting timestamp for MNotificationGroup does nothing.
     *
     *\param timestamp Timestamp for notification
     *
     *\sa timestamp()
     */
    void setTimestamp(const QDateTime &timestamp);

    /*!
     * Returns the published timestamp of the notification. If notification
     * has not yet been published null QDateTime is returned.
     *
     *\return timestamp of notification. If timestamp is not published null QDateTime is returned.
     *
     *\sa setTimestamp()
     */
    const QDateTime timestamp() const;

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
    static QList<MNotification *> notifications();

    //! \internal
    /*!
     * Creates a new uninitialized representation of a notification. This
     * constructor should only be used for populating the notification list
     * from D-Bus structures.
     */
    MNotification();

    /*!
     * Creates a copy of an existing representation of a notification.
     * This constructor should only be used for populating the notification
     * list from D-Bus structures.
     *
     * \param notification the notification representation to a create copy of
     */
    explicit MNotification(const MNotification &notification);

    friend QDBusArgument &operator<<(QDBusArgument &, const MNotification &);
    friend const QDBusArgument &operator>>(const QDBusArgument &, MNotification &);
    MNotification &operator=(const MNotification &);

protected:
    //! A pointer to the private implementation class
    MNotificationPrivate *d_ptr;

    /*!
     * A constructor to be called by the derived classes.
     * \param dd a derived private class object.
     */
    MNotification(MNotificationPrivate &dd);

    /*!
     * Creates a representation of an existing notification.
     * Should be used to get a handle to an existing notification
     * with a known ID.
     *
     * \param id the ID of the existing notification
     */
    explicit MNotification(uint id);

    //! Returns the ID of the notification.
    uint id() const;

    //! Returns the group ID of the notification.
    uint groupId() const;

    /*!
     * Sets the event type of the notification.
     *
     * \param eventType the event type of the notification
     */
    void setEventType(const QString &eventType);
    //! \internal_end

    friend class MNotificationGroup;

    Q_DECLARE_PRIVATE(MNotification)

#ifdef UNIT_TEST
    friend class Ut_MNotification;
#endif
};

Q_DECLARE_METATYPE(MNotification)

#endif /* MNOTIFICATION_H_ */
