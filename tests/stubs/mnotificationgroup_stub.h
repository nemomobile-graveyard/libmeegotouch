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
#ifndef MNOTIFICATIONGROUP_STUB
#define MNOTIFICATIONGROUP_STUB

#include "mnotificationgroup.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNotificationGroupStub : public StubBase {
  public:
  virtual void MNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body);
  virtual void MNotificationGroupDestructor();
  virtual bool publish();
  virtual bool remove();
  virtual void MNotificationGroupConstructor(uint id);
  uint notificationCount();
}; 

// 2. IMPLEMENT STUB
void MNotificationGroupStub::MNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body) {
  Q_UNUSED(eventType);
  Q_UNUSED(summary);
  Q_UNUSED(body);

}
void MNotificationGroupStub::MNotificationGroupDestructor() {

}
bool MNotificationGroupStub::publish() {
  stubMethodEntered("publish");
  return stubReturnValue<bool>("publish");
}

bool MNotificationGroupStub::remove() {
  stubMethodEntered("remove");
  return stubReturnValue<bool>("remove");
}

void MNotificationGroupStub::MNotificationGroupConstructor(uint id) {
  Q_UNUSED(id);

}

uint MNotificationGroupStub::notificationCount() {
  stubMethodEntered("notificationCount");
  return stubReturnValue<uint>("notificationCount");
}

// 3. CREATE A STUB INSTANCE
MNotificationGroupStub gDefaultMNotificationGroupStub;
MNotificationGroupStub* gMNotificationGroupStub = &gDefaultMNotificationGroupStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNotificationGroup::MNotificationGroup(const QString &eventType, const QString &summary, const QString &body) : MNotification(eventType, summary, body) {
  gMNotificationGroupStub->MNotificationGroupConstructor(eventType, summary, body);
}

MNotificationGroup::~MNotificationGroup() {
  gMNotificationGroupStub->MNotificationGroupDestructor();
}

bool MNotificationGroup::publish() {
  return gMNotificationGroupStub->publish();
}

bool MNotificationGroup::remove() {
  return gMNotificationGroupStub->remove();
}

MNotificationGroup::MNotificationGroup(uint id) : MNotification(id) {
  gMNotificationGroupStub->MNotificationGroupConstructor(id);
}

uint MNotificationGroup::notificationCount() {
  return gMNotificationGroupStub->notificationCount();
}

#endif
