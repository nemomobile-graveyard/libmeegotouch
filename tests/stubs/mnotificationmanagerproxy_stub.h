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
#ifndef MNOTIFICATIONMANAGERPROXY_STUB
#define MNOTIFICATIONMANAGERPROXY_STUB

#include "mnotificationmanagerproxy.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNotificationManagerProxyStub : public StubBase {
  public:
  virtual void MNotificationManagerProxyConstructor(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent);
  virtual void MNotificationManagerProxyDestructor();
}; 

// 2. IMPLEMENT STUB
void MNotificationManagerProxyStub::MNotificationManagerProxyConstructor(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) {
  Q_UNUSED(service);
  Q_UNUSED(path);
  Q_UNUSED(connection);
  Q_UNUSED(parent);

}
void MNotificationManagerProxyStub::MNotificationManagerProxyDestructor() {

}


// 3. CREATE A STUB INSTANCE
MNotificationManagerProxyStub gDefaultMNotificationManagerProxyStub;
MNotificationManagerProxyStub* gMNotificationManagerProxyStub = &gDefaultMNotificationManagerProxyStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNotificationManagerProxy::MNotificationManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) : QDBusAbstractInterface(QString(), QString(), "", connection, parent) {
  gMNotificationManagerProxyStub->MNotificationManagerProxyConstructor(service, path, connection, parent);
}

MNotificationManagerProxy::~MNotificationManagerProxy() {
  gMNotificationManagerProxyStub->MNotificationManagerProxyDestructor();
}


#endif
