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

#include <MViewCreator>
#include <MSceneManager>
#include <MOverlay>
#include <MButton>
#include <MLayout>
#include <MFlowLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <MContainer>
#include <MLocale>
#include "mappletinventory.h"
#include "mappletinventoryview.h"
#include "mappletinventoryview_p.h"
#include "mapplicationextensionarea.h"
#include "mapplicationextensioninterface.h"

MAppletInventoryViewPrivate::MAppletInventoryViewPrivate() :
    controller(NULL),
    layout(new MLayout),
    layoutPolicy(new MLinearLayoutPolicy(layout, Qt::Vertical)),
    appletLayout(new MLayout),
    appletLayoutPolicy(new MFlowLayoutPolicy(appletLayout)),
    appletContainer(NULL),
    closeButtonOverlay(new MOverlay),
    closeButton(new MButton(closeButtonOverlay)),
    q_ptr(NULL)
{
}

MAppletInventoryViewPrivate::~MAppletInventoryViewPrivate()
{
    delete closeButtonOverlay;
}

void MAppletInventoryViewPrivate::init(MAppletInventory *controller)
{
    this->controller = controller;

    // Create a container for the applets
    //~ uispec-document ??? FIXME
    //% "Applets"
    appletContainer = new MContainer(qtTrId("mappletinventoryview_appletcontainer"), controller);
    appletContainer->setObjectName("MAppletInventoryAppletContainer");
    appletContainer->centralWidget()->setLayout(appletLayout);
    appletLayout->setContentsMargins(0, 0, 0, 0);
    appletLayoutPolicy->setStyleName("MAppletInventoryFlowLayoutPolicy");

    // Define layout properties
    controller->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layoutPolicy->setStyleName("MAppletInventoryLinearLayoutPolicy");
    layoutPolicy->addItem(appletContainer);

    // Set close button properties
    closeButton->setViewType("icon");
    closeButton->setObjectName("MAppletInventoryCloseButton");
    closeButton->setIconID("icon-m-framework-close");
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(hideAppletInventory()));

    closeButtonOverlay->setObjectName("MAppletInventoryCloseButtonOverlay");

    MApplicationExtensionArea *area = new MApplicationExtensionArea("com.meego.core.MAppletInstallationSourceInterface/1.0");
    layoutPolicy->addItem(area);
    QObject::connect(area, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setMAppletInventoryInterface(MApplicationExtensionInterface*)));
    area->init();
}

MAppletInventoryView::MAppletInventoryView(MAppletInventory *container) :
    MExtendingBackgroundView(container),
    d_ptr(new MAppletInventoryViewPrivate())
{
    Q_D(MAppletInventoryView);
    d->init(container);
}

MAppletInventoryView::~MAppletInventoryView()
{
    delete d_ptr;
}

void MAppletInventoryView::updateData(const QList<const char *>& modifications)
{
    Q_D(const MAppletInventoryView);

    MExtendingBackgroundView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == MAppletInventoryModel::Widgets) {
            // Remove all widgets from the layout (do not destroy them)
            while (d->appletLayout->count() > 0) {
                d->appletLayout->removeAt(0);
            }

            // Add widgets from the model to the layout
            foreach(MWidget * widget, model()->widgets()) {
                d->appletLayoutPolicy->addItem(widget);
            }
        } else if (member == MAppletInventoryModel::CloseButtonVisible) {
            // Set the visibility of the button
            if (model()->closeButtonVisible()) {
                d->controller->sceneManager()->appearSceneWindow(d->closeButtonOverlay);
            } else {
                d->controller->sceneManager()->disappearSceneWindow(d->closeButtonOverlay);
            }
        }
    }
}


void MAppletInventoryView::connectContainerToInstallationSource(MContainer *container, MWidget *sourceWidget) const
{
    const QMetaObject *mob = sourceWidget->metaObject();

    // use properties to fill header of the container
    int iconProperty  = mob->indexOfProperty("installationSourceIcon");
    int titleProperty = mob->indexOfProperty("installationSourceTitle");
    int textProperty  = mob->indexOfProperty("installationSourceText");

    if (iconProperty != -1) {
        container->setIconID((mob->property(iconProperty).read(sourceWidget)).toString());
    }

    if (titleProperty != -1) {
        container->setTitle((mob->property(titleProperty).read(sourceWidget)).toString());
        container->setHeaderVisible(true);
    }

    if (textProperty != -1) {
        container->setText((mob->property(textProperty).read(sourceWidget)).toString());
    }

    // connect signals from source to the container
    if (mob->indexOfSignal("installationSourceIconChanged(QString)") != -1) {
        QObject::connect(sourceWidget, SIGNAL(installationSourceIconChanged(QString)), container, SLOT(setIconID(QString)));
    }

    if (mob->indexOfSignal("installationSourceTitleChanged(QString)") != -1) {
        QObject::connect(sourceWidget, SIGNAL(installationSourceTitleChanged(QString)), container, SLOT(setTitle(QString)));
    }

    if (mob->indexOfSignal("installationSourceTextChanged(QString)") != -1) {
        QObject::connect(sourceWidget, SIGNAL(installationSourceTextChanged(QString)), container, SLOT(setText(QString)));
    }
}


M_REGISTER_VIEW_NEW(MAppletInventoryView, MAppletInventory)
