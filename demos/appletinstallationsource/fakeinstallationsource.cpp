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

#include <QtGui>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusPendingReply>
#include <MButton>
#include <mappletinstantiator.h>
#include <QGraphicsGridLayout>
#include "fakeinstallationsource.h"
#include <stdlib.h>

Q_EXPORT_PLUGIN2(fakeinstallationsource, FakeInstallationSource)

FakeInstallationSource::FakeInstallationSource() : source(new InstallationSourceWidget(*this)),
    appletInventory(NULL)
{
}

FakeInstallationSource::~FakeInstallationSource()
{
    delete source;
    delete appletInventory;
}

QGraphicsWidget *FakeInstallationSource::widget()
{
    return source;
}

bool FakeInstallationSource::initialize(const QString& )
{
    return true;
}

void FakeInstallationSource::setMAppletInventoryInterface(MAppletInventoryInterface &appletInventory)
{
    this->appletInventory = &appletInventory;
}

MAppletInventoryInterface* FakeInstallationSource::appletInventoryInterface() const
{
    return appletInventory;
}

InstallationSourceWidget::InstallationSourceWidget(FakeInstallationSource &source) :
        installationSource(source)
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    setLayout(layout);

    // Button for installing a package from a local file
    MButton *button = new MButton(this);
    button->setText("Install local applet");
    layout->addItem(button, 0, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(installApplet()));

    // Buttons for different test cases
    button = new MButton(this);
    button->setText("Installable");
    layout->addItem(button, 0, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(installableApplet()));

    button = new MButton(this);
    button->setText("Uninstallable");
    layout->addItem(button, 1, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(uninstallableApplet()));

    button = new MButton(this);
    button->setText("Not Downloadable");
    layout->addItem(button, 1, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(notDownloadableApplet()));

    button = new MButton(this);
    button->setText("Metadata Only");
    layout->addItem(button, 2, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(metadataOnly()));

    button = new MButton(this);
    button->setText("Non-existent");
    layout->addItem(button, 2, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(nonExistent()));
}

void InstallationSourceWidget::installApplet()
{
    if (char *package = getenv("FAKEAPPLETINSTALLATIONSOURCEPACKAGE")) {
        qDebug() << "Local package" << package;
        instantiateAppletsInPackage(package);
    } else {
        qDebug() << "Local Installable package";
        instantiateAppletsInPackage("./fakeappletpackage-installable.deb");
    }
}

void InstallationSourceWidget::installableApplet()
{
    qDebug() << "Installable package";
    instantiateAppletsInPackage("fakeappletpackage-installable.deb");
}

void InstallationSourceWidget::uninstallableApplet()
{
    qDebug() << "UnInstallable package";
    instantiateAppletsInPackage("fakeappletpackage-uninstallable.deb");
}

void InstallationSourceWidget::notDownloadableApplet()
{
    qDebug() << "Not downloadable package";
    instantiateAppletsInPackage("fakeappletpackage-notdownloadable.deb");
}

void InstallationSourceWidget::metadataOnly()
{
    qDebug() << "Metadata only";
    instantiateAppletsInPackage("fakeappletpackage-metadataonly.deb");
}

void InstallationSourceWidget::nonExistent()
{
    qDebug() << "Non existent package";
    instantiateAppletsInPackage("fakeappletpackage-nonexistent.deb");
}

void InstallationSourceWidget::instantiateAppletsInPackage(const QString &packageName)
{
    MAppletInventoryInterface *inventory = installationSource.appletInventoryInterface();
    if (inventory != NULL) {
        inventory->instantiateAppletsInPackage(packageName);
    }
}
