#include "mpixmaphandle.h"

MPixmapHandle::MPixmapHandle() :
    xHandle(0),
    eglHandle(0),
    shmHandle(QString())
{}

bool MPixmapHandle::isValid() const
{
    return (!size.isEmpty() && eglHandle && !shmHandle.isEmpty()) ^ xHandle;
}

