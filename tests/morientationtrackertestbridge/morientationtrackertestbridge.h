#ifndef MORIENTATIONTRACKERTESTBRIDGE_H
#define MORIENTATIONTRACKERTESTBRIDGE_H

#include <MNamespace>
#include "morientationtracker.h"

class MOrientationTrackerTestBridge : public QObject
{
    Q_OBJECT
public:
    MOrientationTrackerTestBridge(QObject *parent = 0);

    void doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                  bool isDeviceFlat, bool tvIsConnected);
#ifdef Q_WS_X11
    void handleCurrentAppWindowChange();
    void handleCurrentAppWindowOrientationAngleChange();
#endif
};

#endif // MORIENTATIONTRACKERTESTBRIDGE_H
