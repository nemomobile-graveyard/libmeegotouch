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

#ifndef MNOTIFICATIONMANAGER_STUB
#define MNOTIFICATIONMANAGER_STUB

#include "mnotificationmanager.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNotificationManagerStub : public StubBase {
public:
    virtual void MNotificationManagerConstructor();
    virtual void MNotificationManagerDestructor();
    virtual MNotificationManager * instance();
    virtual QList<uint> notificationIdList();
    virtual QList<MNotification> notificationList();
    virtual QList<MNotificationGroup> notificationGroupList();
    virtual uint notificationCountInGroup(uint groupId);
    virtual uint addNotification(uint groupId, const QVariantHash &parameters);
    virtual bool updateNotification(uint notificationId, const QVariantHash &parameters);
    virtual bool removeNotification(uint notificationId);
    virtual uint addGroup(const QVariantHash &parameters);
    virtual bool updateGroup(uint groupId, const QVariantHash &parameters);
    virtual bool removeGroup(uint groupId);
};

// 2. IMPLEMENT STUB
void MNotificationManagerStub::MNotificationManagerConstructor() {

}
void MNotificationManagerStub::MNotificationManagerDestructor() {

}
MNotificationManager * MNotificationManagerStub::instance() {
    stubMethodEntered("instance");
    return stubReturnValue<MNotificationManager *>("instance");
}

QList<uint> MNotificationManagerStub::notificationIdList() {
    stubMethodEntered("notificationIdList");
    return stubReturnValue<QList<uint> >("notificationIdList");
}

QList<MNotification> MNotificationManagerStub::notificationList() {
    stubMethodEntered("notificationList");
    return stubReturnValue<QList<MNotification> >("notificationList");
}

QList<MNotificationGroup> MNotificationManagerStub::notificationGroupList() {
    stubMethodEntered("notificationGroupList");
    return stubReturnValue<QList<MNotificationGroup> >("notificationGroupList");
}

uint MNotificationManagerStub::notificationCountInGroup(uint groupId) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(groupId));
    stubMethodEntered("notificationCountInGroup",params);
    return stubReturnValue<uint>("notificationCountInGroup");
}

uint MNotificationManagerStub::addNotification(uint groupId, const QVariantHash &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(groupId));
    params.append( new Parameter<const QVariantHash >(parameters));
    stubMethodEntered("addNotification",params);
    return stubReturnValue<uint>("addNotification");
}

bool MNotificationManagerStub::updateNotification(uint notificationId, const QVariantHash &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationId));
    params.append( new Parameter<const QVariantHash >(parameters));
    stubMethodEntered("updateNotification",params);
    return stubReturnValue<bool>("updateNotification");
}

bool MNotificationManagerStub::removeNotification(uint notificationId) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(notificationId));
    stubMethodEntered("removeNotification",params);
    return stubReturnValue<bool>("removeNotification");
}

uint MNotificationManagerStub::addGroup(const QVariantHash &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QVariantHash >(parameters));
    stubMethodEntered("addGroup",params);
    return stubReturnValue<uint>("addGroup");
}

bool MNotificationManagerStub::updateGroup(uint groupId, const QVariantHash &parameters) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(groupId));
    params.append( new Parameter<const QVariantHash >(parameters));
    stubMethodEntered("updateGroup",params);
    return stubReturnValue<bool>("updateGroup");
}

bool MNotificationManagerStub::removeGroup(uint groupId) {
    QList<ParameterBase*> params;
    params.append( new Parameter<uint >(groupId));
    stubMethodEntered("removeGroup",params);
    return stubReturnValue<bool>("removeGroup");
}



// 3. CREATE A STUB INSTANCE
MNotificationManagerStub gDefaultMNotificationManagerStub;
MNotificationManagerStub* gMNotificationManagerStub = &gDefaultMNotificationManagerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNotificationManager::MNotificationManager() : proxy(QString(), QString(), QDBusConnection::sessionBus()), userId( 0 ) {
    gMNotificationManagerStub->MNotificationManagerConstructor();
}

MNotificationManager::~MNotificationManager() {
    gMNotificationManagerStub->MNotificationManagerDestructor();
}

MNotificationManager * MNotificationManager::instance() {
    static MNotificationManager manager;
    return &manager;
}

QList<uint> MNotificationManager::notificationIdList() {
    return gMNotificationManagerStub->notificationIdList();
}

QList<MNotification> MNotificationManager::notificationList() {
    return gMNotificationManagerStub->notificationList();
}

QList<MNotificationGroup> MNotificationManager::notificationGroupList() {
    return gMNotificationManagerStub->notificationGroupList();
}

uint MNotificationManager::notificationCountInGroup(uint groupId) {
    return gMNotificationManagerStub->notificationCountInGroup(groupId);
}

uint MNotificationManager::addNotification(uint groupId, const QVariantHash &parameters) {
    return gMNotificationManagerStub->addNotification(groupId, parameters);
}

bool MNotificationManager::updateNotification(uint notificationId, const QVariantHash &parameters) {
    return gMNotificationManagerStub->updateNotification(notificationId, parameters);
}

bool MNotificationManager::removeNotification(uint notificationId) {
    return gMNotificationManagerStub->removeNotification(notificationId);
}

uint MNotificationManager::addGroup(const QVariantHash &parameters) {
    return gMNotificationManagerStub->addGroup(parameters);
}

bool MNotificationManager::updateGroup(uint groupId, const QVariantHash &parameters) {
    return gMNotificationManagerStub->updateGroup(groupId, parameters);
}

bool MNotificationManager::removeGroup(uint groupId) {
    return gMNotificationManagerStub->removeGroup(groupId);
}

MNotificationManagerProxy::MNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

MNotificationManagerProxy::~MNotificationManagerProxy()
{
}

#endif
