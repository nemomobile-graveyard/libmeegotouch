// The common part of the workaround(s) used to fix some unit tests failing on scratchbox (i386)
// due to the implementation of the portrait mode being different than on the device.

#ifndef TESTS_SBOX_PORTRAIT_SUPPORT_H
#define TESTS_SBOX_PORTRAIT_SUPPORT_H
#ifndef __arm__

#include <MDeviceProfile>
#include <MSceneManager>

M::Orientation MDeviceProfile::orientationFromAngle(M::OrientationAngle angle) const
{
    if (angle == M::Angle90 || angle == M::Angle270)
        return M::Portrait;
    return M::Landscape;
}

QSize MSceneManager::visibleSceneSize(M::Orientation orientation) const
{
    QSize s = MDeviceProfile::instance()->resolution();

    if ((s.height() < s.width() && orientation == M::Portrait) ||
        (s.height() > s.width() && orientation == M::Landscape))
        return QSize(s.height(), s.width());

    return s;
}

#endif // !__arm__
#endif // !TESTS_SBOX_PORTRAIT_SUPPORT_H
