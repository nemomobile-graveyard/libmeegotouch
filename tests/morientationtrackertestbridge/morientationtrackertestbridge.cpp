#include "morientationtrackertestbridge.h"

MOrientationTrackerTestBridge::MOrientationTrackerTestBridge(QObject *parent)
    : QObject(parent)
{
    setObjectName("_m_testBridge");
}

void MOrientationTrackerTestBridge::doUpdateOrientationAngle(M::OrientationAngle angle, bool isKeyboardOpen,
                                                             bool isDeviceFlat, bool tvIsConnected)
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "doUpdateOrientationAngle",
                              Qt::DirectConnection,
                              Q_ARG(M::OrientationAngle, angle),
                              Q_ARG(bool, isKeyboardOpen),
                              Q_ARG(bool, isDeviceFlat),
                              Q_ARG(bool, tvIsConnected));
}
#ifdef Q_WS_X11
void MOrientationTrackerTestBridge::handleCurrentAppWindowChange()
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "handleCurrentAppWindowChange",
                              Qt::DirectConnection);

}

void MOrientationTrackerTestBridge::handleCurrentAppWindowOrientationAngleChange()
{
    QObject *testInterface = children()[0];

    QMetaObject::invokeMethod(testInterface, "handleCurrentAppWindowOrientationAngleChange",
                              Qt::DirectConnection);
}
#endif
