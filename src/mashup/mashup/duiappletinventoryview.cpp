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

#include <DuiViewCreator>
#include <DuiSceneManager>
#include <DuiOverlay>
#include <DuiButton>
#include <DuiLayout>
#include <DuiFlowLayoutPolicy>
#include <DuiLinearLayoutPolicy>
#include <DuiContainer>
#include <DuiLocale>
#include "duiappletinventory.h"
#include "duiappletinventoryview.h"
#include "duiappletinventoryview_p.h"
#include "duiapplicationextensionarea.h"
#include "duiapplicationextensioninterface.h"

DuiAppletInventoryViewPrivate::DuiAppletInventoryViewPrivate() :
    controller(NULL),
    layout(new DuiLayout),
    layoutPolicy(new DuiLinearLayoutPolicy(layout, Qt::Vertical)),
    appletLayout(new DuiLayout),
    appletLayoutPolicy(new DuiFlowLayoutPolicy(appletLayout)),
    appletContainer(NULL),
    closeButtonOverlay(new DuiOverlay),
    closeButton(new DuiButton(closeButtonOverlay))
{
}

DuiAppletInventoryViewPrivate::~DuiAppletInventoryViewPrivate()
{
    delete closeButtonOverlay;
}

void DuiAppletInventoryViewPrivate::init(DuiAppletInventory *controller)
{
    this->controller = controller;

    // Create a container for the applets
    //~ uispec-document ??? FIXME
    //% "Applets"
    appletContainer = new DuiContainer(qtTrId("duiappletinventoryview_appletcontainer"), controller);
    appletContainer->setObjectName("DuiAppletInventoryAppletContainer");
    appletContainer->centralWidget()->setLayout(appletLayout);
    appletLayout->setContentsMargins(0, 0, 0, 0);
    appletLayoutPolicy->setObjectName("DuiAppletInventoryFlowLayoutPolicy");

    // Define layout properties
    controller->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layoutPolicy->setObjectName("DuiAppletInventoryLinearLayoutPolicy");
    layoutPolicy->addItem(appletContainer);

    // Set close button properties
    closeButton->setViewType("icon");
    closeButton->setObjectName("DuiAppletInventoryCloseButton");
    closeButton->setIconID("Icon-close");
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(hideAppletInventory()));

    closeButtonOverlay->setObjectName("DuiAppletInventoryCloseButtonOverlay");

    DuiApplicationExtensionArea *area = new DuiApplicationExtensionArea("com.nokia.dui.core.AppletInstallationSourceInterface/1.0");
    layoutPolicy->addItem(area);
    QObject::connect(area, SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface*)), controller, SLOT(setDuiAppletInventoryInterface(DuiApplicationExtensionInterface*)));
}

DuiAppletInventoryView::DuiAppletInventoryView(DuiAppletInventory *container) :
    DuiExtendingBackgroundView(* new DuiAppletInventoryViewPrivate, container)
{
    Q_D(DuiAppletInventoryView);
    d->init(container);
}

DuiAppletInventoryView::~DuiAppletInventoryView()
{
}

void DuiAppletInventoryView::updateData(const QList<const char *>& modifications)
{
    Q_D(const DuiAppletInventoryView);

    DuiExtendingBackgroundView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == DuiAppletInventoryModel::Widgets) {
            // Remove all widgets from the layout (do not destroy them)
            while (d->appletLayout->count() > 0) {
                d->appletLayout->removeAt(0);
            }

            // Add widgets from the model to the layout
            foreach(DuiWidget * widget, model()->widgets()) {
                d->appletLayoutPolicy->addItem(widget);
            }
        } else if (member == DuiAppletInventoryModel::CloseButtonVisible) {
            // Set the visibility of the button
            if (model()->closeButtonVisible()) {
                d->controller->sceneManager()->appearSceneWindow(d->closeButtonOverlay);
            } else {
                d->controller->sceneManager()->disappearSceneWindow(d->closeButtonOverlay);
            }
        }
    }
}


void DuiAppletInventoryView::connectContainerToInstallationSource(DuiContainer *container, DuiWidget *sourceWidget) const
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


DUI_REGISTER_VIEW_NEW(DuiAppletInventoryView, DuiAppletInventory)
