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

#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MList>
#include "apscannerpage.h"
#include "apitemcreator.h"
#include "apitemmodel.h"

APScannerPage::APScannerPage()
{
    setTitle("APScanner");
}

APScannerPage::~APScannerPage()
{
    delete scanner;
}

void APScannerPage::createContent()
{
    QGraphicsWidget *panel = centralWidget();
    MLayout *layout = new MLayout(panel);
    layout->setAnimation(NULL);
    panel->setLayout(layout);
    MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Vertical);

    // MList with fast view
    list = new MList(panel);
    list->setViewType("fast");
    list->setObjectName("wgList");
    list->setSelectionMode(MList::MultiSelection);
    list->setShowGroups(false);

    // Content item creator and item model for the list
    APItemCreator *cellCreator = new APItemCreator();
    list->setCellCreator(cellCreator);
    model = new APItemModel();
    cellCreator->model = model;
    list->setItemModel(model);
    policy->addItem(list);

    // Start scanning access points
    scanner = new APScanner();
    connect(scanner, SIGNAL(accessPointUpdate(APScanner::AccessPointInfo)), this, SLOT(accessPointUpdate(APScanner::AccessPointInfo)));
    scanner->startScanning();
}

void APScannerPage::accessPointUpdate(const APScanner::AccessPointInfo &accessPoint)
{
    int accessPointDataIndex = model->accessPointDataIndex(accessPoint.stationName);

    // Handle adding, removing and updating list items
    if (accessPointDataIndex == -1) {
        model->accessPointInfo = accessPoint;
        model->insertRows(model->rowCount(), 1);
    } else if (accessPoint.signalStrength == 0) {
        model->removeRows(accessPointDataIndex, 1);
    } else {
        model->accessPointInfo = accessPoint;
        model->updateAccessPointData(accessPointDataIndex);
    }

    // Set application title to indicate found action points
    setTitle("AP Scanner (" + QString::number(model->rowCount()) + " found)");
}
