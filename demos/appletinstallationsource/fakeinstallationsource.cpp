/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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
#include <DuiButton>
#include <DuiAppletInstantiator>
#include <QGraphicsGridLayout>
#include "fakeinstallationsource.h"
#include <stdlib.h>

Q_EXPORT_PLUGIN2(fakeinstallationsource, FakeInstallationSource)

FakeInstallationSource::FakeInstallationSource() : source(new InstallationSourceWidget())
{
}

FakeInstallationSource::~FakeInstallationSource()
{
    delete source;
}

DuiWidget *FakeInstallationSource::widget()
{
    return source;
}

bool FakeInstallationSource::initialize(const QString& )
{
    return true;
}

InstallationSourceWidget::InstallationSourceWidget()
{
    QGraphicsGridLayout *layout = new QGraphicsGridLayout;
    setLayout(layout);

    // Button for installing a package from a local file
    DuiButton *button = new DuiButton(this);
    button->setText("Install local applet");
    layout->addItem(button, 0, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(installApplet()));

    // Buttons for different test cases
    button = new DuiButton(this);
    button->setText("Installable");
    layout->addItem(button, 0, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(installableApplet()));

    button = new DuiButton(this);
    button->setText("Uninstallable");
    layout->addItem(button, 1, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(uninstallableApplet()));

    button = new DuiButton(this);
    button->setText("Not Downloadable");
    layout->addItem(button, 1, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(notDownloadableApplet()));

    button = new DuiButton(this);
    button->setText("Metadata Only");
    layout->addItem(button, 2, 0);
    connect(button, SIGNAL(clicked()), this, SLOT(metadataOnly()));

    button = new DuiButton(this);
    button->setText("Non-existent");
    layout->addItem(button, 2, 1);
    connect(button, SIGNAL(clicked()), this, SLOT(nonExistent()));
}

void InstallationSourceWidget::installApplet()
{
    if (char *package = getenv("FAKEAPPLETINSTALLATIONSOURCEPACKAGE")) {
        qDebug() << "Local package" << package;
        emit packageSelectedForInstallation(package);
    } else {
        qDebug() << "Local Installable package";
        emit packageSelectedForInstallation("./fakeappletpackage-installable.deb");
    }
}

void InstallationSourceWidget::installableApplet()
{
    qDebug() << "Installable package";
    emit packageSelectedForInstallation("fakeappletpackage-installable.deb");
}

void InstallationSourceWidget::uninstallableApplet()
{
    qDebug() << "UnInstallable package";
    emit packageSelectedForInstallation("fakeappletpackage-uninstallable.deb");
}

void InstallationSourceWidget::notDownloadableApplet()
{
    qDebug() << "Not downloadable package";
    emit packageSelectedForInstallation("fakeappletpackage-notdownloadable.deb");
}

void InstallationSourceWidget::metadataOnly()
{
    qDebug() << "Metadata only";
    emit packageSelectedForInstallation("fakeappletpackage-metadataonly.deb");
}

void InstallationSourceWidget::nonExistent()
{
    qDebug() << "Non existent package";
    emit packageSelectedForInstallation("fakeappletpackage-nonexistent.deb");
}
