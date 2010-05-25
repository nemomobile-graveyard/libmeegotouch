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

#ifndef MNOTIFICATIONMANAGER_STUB
#define MNOTIFICATIONMANAGER_STUB

#include "mnotificationmanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNotificationManagerStub : public StubBase
{
public:
    virtual void MNotificationManagerConstructor();
    virtual void MNotificationManagerDestructor();
    virtual uint addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual uint addGroup(const QString &eventType);
    virtual uint addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual uint addNotification(uint groupId, const QString &eventType);
    virtual bool removeGroup(uint groupId);
    virtual bool removeNotification(uint notificationId);
    virtual bool updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual bool updateGroup(uint groupId, const QString &eventType);
    virtual bool updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual bool updateNotification(uint notificationId, const QString &eventType);
    virtual QList<uint> notificationIdList();
};

// 2. IMPLEMENT STUB
void MNotificationManagerStub::MNotificationManagerConstructor()
{

}
void MNotificationManagerStub::MNotificationManagerDestructor()
{

}

uint MNotificationManagerStub::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    stubMethodEntered("addGroup", params);
    return stubReturnValue<uint>("addGroup");
}

uint MNotificationManagerStub::addGroup(const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("addGroup", params);
    return stubReturnValue<uint>("addGroup");
}

uint MNotificationManagerStub::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    stubMethodEntered("addNotification", params);
    return stubReturnValue<uint>("addNotification");
}

uint MNotificationManagerStub::addNotification(uint groupId, const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("addNotification", params);
    return stubReturnValue<uint>("addNotification");
}

bool MNotificationManagerStub::removeGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
    return stubReturnValue<bool>("removeGroup");
}

bool MNotificationManagerStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
    return stubReturnValue<bool>("removeNotification");
}

bool MNotificationManagerStub::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    stubMethodEntered("updateGroup", params);
    return stubReturnValue<bool>("updateGroup");
}

bool MNotificationManagerStub::updateGroup(uint groupId, const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("updateGroup", params);
    return stubReturnValue<bool>("updateGroup");
}

bool MNotificationManagerStub::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    stubMethodEntered("updateNotification", params);
    return stubReturnValue<bool>("updateNotification");
}

bool MNotificationManagerStub::updateNotification(uint notificationId, const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("updateNotification", params);
    return stubReturnValue<bool>("updateNotification");
}

QList<uint> MNotificationManagerStub::notificationIdList()
{
    stubMethodEntered("notificationIdList");
    return stubReturnValue<QList<uint> >("notificationIdList");
}



// 3. CREATE A STUB INSTANCE
MNotificationManagerStub gDefaultMNotificationManagerStub;
MNotificationManagerStub *gMNotificationManagerStub = &gDefaultMNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNotificationManager::MNotificationManager() : proxy(QString(), QString(), QDBusConnection::sessionBus()),
  userId( 0 )
{
    gMNotificationManagerStub->MNotificationManagerConstructor();
}

MNotificationManager::~MNotificationManager()
{
    gMNotificationManagerStub->MNotificationManagerDestructor();
}

MNotificationManager *MNotificationManager::instance()
{
    static MNotificationManager manager;
    return &manager;
}

uint MNotificationManager::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gMNotificationManagerStub->addGroup(eventType, summary, body, action, imageURI, count);
}

uint MNotificationManager::addGroup(const QString &eventType)
{
    return gMNotificationManagerStub->addGroup(eventType);
}

uint MNotificationManager::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gMNotificationManagerStub->addNotification(groupId, eventType, summary, body, action, imageURI, count);
}

uint MNotificationManager::addNotification(uint groupId, const QString &eventType)
{
    return gMNotificationManagerStub->addNotification(groupId, eventType);
}

bool MNotificationManager::removeGroup(uint groupId)
{
    return gMNotificationManagerStub->removeGroup(groupId);
}

bool MNotificationManager::removeNotification(uint notificationId)
{
    return gMNotificationManagerStub->removeNotification(notificationId);
}

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gMNotificationManagerStub->updateGroup(groupId, eventType, summary, body, action, imageURI, count);
}

bool MNotificationManager::updateGroup(uint groupId, const QString &eventType)
{
    return gMNotificationManagerStub->updateGroup(groupId, eventType);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gMNotificationManagerStub->updateNotification(notificationId, eventType, summary, body, action, imageURI, count);
}

bool MNotificationManager::updateNotification(uint notificationId, const QString &eventType)
{
    return gMNotificationManagerStub->updateNotification(notificationId, eventType);
}

QList<uint> MNotificationManager::notificationIdList()
{
    return gMNotificationManagerStub->notificationIdList();
}

MNotificationManagerProxy::MNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

MNotificationManagerProxy::~MNotificationManagerProxy()
{
}

#endif
