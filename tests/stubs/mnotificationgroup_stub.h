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
