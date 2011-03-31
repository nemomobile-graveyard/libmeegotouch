#include "mpixmaphandle.h"

MPixmapHandle::MPixmapHandle() :
    xHandle(0),
    eglHandle(0),
    shmHandle(QByteArray()),
    format(QImage::Format_Invalid),
    numBytes(0),
    directMap(false)
{}

bool MPixmapHandle::isValid() const
{
    return (!size.isEmpty() && eglHandle && !shmHandle.isEmpty()) ^ (unsigned long)xHandle;
}

