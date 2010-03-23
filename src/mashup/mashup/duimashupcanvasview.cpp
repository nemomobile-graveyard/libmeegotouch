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

#include "duiviewcreator.h"
#include <DuiPannableViewport>
#include <DuiButton>
#include <DuiLayout>
#include <DuiContainer>
#include <duiflowlayoutpolicy.h>
#include <DuiModalSceneWindow>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiSceneManager>
#include <QMetaProperty>
#include <QGraphicsLinearLayout>
#include <DuiDebug>
#include "duimashupcanvasview.h"
#include "duimashupcanvasview_p.h"
#include "duimashupcanvas.h"
#include "duiappletinventory.h"
#include "duiappletinstancemanager.h"
#include "duidatastore.h"
#include "duilocale.h"

DuiMashupCanvasViewPrivate::DuiMashupCanvasViewPrivate() :
    controller(NULL),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    appletInventoryWindow(NULL)
{
}

DuiMashupCanvasViewPrivate::~DuiMashupCanvasViewPrivate()
{
    // Destroying the window destroys it's children (the viewport, layouts, button...)
    delete appletInventoryWindow;
}

void DuiMashupCanvasViewPrivate::init()
{
    DuiLayout *dl = new DuiLayout;
    dl->setAnimation(NULL);
    layout = dl;
    mainLayout->addItem(layout);
    layoutPolicy = new DuiFlowLayoutPolicy(dl);
    appletInventoryButton = new DuiButton;
    appletInventory = new DuiAppletInventory;
    appletInventoryWindow = new DuiModalSceneWindow;
    appletInventoryViewport = new DuiPannableViewport(appletInventoryWindow);

    // Put the applet inventory inside a viewport
    appletInventoryViewport->setWidget(appletInventory);

    // Set the applet installation source libraries for the applet inventory.
    // TODO: this is currently a fake installation source to provide some demo functionality.
    // Once the real installation sources are known, they will be added here.
    appletInventory->setInstallationSources(QStringList("libfakeinstallationsource.so"));

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active DuiWindow's scene.
    QSize sceneSize = DuiApplication::activeWindow()->visibleSceneSize();

    appletInventoryViewport->setMinimumSize(sceneSize);
    appletInventoryViewport->setMaximumSize(sceneSize);

    // Create a dialog layout
    QGraphicsLinearLayout *dialogLayout = new QGraphicsLinearLayout();
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addItem(appletInventoryViewport);
    appletInventoryWindow->setLayout(dialogLayout);
    appletInventoryWindow->setObjectName("DuiAppletInventoryWindow");

    // Create an applet inventory button
    appletInventoryButton->setObjectName("DuiAppletInventoryButton");
    //~ uispec-document ??? FIXME
    //% "Applet Library"
    appletInventoryButton->setText(qtTrId("qtn_appl_inventory"));

    // Add the applet inventory button to a horizontal layout for centering
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    l->addStretch();
    l->addItem(appletInventoryButton);
    l->addStretch();

    // Add the horizontal layout to the main layout
    mainLayout->addItem(l);
}

void DuiMashupCanvasViewPrivate::addToLayout(DuiWidget *widget, int index)
{
    if (index >= 0) {
        layoutPolicy->insertItem(index, widget);
    } else {
        layoutPolicy->addItem(widget);
    }
}

void DuiMashupCanvasViewPrivate::connectContainerToWidget(DuiContainer *container, DuiWidget *widget) const
{
    const QMetaObject *mob = widget->metaObject();

    // use widget properties to fill header of the container
    int iconProperty  = mob->indexOfProperty("appletIcon");
    int titleProperty = mob->indexOfProperty("appletTitle");
    int textProperty  = mob->indexOfProperty("appletText");

    if (iconProperty != -1) {
        container->setIconID((mob->property(iconProperty).read(widget)).toString());
    }

    if (titleProperty != -1) {
        container->setTitle((mob->property(titleProperty).read(widget)).toString());
        container->setHeaderVisible(true);
    }

    if (textProperty != -1) {
        container->setText((mob->property(textProperty).read(widget)).toString());
    }

    // connect signals from widget to the container
    if (mob->indexOfSignal("appletIconChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(appletIconChanged(QString)), container, SLOT(setIconID(QString)));
    }

    if (mob->indexOfSignal("appletTitleChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(appletTitleChanged(QString)), container, SLOT(setTitle(QString)));
    }

    if (mob->indexOfSignal("appletTextChanged(QString)") != -1) {
        QObject::connect(widget, SIGNAL(appletTextChanged(QString)), container, SLOT(setText(QString)));
    }

    DuiExtensionAreaViewPrivate::connectContainerToWidget(container, widget);
}


DuiMashupCanvasView::DuiMashupCanvasView(DuiMashupCanvas *controller) :
    DuiWidgetView(* new DuiMashupCanvasViewPrivate, controller)
{
    Q_D(DuiMashupCanvasView);
    d->q_ptr = this;
    d->init();

    init(controller);
}

DuiMashupCanvasView::DuiMashupCanvasView(DuiMashupCanvasViewPrivate &dd, DuiMashupCanvas *controller) :
    DuiWidgetView(dd, controller)
{
    init(controller);
}

DuiMashupCanvasView::~DuiMashupCanvasView()
{
}

void DuiMashupCanvasView::init(DuiMashupCanvas *controller)
{
    Q_D(DuiMashupCanvasView);
    d->controller = controller;
    d->controller->setLayout(d->mainLayout);

    // Set up the applet inventory
    d->appletInventory->setMashupCanvas(*controller);
    connect(d->appletInventory, SIGNAL(hideAppletInventory()), this, SLOT(hideAppletInventory()));
    connect(d->appletInventory, SIGNAL(appletUninstalled(QString)), controller->appletInstanceManager(), SLOT(appletUninstalled(QString)));

    // Connect the applet inventory button clicking signal
    QObject::connect(d->appletInventoryButton, SIGNAL(clicked()), this, SLOT(showAppletInventory()));

    // Get informed about orientation changes
    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active DuiWindow's scene.
    connect(DuiApplication::activeWindow(), SIGNAL(orientationChanged(Dui::Orientation)), this, SLOT(orientationChanged(Dui::Orientation)));
}

void DuiMashupCanvasView::orientationChanged(const Dui::Orientation &)
{
    Q_D(DuiMashupCanvasView);

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active DuiWindow's scene.
    QSize sceneSize = DuiApplication::activeWindow()->visibleSceneSize();

    // Set the applet inventory viewport to the size of the display
    d->appletInventoryViewport->setMinimumSize(sceneSize);
    d->appletInventoryViewport->setMaximumSize(sceneSize);
}

void DuiMashupCanvasView::showAppletInventory()
{
    Q_D(DuiMashupCanvasView);

    d->controller->sceneManager()->showWindow(d->appletInventoryWindow);
    d->appletInventory->setEnabled(true);
}

void DuiMashupCanvasView::hideAppletInventory()
{
    Q_D(DuiMashupCanvasView);

    // Disable the applet inventory, so that during the disappear animation of
    // the dialog it's not possible to launch another applet
    d->appletInventory->setEnabled(false);

    d->controller->sceneManager()->hideWindow(d->appletInventoryWindow);
}

void DuiMashupCanvasView::setupModel()
{
    Q_D(DuiMashupCanvasView);

    DuiWidgetView::setupModel();

    d->updateLayout();
    d->appletInventory->setCategories(model()->categories());
}

void DuiMashupCanvasView::applyStyle()
{
    Q_D(DuiMashupCanvasView);
    d->setupContainers(style()->containerMode());
}

void DuiMashupCanvasView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiMashupCanvasView);

    bool appletLayoutUpdateNeeded = false;
    DuiWidgetView::updateData(modifications);
    foreach(const char * member, modifications) {
        if (member == DuiMashupCanvasModel::DataStores) {
            appletLayoutUpdateNeeded = true;
        } else if (member == DuiMashupCanvasModel::Categories) {
            d->appletInventory->setCategories(model()->categories());
        }
    }

    if (appletLayoutUpdateNeeded) {
        d->updateLayout();
    }
}

void DuiMashupCanvasView::setGeometry(const QRectF &rect)
{
    Q_D(DuiMashupCanvasView);

    // Set new geometry to the widget and to the layout
    DuiWidgetView::setGeometry(rect);

    // Geometry change might have affected the geometry of applets. Update all applets data stores.
    d->updateData();
}

DUI_REGISTER_VIEW_NEW(DuiMashupCanvasView, DuiMashupCanvas)
