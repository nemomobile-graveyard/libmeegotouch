/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

//============================================================================
// Description : Tool to handle notifications and notification groups
//============================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <getopt.h>
#include <MApplication>
#include <MNotification>
#include <MRemoteAction>
#include <MNotificationGroup>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QApplication>
#include <mnotificationmanager.h>

// Subclasses to gain access to the IDs
class MNotificationToolNotification : public MNotification
{
public:
    explicit MNotificationToolNotification(const QString &eventType, const QString &summary, const QString &body);
    explicit MNotificationToolNotification(uint id);
    uint id() const;
    void setEventType(const QString &eventType);
};

class MNotificationToolNotificationGroup : public MNotificationGroup
{
public:
    explicit MNotificationToolNotificationGroup(const QString &eventType, const QString &summary, const QString &body);
    explicit MNotificationToolNotificationGroup(uint id);
    uint id() const;
    void setEventType(const QString &eventType);
};

MNotificationToolNotification::MNotificationToolNotification(const QString &eventType, const QString &summary, const QString &body) : MNotification(eventType, summary, body)
{
}

MNotificationToolNotification::MNotificationToolNotification(uint id) : MNotification(id)
{
}

uint MNotificationToolNotification::id() const
{
    return MNotification::id();
}

void MNotificationToolNotification::setEventType(const QString &eventType)
{
    MNotification::setEventType(eventType);
}

MNotificationToolNotificationGroup::MNotificationToolNotificationGroup(const QString &eventType, const QString &summary, const QString &body) : MNotificationGroup(eventType, summary, body)
{
}

MNotificationToolNotificationGroup::MNotificationToolNotificationGroup(uint id) : MNotificationGroup(id)
{
}

uint MNotificationToolNotificationGroup::id() const
{
    return MNotificationGroup::id();
}

void MNotificationToolNotificationGroup::setEventType(const QString &eventType)
{
    MNotificationGroup::setEventType(eventType);
}

// The actions for this tool
enum ToolAction {
    Undefined,
    Add,
    Update,
    Remove
};

// The action to perform
ToolAction toolAction = Undefined;

// Whether to operate on notification groups instead of notifications
bool groupMode = false;

// Notifications list mode in use
bool listMode = false;

// Application name provided with list mode
QString applicationName("mnotificationtool");

// The notification/notification group ID to use
uint id = 0;

// The notification/notification group ID to use
uint count = 1;

// Prints usage information
int usage(const char *program)
{
    std::cerr << std::setw(7) << "Usage: " << program << " [OPTION]... EVENTTYPE [SUMMARY BODY IMAGE SERVICENAME OBJECTPATH INTERFACE METHODNAME ARG...]" << std::endl;
    std::cerr << std::setw(7) << "Manage notifications." << std::endl;
    std::cerr << std::setw(7) << std::endl;
    std::cerr << std::setw(7) << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
    std::cerr << std::setw(7) << "  -a, --action=ACTION        The action (add/update/remove) to perform." << std::endl;
    std::cerr << std::setw(7) << "                             add - Adds a new notification or notification group." << std::endl;
    std::cerr << std::setw(7) << "                             update - Updates an existing notification or notification group." << std::endl;
    std::cerr << std::setw(7) << "                             remove - Removes an existing notification or notification group." << std::endl;
    std::cerr << std::setw(7) << "                                      A specific notification or group can be specified with the -i argument." << std::endl;
    std::cerr << std::setw(7) << "                                      If no id is given, all the notifications will be removed." << std::endl;
    std::cerr << std::setw(7) << "  -g, --group                Whether to operate on notification groups instead of notifications." << std::endl;
    std::cerr << std::setw(7) << "  -i, --id=ID                The notification/notification group ID to use." << std::endl;
    std::cerr << std::setw(7) << "  -c, --count=NUMBER         The number of notifications. This parameter has no effect when the action is 'remove'" << std::endl;
    std::cerr << std::setw(7) << "  -l, --list=APPLICATION     List all notifications that belong to an application name. Returns a count of notifications as an exit value." << std::endl;
    std::cerr << std::setw(7) << "      --help     display this help and exit" << std::endl;
    return -1;
}

// Parses command line arguments (the flags)
int parseArguments(int argc, char *argv[])
{
    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            { "action", 1, NULL, 'a' },
            { "group", 0, NULL, 'g' },
            { "id", 1, NULL, 'i' },
            { "count", 1, NULL, 'c' },
            { "help", 0, NULL, 'h' },
            { "list", 1, NULL, 'l'},
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "a:gi:c:pl:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'a':
            if (strcmp(optarg, "add") == 0) {
                toolAction = Add;
            } else if (strcmp(optarg, "update") == 0) {
                toolAction = Update;
            } else if (strcmp(optarg, "remove") == 0) {
                toolAction = Remove;
            }
            break;
        case 'i':
            id = atoi(optarg);
            break;
        case 'c':
            count = atoi(optarg);
            break;
        case 'g':
            groupMode = true;
            break;
        case 'l':
            listMode = true;
            applicationName = QString(optarg);
            break;
        case 'h':
            return usage(argv[0]);
        default:
            break;
        }
    }

    if (!listMode) {
        if (toolAction == Undefined ||
                (toolAction == Add && argc < optind + 1) ||
                (toolAction == Update && argc < optind + 4) ||
                (toolAction == Update && id == 0)) {
            return usage(argv[0]);
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Parse arguments
    int result = parseArguments(argc, argv);
    if (result != 0) {
        return result;
    }

    // MApplication creates MNotificationManager for specified application
    QScopedPointer<MApplication> mApplication(new MApplication(argc, argv, applicationName, 0));
    if (mApplication.isNull()) {
        std::cerr << "Couldn't initialize MApplication" << std::endl;
        return -1;
    }

    // Calls notificationIdList from NotificationManager. Returns size of the list from main.
    if (listMode) {
        QList<MNotification *> list = MNotification::notifications();
        result = list.size();
        std::cout << "\n" << applicationName.toUtf8().data() << " has " << list.size() << " notifications." << std::endl;
        std::cout << "Notification id(s):" << std::endl;
        foreach(MNotification *notification, list) {
            std::cout << static_cast<MNotificationToolNotification *>(notification)->id() << std::endl;
            delete notification;
        }
        list.clear();
    }

    // Execute the desired action
    switch (toolAction) {
    case Add:
    case Update: {
        // Get the parameters for adding and updating notifications/notification groups
        QString eventType = QString(argv[optind]);
        QString summary, body, image;
        if (argc >= optind + 1) {
            summary = QString(argv[optind + 1]);
        }
        if (argc >= optind + 2) {
            body = QString(argv[optind + 2]);
        }
        if (argc >= optind + 3) {
            image = QString(argv[optind + 3]);
        }

        // Create an action if one is defined
        MRemoteAction *remoteAction;
        if (argc >= optind + 4 + 4) {
            QString serviceName(argv[optind + 4]);
            QString objectPath(argv[optind + 5]);
            QString interface(argv[optind + 6]);
            QString methodName(argv[optind + 7]);
            QList<QVariant> arguments;

            for (int i = optind + 8; i < argc; ++i) {
                arguments.append(QVariant(argv[i]));
            }
            remoteAction = new MRemoteAction(serviceName, objectPath, interface, methodName, arguments);
        } else {
            remoteAction = new MRemoteAction;
        }

        if (toolAction == Add) {
            // Add a notification/notification group
            if (groupMode) {
                MNotificationToolNotificationGroup group(eventType, summary, body);
                group.setImage(image);
                group.setAction(*remoteAction);
                group.setCount(count);
                group.publish();
                result = group.id();
            } else {
                if (id != 0) {
                    MNotificationToolNotification notification(eventType, summary, body);
                    notification.setGroup(MNotificationToolNotificationGroup(id));
                    notification.setImage(image);
                    notification.setAction(*remoteAction);
                    notification.setCount(count);
                    notification.publish();
                    result = notification.id();
                } else {
                    MNotificationToolNotification notification(eventType, summary, body);
                    notification.setImage(image);
                    notification.setAction(*remoteAction);
                    notification.setCount(count);
                    notification.publish();
                    result = notification.id();
                }
            }
        } else {
            // Update a notification/notification group
            if (groupMode) {
                MNotificationToolNotificationGroup group(id);
                group.setEventType(eventType);
                group.setSummary(summary);
                group.setBody(body);
                group.setImage(image);
                group.setAction(*remoteAction);
                group.setCount(count);
                group.publish();
            } else {
                MNotificationToolNotification notification(id);
                notification.setEventType(eventType);
                notification.setSummary(summary);
                notification.setBody(body);
                notification.setImage(image);
                notification.setAction(*remoteAction);
                notification.setCount(count);
                notification.publish();
            }
        }

        delete remoteAction;
        break;
    }
    case Remove:
        if (id > 0) {
            if (groupMode) {
                MNotificationToolNotificationGroup group(id);
                group.remove();
            } else {
                MNotificationToolNotification notification(id);
                notification.remove();
            }
        } else {
            // Remove all the notifications for this user
            QList<MNotification *> list = MNotification::notifications();
            foreach(MNotification *notification, list) {
                notification->remove();
                delete notification;
            }
            list.clear();

            // There's no way to query the groups of the user, so there's no way to remove all the groups either :(
        }
        break;
    default:
        break;
    }

    return result;
}
