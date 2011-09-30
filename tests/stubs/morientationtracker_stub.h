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
