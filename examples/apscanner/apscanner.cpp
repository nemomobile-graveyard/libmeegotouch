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

#include <QTimer>
#include "apscanner.h"

APScanner::APScanner() : timer (NULL)
{
    QStringList apNames = QStringList() << "Home"
                          << "Private"
                          << "Zyxel"
                          << "Kotiverkko"
                          << "Shared"
                          << "Tampere 1"
                          << "VoIP"
                          << "SWZone"
                          << "UTA"
                          << "PWLAN"
                          << "mobileAP"
                          << "ARP0482"
                          << "Koskikatu"
                          << "HimosHotelli"
                          << "DLinkWLAN";

    QStringList apEncryptions = QStringList() << "Free" << "WEP" << "WPA" << "WPA2";

    // Populate accessPointInfos list. Only signal strengths will be changed later on.
    while (apNames.count()) {
        AccessPointInfo apInfo;
        apInfo.signalStrength = qrand() % 4;  // initial signal strength varies from 0 to 3
        apInfo.stationName = apNames.takeAt(qrand() % apNames.count());
        apInfo.encryption = apEncryptions.at(qrand() % apEncryptions.count());
        accessPointInfos << apInfo;
    }
}

APScanner::~APScanner()
{
}

void APScanner::startScanning()
{
    // Emit initial accessPointInfos if they have positive signal strength
    for (int i = 0; i < accessPointInfos.count(); ++i) {
        if (accessPointInfos.at(i).signalStrength > 0)
            emit accessPointUpdate(accessPointInfos.at(i));
    }

    // Start timer to update access point infos
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateAccessPoints()));
    timer->start(1000);
}

void APScanner::updateAccessPoints()
{
    // Update accessPointInfos signal strength and emit updated infos
    for (int i = 0; i < accessPointInfos.count(); ++i) {
        int newStrength = (qrand() % 3 - 1) + accessPointInfos.at(i).signalStrength;
        if (accessPointInfos.at(i).signalStrength != newStrength && newStrength >= 0 && newStrength <= 5) {

            // Update signal strength and emit updated access point info
            accessPointInfos[i].signalStrength = newStrength;
            emit accessPointUpdate(accessPointInfos.at(i));
        }
    }
}
