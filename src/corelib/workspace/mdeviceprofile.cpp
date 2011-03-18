/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QString>
#include <QSize>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDesktopWidget>

#ifdef HAVE_DBUS
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#endif

#include "mdebug.h"
#include "mdeviceprofile.h"
#include "mdeviceprofile_p.h"

#include "mapplication.h"
#include "mcomponentdata.h"
#include "mcomponentdata_p.h"
#include "mgconfitem.h"

namespace{
#ifdef HAVE_DBUS
    static const QString pixmapProviderDbusService = "com.meego.core.MStatusBar";
#endif
    static const qreal mmPerInch = 25.4;
    static const qreal pointsPerInch = 72.0;
}

MDeviceProfilePrivate::MDeviceProfilePrivate()
    : q_ptr(0)
{
    QString filename;
    QString targetname = MApplication::deviceName();

#ifdef Q_OS_WIN
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();
    appDir.cd("share");
    appDir.cd("meegotouch");
    appDir.cd("targets");
    if(targetname.isEmpty())
        filename = appDir.path().append(QDir::separator())
                                .append("Default.conf");
    else
        filename = appDir.path().append(QDir::separator())
                                .append(targetname).append(".conf");
#else //!Q_OS_WIN

    if (targetname.isEmpty()) {
        #ifdef HAVE_GCONF
        MGConfItem targetNameItem("/meegotouch/target/name");
        targetname = targetNameItem.value("Default").toString();
        #else
        targetname = "Default";
        #endif //HAVE_GCONF
    }

    filename = M_TARGETS_CONF_DIR + QString("/") + targetname + ".conf";

#endif //Q_OS_WIN

    if (!load(filename)) {
        qCritical("Failed to load device profile '%s'", qPrintable(filename));
        autodetection();
    }
}

MDeviceProfilePrivate::~MDeviceProfilePrivate()
{
}

bool MDeviceProfilePrivate::load(const QString& filename)
{
    if (!QFile::exists(filename))
        return false;

    QSettings settings(filename, QSettings::IniFormat);
    if (settings.status() != QSettings::NoError)
        return false;

    if (settings.value("/resolution/X").toString() == "autodetect")
        resolution.setWidth(QApplication::desktop()->screenGeometry().size().width());
    else
        resolution.setWidth(settings.value("/resolution/X", 0).toInt());

    if (settings.value("/resolution/Y").toString() == "autodetect")
        resolution.setHeight(QApplication::desktop()->screenGeometry().size().height());
    else
        resolution.setHeight(settings.value("/resolution/Y", 0).toInt());

    if (settings.value("/ppi/X").toString() == "autodetect") {
        int widthMM = QApplication::desktop()->widthMM();
        if(widthMM){
            int autodetectedPpiX = mmPerInch * QApplication::desktop()->screenGeometry().size().width() / widthMM;
            pixelsPerInch.setWidth(autodetectedPpiX);
        }
    }
    else
        pixelsPerInch.setWidth(settings.value("/ppi/X", 0).toInt());

    if (settings.value("/ppi/Y") == "autodetect") {
        int heightMM = QApplication::desktop()->heightMM();
        if(heightMM){
            int autodetectedPpiY =  mmPerInch * QApplication::desktop()->screenGeometry().size().height() / heightMM;
            pixelsPerInch.setWidth(autodetectedPpiY);
        }
    }
    else
        pixelsPerInch.setHeight(settings.value("/ppi/Y", 0).toInt());

    pixelsPerMm = pixelsPerInch.width() / mmPerInch;
    pixelsPerMmF = pixelsPerInch.width() / mmPerInch;
    pixelsPerPt = pixelsPerInch.height() / pointsPerInch;
    pixelsPerPtF = pixelsPerInch.height() / pointsPerInch;

    if (settings.value("/other/showStatusBar").toString() == "autodetect")
        showStatusBar = hasStatusbarProvider();
    else
        showStatusBar=settings.value("/other/showStatusBar",false).toBool();

    if (settings.value("/allowedOrientations/keyboardOpen").toString() == "autodetect"){
        supportedOrientationsForKeyboardClosed << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
    } else {
        QStringList orientationsForDevOpen = settings.value("/allowedOrientations/keyboardOpen",
                                                            QStringList()).toStringList();
        if (orientationsForDevOpen.contains("0"))
            supportedOrientationsForKeyboardOpen << M::Angle0;
        if (orientationsForDevOpen.contains("90"))
            supportedOrientationsForKeyboardOpen << M::Angle90;
        if (orientationsForDevOpen.contains("180"))
            supportedOrientationsForKeyboardOpen << M::Angle180;
        if (orientationsForDevOpen.contains("270"))
            supportedOrientationsForKeyboardOpen << M::Angle270;
    }

    if (settings.value("/allowedOrientations/keyboardOpen", "").toString() == "autodetect"){
        supportedOrientationsForKeyboardOpen << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
    } else {
        QStringList orientationsForDevClosed = settings.value("/allowedOrientations/keyboardClosed",
                                                              QStringList()).toStringList();
        if (orientationsForDevClosed.contains("0"))
            supportedOrientationsForKeyboardClosed << M::Angle0;
        if (orientationsForDevClosed.contains("90"))
            supportedOrientationsForKeyboardClosed << M::Angle90;
        if (orientationsForDevClosed.contains("180"))
            supportedOrientationsForKeyboardClosed << M::Angle180;
        if (orientationsForDevClosed.contains("270"))
            supportedOrientationsForKeyboardClosed << M::Angle270;
    }

    return true;
}

/*
 * @return pointer to the singleton MDeviceProfile instance
 */
MDeviceProfile *MDeviceProfile::instance()
{
    MComponentData *data = MComponentData::instance();
    if (!data)
    {
        qFatal("There is no instance of MDeviceProfile. Please create MComponentData first.");
        return 0;
    }
    else
    {
        return data->d_ptr->deviceProfile;
    }
}

MDeviceProfile::MDeviceProfile(QObject *parent)
    : QObject(parent),
      d_ptr(new MDeviceProfilePrivate)
{
    Q_D(MDeviceProfile);
    d->q_ptr = this;

    if (!MComponentData::instance())
        qFatal("There is no instance of MComponentData. Please create MApplication first.");
    if (MComponentData::instance()->d_ptr->deviceProfile)
        qFatal("Device profile is already created. Please use MDeviceProfile::instance()");
}

MDeviceProfile::~MDeviceProfile()
{
    delete d_ptr;
}

QSize MDeviceProfile::resolution() const
{
    Q_D(const MDeviceProfile);
    return d->resolution;
}

QSize MDeviceProfile::pixelsPerInch() const
{
    Q_D(const MDeviceProfile);

    return d->pixelsPerInch;
}

bool MDeviceProfile::showStatusbar() const
{
    Q_D(const MDeviceProfile);
    return d->showStatusBar;
}

M::Orientation MDeviceProfile::orientationFromAngle(M::OrientationAngle angle) const
{
    Q_D(const MDeviceProfile);
    M::Orientation orientation;

    if (d->resolution.width() >= d->resolution.height()) {
        // native display orientation is landscape
        if (angle == M::Angle0 || angle == M::Angle180) {
            orientation = M::Landscape;
        } else {
            orientation = M::Portrait;
        }
    } else {
        // native display orientation is portrait
        if (angle == M::Angle0 || angle == M::Angle180) {
            orientation = M::Portrait;
        } else {
            orientation = M::Landscape;
        }
    }

    return orientation;
}

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    Q_D(const MDeviceProfile);
    if (isKeyboardOpen)
        return d->supportedOrientationsForKeyboardOpen.contains(angle);
    else
        return d->supportedOrientationsForKeyboardClosed.contains(angle);
}

void MDeviceProfilePrivate::autodetection()
{
    QRect screenRect = QApplication::desktop()->screenGeometry();
    resolution = screenRect.size();

    pixelsPerInch.setHeight(QApplication::desktop()->physicalDpiY());
    pixelsPerInch.setWidth(QApplication::desktop()->physicalDpiX());

    showStatusBar = hasStatusbarProvider();

    supportedOrientationsForKeyboardClosed << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
    supportedOrientationsForKeyboardOpen << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
}

bool MDeviceProfilePrivate::hasStatusbarProvider()
{
#ifdef HAVE_DBUS
    if (QDBusConnection::sessionBus().interface()->isServiceRegistered(pixmapProviderDbusService))
        return true;
#endif
    return false;
}

int MDeviceProfile::mmToPixels(qreal mm)
{
    Q_D(const MDeviceProfile);

    return mm * d->pixelsPerMm;
}

qreal MDeviceProfile::mmToPixelsF(qreal mm)
{
    Q_D(const MDeviceProfile);

    return mm * d->pixelsPerMmF;
}

int MDeviceProfile::ptToPixels(qreal pt)
{
    Q_D(const MDeviceProfile);
    return pt * d->pixelsPerPt;
}

qreal MDeviceProfile::ptToPixelsF(qreal pt)
{
    Q_D(const MDeviceProfile);
    return pt * d->pixelsPerPtF;
}

int MDeviceProfile::pixelsToPt(int pixels)
{
    Q_D(const MDeviceProfile);
    return pixels / d->pixelsPerPt;
}

qreal MDeviceProfile::pixelsToPtF(qreal pixels)
{
    Q_D(const MDeviceProfile);
    return pixels / d->pixelsPerPtF;
}

