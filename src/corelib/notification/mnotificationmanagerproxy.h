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

#ifndef MNOTIFICATIONMANAGERPROXY_H_1318419651
#define MNOTIFICATIONMANAGERPROXY_H_1318419651

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include "metatypedeclarations.h"

/*
 * Proxy class for interface com.meego.core.MNotificationManager
 */
class MNotificationManagerProxy: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "com.meego.core.MNotificationManager"; }

public:
    MNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~MNotificationManagerProxy();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<uint> addGroup(uint notificationUserId, const QHash < QString, QVariant >  &parameters)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(parameters);
        return asyncCallWithArgumentList(QLatin1String("addGroup"), argumentList);
    }

    inline QDBusPendingReply<uint> addNotification(uint notificationUserId, uint groupId, const QVariantHash &parameters)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(groupId) << qVariantFromValue(parameters);
        return asyncCallWithArgumentList(QLatin1String("addNotification"), argumentList);
    }

    inline QDBusPendingReply<uint> notificationCountInGroup(uint notificationUserId, uint groupId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(groupId);
        return asyncCallWithArgumentList(QLatin1String("notificationCountInGroup"), argumentList);
    }

    inline QDBusPendingReply<QList < MNotificationGroup > > notificationGroupListWithNotificationParameters(uint notificationUserId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId);
        return asyncCallWithArgumentList(QLatin1String("notificationGroupListWithNotificationParameters"), argumentList);
    }

    inline QDBusPendingReply<QList < uint > > notificationIdList(uint notificationUserId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId);
        return asyncCallWithArgumentList(QLatin1String("notificationIdList"), argumentList);
    }

    inline QDBusPendingReply<QList < MNotification > > notificationListWithNotificationParameters(uint notificationUserId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId);
        return asyncCallWithArgumentList(QLatin1String("notificationListWithNotificationParameters"), argumentList);
    }

    inline QDBusPendingReply<uint> notificationUserId()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("notificationUserId"), argumentList);
    }

    inline QDBusPendingReply<bool> removeGroup(uint notificationUserId, uint groupId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(groupId);
        return asyncCallWithArgumentList(QLatin1String("removeGroup"), argumentList);
    }

    inline QDBusPendingReply<bool> removeNotification(uint notificationUserId, uint notificationId)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(notificationId);
        return asyncCallWithArgumentList(QLatin1String("removeNotification"), argumentList);
    }

    inline QDBusPendingReply<bool> updateGroup(uint notificationUserId, uint groupId, const QHash < QString, QVariant >  &parameters)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(groupId) << qVariantFromValue(parameters);
        return asyncCallWithArgumentList(QLatin1String("updateGroup"), argumentList);
    }

    inline QDBusPendingReply<bool> updateNotification(uint notificationUserId, uint notificationId, const QHash < QString, QVariant >  &parameters)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(notificationUserId) << qVariantFromValue(notificationId) << qVariantFromValue(parameters);
        return asyncCallWithArgumentList(QLatin1String("updateNotification"), argumentList);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
  namespace meego {
    namespace core {
      typedef ::MNotificationManagerProxy MNotificationManager;
    }
  }
}
#endif
