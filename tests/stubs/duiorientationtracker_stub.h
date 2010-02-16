#ifndef DUIORIENTATIONTRACKER_STUB
#define DUIORIENTATIONTRACKER_STUB

#include "duiorientationtracker.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiOrientationTrackerStub : public StubBase {
  public:
  virtual Dui::OrientationAngle orientationAngle() const;
  virtual void DuiOrientationTrackerConstructor();
  virtual void DuiOrientationTrackerDestructor();
}; 

// 2. IMPLEMENT STUB
Dui::OrientationAngle DuiOrientationTrackerStub::orientationAngle() const {
  stubMethodEntered("orientationAngle");
  return stubReturnValue<Dui::OrientationAngle>("orientationAngle");
}

void DuiOrientationTrackerStub::DuiOrientationTrackerConstructor() {

}
void DuiOrientationTrackerStub::DuiOrientationTrackerDestructor() {

}


// 3. CREATE A STUB INSTANCE
DuiOrientationTrackerStub gDefaultDuiOrientationTrackerStub;
DuiOrientationTrackerStub* gDuiOrientationTrackerStub = &gDefaultDuiOrientationTrackerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiOrientationTracker * DuiOrientationTracker::instance() {
  static DuiOrientationTracker tracker;
  return &tracker;
}

Dui::OrientationAngle DuiOrientationTracker::orientationAngle() const {
  return gDuiOrientationTrackerStub->orientationAngle();
}

DuiOrientationTracker::DuiOrientationTracker() : d_ptr(NULL) {
  gDuiOrientationTrackerStub->DuiOrientationTrackerConstructor();
}

DuiOrientationTracker::~DuiOrientationTracker() {
  gDuiOrientationTrackerStub->DuiOrientationTrackerDestructor();
}


#endif
