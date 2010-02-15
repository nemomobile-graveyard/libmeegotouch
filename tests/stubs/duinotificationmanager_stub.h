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

#ifndef DUINOTIFICATIONMANAGER_STUB
#define DUINOTIFICATIONMANAGER_STUB

#include "duinotificationmanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiNotificationManagerStub : public StubBase
{
public:
    virtual void DuiNotificationManagerConstructor();
    virtual void DuiNotificationManagerDestructor();
    virtual uint addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent);
    virtual uint addGroup(const QString &eventType, bool persistent);
    virtual uint addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent);
    virtual uint addNotification(uint groupId, const QString &eventType, bool persistent);
    virtual bool removeGroup(uint groupId);
    virtual bool removeNotification(uint notificationId);
    virtual bool updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual bool updateGroup(uint groupId, const QString &eventType);
    virtual bool updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);
    virtual bool updateNotification(uint notificationId, const QString &eventType);
    virtual QList<uint> notificationIdList();
};

// 2. IMPLEMENT STUB
void DuiNotificationManagerStub::DuiNotificationManagerConstructor()
{

}
void DuiNotificationManagerStub::DuiNotificationManagerDestructor()
{

}

uint DuiNotificationManagerStub::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    params.append(new Parameter<bool >(persistent));
    stubMethodEntered("addGroup", params);
    return stubReturnValue<uint>("addGroup");
}

uint DuiNotificationManagerStub::addGroup(const QString &eventType, bool persistent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<bool >(persistent));
    stubMethodEntered("addGroup", params);
    return stubReturnValue<uint>("addGroup");
}

uint DuiNotificationManagerStub::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<const QString & >(summary));
    params.append(new Parameter<const QString & >(body));
    params.append(new Parameter<const QString & >(action));
    params.append(new Parameter<const QString & >(imageURI));
    params.append(new Parameter<uint >(count));
    params.append(new Parameter<bool >(persistent));
    stubMethodEntered("addNotification", params);
    return stubReturnValue<uint>("addNotification");
}

uint DuiNotificationManagerStub::addNotification(uint groupId, const QString &eventType, bool persistent)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    params.append(new Parameter<bool >(persistent));
    stubMethodEntered("addNotification", params);
    return stubReturnValue<uint>("addNotification");
}

bool DuiNotificationManagerStub::removeGroup(uint groupId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup", params);
    return stubReturnValue<bool>("removeGroup");
}

bool DuiNotificationManagerStub::removeNotification(uint notificationId)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification", params);
    return stubReturnValue<bool>("removeNotification");
}

bool DuiNotificationManagerStub::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
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

bool DuiNotificationManagerStub::updateGroup(uint groupId, const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(groupId));
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("updateGroup", params);
    return stubReturnValue<bool>("updateGroup");
}

bool DuiNotificationManagerStub::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
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

bool DuiNotificationManagerStub::updateNotification(uint notificationId, const QString &eventType)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(notificationId));
    params.append(new Parameter<const QString & >(eventType));
    stubMethodEntered("updateNotification", params);
    return stubReturnValue<bool>("updateNotification");
}

QList<uint> DuiNotificationManagerStub::notificationIdList()
{
    stubMethodEntered("notificationIdList");
    return stubReturnValue<QList<uint> >("notificationIdList");
}



// 3. CREATE A STUB INSTANCE
DuiNotificationManagerStub gDefaultDuiNotificationManagerStub;
DuiNotificationManagerStub *gDuiNotificationManagerStub = &gDefaultDuiNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiNotificationManager::DuiNotificationManager() : proxy(QString(), QString(), QDBusConnection::sessionBus())
{
    gDuiNotificationManagerStub->DuiNotificationManagerConstructor();
}

DuiNotificationManager::~DuiNotificationManager()
{
    gDuiNotificationManagerStub->DuiNotificationManagerDestructor();
}

DuiNotificationManager *DuiNotificationManager::instance()
{
    static DuiNotificationManager manager;
    return &manager;
}

uint DuiNotificationManager::addGroup(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent)
{
    return gDuiNotificationManagerStub->addGroup(eventType, summary, body, action, imageURI, count, persistent);
}

uint DuiNotificationManager::addGroup(const QString &eventType, bool persistent)
{
    return gDuiNotificationManagerStub->addGroup(eventType, persistent);
}

uint DuiNotificationManager::addNotification(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, bool persistent)
{
    return gDuiNotificationManagerStub->addNotification(groupId, eventType, summary, body, action, imageURI, count, persistent);
}

uint DuiNotificationManager::addNotification(uint groupId, const QString &eventType, bool persistent)
{
    return gDuiNotificationManagerStub->addNotification(groupId, eventType, persistent);
}

bool DuiNotificationManager::removeGroup(uint groupId)
{
    return gDuiNotificationManagerStub->removeGroup(groupId);
}

bool DuiNotificationManager::removeNotification(uint notificationId)
{
    return gDuiNotificationManagerStub->removeNotification(notificationId);
}

bool DuiNotificationManager::updateGroup(uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gDuiNotificationManagerStub->updateGroup(groupId, eventType, summary, body, action, imageURI, count);
}

bool DuiNotificationManager::updateGroup(uint groupId, const QString &eventType)
{
    return gDuiNotificationManagerStub->updateGroup(groupId, eventType);
}

bool DuiNotificationManager::updateNotification(uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return gDuiNotificationManagerStub->updateNotification(notificationId, eventType, summary, body, action, imageURI, count);
}

bool DuiNotificationManager::updateNotification(uint notificationId, const QString &eventType)
{
    return gDuiNotificationManagerStub->updateNotification(notificationId, eventType);
}

QList<uint> DuiNotificationManager::notificationIdList()
{
    return gDuiNotificationManagerStub->notificationIdList();
}

DuiNotificationManagerProxy::DuiNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

DuiNotificationManagerProxy::~DuiNotificationManagerProxy()
{
}

#endif
