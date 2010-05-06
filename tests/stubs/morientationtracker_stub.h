#ifndef MORIENTATIONTRACKER_STUB
#define MORIENTATIONTRACKER_STUB

#include "morientationtracker.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MOrientationTrackerStub : public StubBase {
  public:
  virtual M::OrientationAngle orientationAngle() const;
  virtual void MOrientationTrackerConstructor();
  virtual void MOrientationTrackerDestructor();
}; 

// 2. IMPLEMENT STUB
M::OrientationAngle MOrientationTrackerStub::orientationAngle() const {
  stubMethodEntered("orientationAngle");
  return stubReturnValue<M::OrientationAngle>("orientationAngle");
}

void MOrientationTrackerStub::MOrientationTrackerConstructor() {

}
void MOrientationTrackerStub::MOrientationTrackerDestructor() {

}


// 3. CREATE A STUB INSTANCE
MOrientationTrackerStub gDefaultMOrientationTrackerStub;
MOrientationTrackerStub* gMOrientationTrackerStub = &gDefaultMOrientationTrackerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MOrientationTracker * MOrientationTracker::instance() {
  static MOrientationTracker tracker;
  return &tracker;
}

M::OrientationAngle MOrientationTracker::orientationAngle() const {
  return gMOrientationTrackerStub->orientationAngle();
}

MOrientationTracker::MOrientationTracker() : d_ptr(NULL) {
  gMOrientationTrackerStub->MOrientationTrackerConstructor();
}

MOrientationTracker::~MOrientationTracker() {
  gMOrientationTrackerStub->MOrientationTrackerDestructor();
}


#endif
