#ifndef DUINOTIFICATIONGROUP_STUB
#define DUINOTIFICATIONGROUP_STUB

#include "duinotificationgroup.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiNotificationGroupStub : public StubBase {
  public:
  virtual void DuiNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body);
  virtual void DuiNotificationGroupDestructor();
  virtual bool publish();
  virtual bool remove();
  virtual void DuiNotificationGroupConstructor(uint id);
}; 

// 2. IMPLEMENT STUB
void DuiNotificationGroupStub::DuiNotificationGroupConstructor(const QString &eventType, const QString &summary, const QString &body) {
  Q_UNUSED(eventType);
  Q_UNUSED(summary);
  Q_UNUSED(body);

}
void DuiNotificationGroupStub::DuiNotificationGroupDestructor() {

}
bool DuiNotificationGroupStub::publish() {
  stubMethodEntered("publish");
  return stubReturnValue<bool>("publish");
}

bool DuiNotificationGroupStub::remove() {
  stubMethodEntered("remove");
  return stubReturnValue<bool>("remove");
}

void DuiNotificationGroupStub::DuiNotificationGroupConstructor(uint id) {
  Q_UNUSED(id);

}


// 3. CREATE A STUB INSTANCE
DuiNotificationGroupStub gDefaultDuiNotificationGroupStub;
DuiNotificationGroupStub* gDuiNotificationGroupStub = &gDefaultDuiNotificationGroupStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiNotificationGroup::DuiNotificationGroup(const QString &eventType, const QString &summary, const QString &body) : DuiNotification(eventType, summary, body) {
  gDuiNotificationGroupStub->DuiNotificationGroupConstructor(eventType, summary, body);
}

DuiNotificationGroup::~DuiNotificationGroup() {
  gDuiNotificationGroupStub->DuiNotificationGroupDestructor();
}

bool DuiNotificationGroup::publish() {
  return gDuiNotificationGroupStub->publish();
}

bool DuiNotificationGroup::remove() {
  return gDuiNotificationGroupStub->remove();
}

DuiNotificationGroup::DuiNotificationGroup(uint id) : DuiNotification(id) {
  gDuiNotificationGroupStub->DuiNotificationGroupConstructor(id);
}


#endif
