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



#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <unistd.h>

//#define SHARED_IMAGE_FAILURE_LOGGING
#ifdef SHARED_IMAGE_FAILURE_LOGGING
namespace DirtyLogger
{
    void logMessage(const char *fmt, ...);
}
#endif


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
#include <QRgb>

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

        QPixmap ret = QMeeGoGraphicsSystemHelper::pixmapFromEGLSharedImage(pixmapHandle.eglHandle, image);

        if(ret.isNull() || !ret.size().isValid()) {
#ifdef SHARED_IMAGE_FAILURE_LOGGING
            DirtyLogger::logMessage("QMeeGoGraphicsSystemHelper::pixmapFromEGLSharedImage failed\n");

            QString imageFileName;

            for(unsigned i = 0; i < 65536; i++) {
                QString tmp = QString("/home/user/MyDocs/cyan-rectangle%1.png").arg(i);

                if(!QFile::exists(tmp)) {
                    imageFileName = tmp;
                    break;
                }
            }

            if(!imageFileName.isEmpty()) {
                if(image.save(imageFileName))
                    DirtyLogger::logMessage("saved image contents to %s\n",
                                            qPrintable(imageFileName));
                else
                    DirtyLogger::logMessage("failed to save image contents to %s\n",
                                            qPrintable(imageFileName));
            }
            DirtyLogger::logMessage("MPixmapHandle is %s\n",
                                    pixmapHandle.isValid() ?
                                   "valid" : "invalid");
            DirtyLogger::logMessage("info:\n"
			            "  size: (%d,%d)\n"
				    "  direct map: %s\n"
				    "  bytes: %d\n"
				    "  format: %d\n",
				    pixmapHandle.size.width(),
				    pixmapHandle.size.height(),
				    pixmapHandle.directMap ? "true" : "false",
				    pixmapHandle.numBytes, pixmapHandle.format);

            // creating shared pixmap failed - have local one instead of cyan
            // rectangle ...
            // TODO: the below takes the soft image, and tins it cyan. remove
            // this code once we are done with debugging and directly use the
            // soft image
            QImage img = image;
            for (int i = 0; i < img.height(); ++i) {
                uchar * scanLine = img.scanLine(i);
                for (int j = 0; j < image.width(); ++j) {
                    QRgb *pixel = reinterpret_cast<QRgb*>(scanLine);
                    int alpha = qMax(20, qAlpha(*pixel));
                    int red = 0;
                    int green = qMax(130, qGreen(*pixel));
                    int blue = qMax(130, qBlue(*pixel));
                    *pixel = qRgba(red, green, blue, alpha);
                    scanLine += sizeof(QRgb);
                }
            }
            ret = QPixmap::fromImage(img);
#else
            ret  = QPixmap::fromImage(image);
#endif
        }

        return ret;
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

void MGraphicsSystemHelper::forceSoftwareRendering()
{
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
    QMeeGoGraphicsSystemHelper::switchToRaster();
    QMeeGoGraphicsSystemHelper::setSwitchPolicy(QMeeGoGraphicsSystemHelper::NoSwitch);
#endif
}


#ifdef SHARED_IMAGE_FAILURE_LOGGING
namespace DirtyLogger
{
  bool initialized = false;


  char *filename = 0;
}


  // temporary cludge that needs to be removed
void DirtyLogger::logMessage(const char *fmt, ...)
{
  if(!initialized) {
    initialized = true;

    pid_t pid = getpid();

    unsigned l = strlen("/home/user/MyDocs/cyan_rectangles-.txt");

    for(signed d = pid; d; d /= 10)
      l++;

    filename = new char[l+1];

    sprintf(filename, "/home/user/MyDocs/cyan-rectangles_%d.txt", pid);

    filename[l] = 0;
  }

  if(!filename)
    return;

  va_list al;

  va_start(al, fmt);

  FILE *f = fopen(filename, "a");
  if(f) {
    vfprintf(f, fmt, al);

    fclose(f);
  }

  va_end(al);
}
#endif //Logging for shared image failure
