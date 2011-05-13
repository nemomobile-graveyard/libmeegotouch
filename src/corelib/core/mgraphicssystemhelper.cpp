#include "mgraphicssystemhelper.h"

#include "mwindow.h"
#include "../theme/mimagedirectory.h"
#include "../theme/mpixmaphandle.h"
#include "mdebug.h"

#ifdef QT_OPENGL_LIB
#include <QGLFormat>
#include <QGLWidget>
#endif

#include <QSharedMemory>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QFileInfo>
#include <QDebug>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <QtMeeGoGraphicsSystemHelper>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#endif


#ifdef  Q_WS_X11
#include <X11/Xlib.h>
#include <QX11Info>
#endif

namespace {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    void allocateSharedMemory(MPixmapHandle *handle, const QImage& image, const QString &uniqueKey, const QSize &requestedSize) {
        static QString appName = QFileInfo(qApp->arguments().at(0)).fileName();
        // must start with a / but may not contain further slashes
        QCryptographicHash crypto(QCryptographicHash::Md5);
        QString keyWithSize = uniqueKey + '(' + QString::number(requestedSize.width()) +
                              QLatin1Char(',') + QString::number(requestedSize.height()) + QLatin1Char(')');
        crypto.addData(qPrintable(keyWithSize));
        handle->shmHandle = crypto.result().toHex();
        handle->shmHandle.prepend(QByteArray("/") + qPrintable(appName));

        // Create a new memory object
        int fd = shm_open(handle->shmHandle.constData(), O_RDWR | O_CREAT, 0666);
        if (fd == -1) {
            qFatal("Failed to open shared memory: %s", strerror(errno));
        }

        // Set memory objects size
        if (ftruncate(fd, image.numBytes()) == -1) {
            qFatal("ltrunc : %s", strerror(errno));
        }

        void *addr = mmap(0, image.numBytes(), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (addr == (void *) -1) {
            qFatal("mmap failed : %s", strerror(errno));
        }
        close(fd);

        memcpy(addr, image.bits(), image.numBytes());
        munmap(addr, image.numBytes());
    }

    void fillHandleFromImage(MPixmapHandle *handle, const QImage& image)
    {
        handle->eglHandle = QMeeGoGraphicsSystemHelper::imageToEGLSharedImage(image);
        handle->size = image.size();
        handle->format = image.format();
        handle->numBytes = image.numBytes();
    }

#endif

    void createXPixmapFromImage(PixmapCacheEntry *cacheEntry, const QImage& image)
    {
        if (image.width() <= 0 || image.height() <= 0) {
            cacheEntry->handle.xHandle = 0;
            return;
        }

#ifdef  Q_WS_X11
        Pixmap pixmap = XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                                      image.width(), image.height(), image.depth());
        XSync(QX11Info::display(), false);
        cacheEntry->pixmap = new QPixmap();
        *cacheEntry->pixmap = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);
        cacheEntry->pixmap->convertFromImage(image);

        // Sync X-Server, without this the pixmap handle is still invalid in client-side
        XSync(QX11Info::display(), false);
        cacheEntry->handle.xHandle = cacheEntry->pixmap->handle();
#else
        cacheEntry->pixmap = new QPixmap(QPixmap::fromImage(image));
        cacheEntry->handle.xHandle = cacheEntry->pixmap;
#endif // Q_WS_X11
    }

    QPixmap createXPixmapFromHandle(const MPixmapHandle& pixmapHandle) {
#ifdef Q_WS_X11
        if (!pixmapHandle.xHandle) {
            if (pixmapHandle.eglHandle) {
                qFatal("Valid eglHandle received but not running with meego compatible graphicssystem.");
            } else {
                qFatal("No valid handle to create pixmap from received.");
            }
        }
        return QPixmap::fromX11Pixmap(pixmapHandle.xHandle, QPixmap::ImplicitlyShared);
#else // Q_WS_X11
        QPixmap *pixmapPointer = (QPixmap *)(pixmapHandle.xHandle);
        return *pixmapPointer;
#endif // Q_WS_X11
    }
}

void MGraphicsSystemHelper::pixmapFromImage(PixmapCacheEntry *cacheEntry, const QImage& image, const QString &uniqueKey, const QSize &requestedSize)
{
#ifndef HAVE_MEEGOGRAPHICSSYSTEM
    Q_UNUSED(uniqueKey);
    Q_UNUSED(requestedSize);
#else
    if (QMeeGoGraphicsSystemHelper::isRunningMeeGo()) {
        QImage imageCopy;
        // the only supported formats are ARGB32_Premultiplied and RGB32 as this are the formats
        // the Qt raster engine can use without a conversion
        // TODO: once the MeeGo graphicssystem allows QImage::Format_RGB32 we can also allow it
        if (image.format() != QImage::Format_ARGB32_Premultiplied /*&& image.format() != QImage::Format_RGB32*/) {
            imageCopy = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        } else {
            imageCopy = image;
        }

        fillHandleFromImage(&cacheEntry->handle, imageCopy);
        if (!cacheEntry->handle.directMap) {
            allocateSharedMemory(&cacheEntry->handle, imageCopy, uniqueKey, requestedSize);
        }
    } else
#endif // HAVE_MEEGO_GRAPHICSSYSTEM
    {
        createXPixmapFromImage(cacheEntry, image);
    }
}

QPixmap MGraphicsSystemHelper::pixmapFromHandle(const MPixmapHandle& pixmapHandle, void **addr, int *numBytes) {
#ifndef HAVE_MEEGOGRAPHICSSYSTEM
    Q_UNUSED(addr);
    Q_UNUSED(numBytes);
#else
    if (MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem() && pixmapHandle.eglHandle) {
        int fd = -1;
        if (pixmapHandle.directMap)
            fd = open(pixmapHandle.shmHandle.constData(), O_RDONLY);
        else
            fd = shm_open(pixmapHandle.shmHandle.constData(), O_RDONLY, 0444);

        if (fd == -1) {
            qFatal("Failed to open shared memory: %s, %s", strerror(errno), pixmapHandle.shmHandle.constData());
        }

        *addr = mmap(0, pixmapHandle.numBytes, PROT_READ, MAP_SHARED, fd, 0);
        *numBytes = pixmapHandle.numBytes;
        close(fd);
        if (*addr == (void *) -1) {
            qFatal("mmap failed : %s", strerror(errno));
        }

        QImage image((const uchar *)*addr, pixmapHandle.size.width(), pixmapHandle.size.height(), pixmapHandle.format);

        return QMeeGoGraphicsSystemHelper::pixmapFromEGLSharedImage(pixmapHandle.eglHandle, image);
    } else
#endif // HAVE_MEEGOGRAPHICSSYSTEM
    {
        return createXPixmapFromHandle(pixmapHandle);
    }
}


/**
  * Returns true when the native graphicssystem is used.
  * If there is no support for the MeeGo graphicssystem true is always returned.
  */
bool MGraphicsSystemHelper::isRunningNativeGraphicsSystem() {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    return QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("native");
#else
    return true;
#endif
}

bool MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem() {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    if (QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("meego") ||
        (QMeeGoGraphicsSystemHelper::isRunningRuntime() &&
         QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("raster"))) {
        return true;
    }
    return false;
#else
    return false;
#endif
}

bool MGraphicsSystemHelper::isRunningMeeGoGraphicsSystem() {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    if (QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("meego")) {
        return true;
    }
#endif
    return false;
}

bool MGraphicsSystemHelper::canSwitchBetweenSoftwareAndHardwareRendering()
{
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    return QMeeGoGraphicsSystemHelper::isRunningRuntime() &&
        (QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("raster") ||
         QMeeGoGraphicsSystemHelper::runningGraphicsSystemName() == QLatin1String("meego"));
#else
    return true;
#endif
}

void MGraphicsSystemHelper::enableSwitchEvents()
{
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    if (MGraphicsSystemHelper::isRunningMeeGoCompatibleGraphicsSystem()) {
        QMeeGoGraphicsSystemHelper::enableSwitchEvents();
    }
#endif
}
