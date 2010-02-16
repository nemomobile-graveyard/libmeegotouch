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
    appletLayout(new DuiLayout),
    layoutPolicy(new DuiFlowLayoutPolicy(appletLayout)),
    appletInventoryButton(new DuiButton),
    appletInventory(new DuiAppletInventory),
    appletInventoryWindow(new DuiModalSceneWindow),
    appletInventoryViewport(new DuiPannableViewport(appletInventoryWindow))
{
    // Set the layout
    appletLayout->setAnimation(NULL);
    mainLayout->addItem(appletLayout);

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

DuiMashupCanvasViewPrivate::~DuiMashupCanvasViewPrivate()
{
    // Destroying the window destroys it's children (the viewport, layouts, button...)
    delete appletInventoryWindow;
}

void DuiMashupCanvasViewPrivate::updateAppletData()
{
    Q_Q(DuiMashupCanvasView);

    if (q->model()->dataStores() != NULL) {
        // Iterate through the items in the layout and write their new geometries to data store.
        const int count = layoutPolicy->count();
        for (int i = 0; i < count; ++i) {
          QGraphicsLayoutItem* layoutItem = layoutPolicy->itemAt(i);

            DuiContainer *container = dynamic_cast<DuiContainer *>(layoutItem);
            DuiWidget* centralWidget = container->centralWidget();
            if (centralWidget != NULL && 
                q->model()->dataStores()->contains(centralWidget) && 
                (*q->model()->dataStores())[centralWidget]->value("layoutIndex") != layoutPolicy->indexOf(layoutItem)) {
                // Change the data in the store if item is still valid and geometry has changed.
                (*q->model()->dataStores())[centralWidget]->createValue("layoutIndex", layoutPolicy->indexOf(layoutItem));
            }
        }
    }
}

void DuiMashupCanvasViewPrivate::updateAppletLayout()
{
    Q_Q(DuiMashupCanvasView);

    if (q->model()->dataStores() != NULL) {
        // Iterate through the items in the layout and remove items 
        // that don't exist in the data store
        for (int i = appletLayout->count() - 1; i >= 0; --i) {
          DuiContainer *container = dynamic_cast<DuiContainer *>(appletLayout->itemAt(i));
            DuiWidget* centralWidget = container->centralWidget();
            if (container != NULL && 
                !q->model()->dataStores()->contains(centralWidget)) {
                // Remove widget from layout
                appletLayout->removeAt(i);
                centralWidget->setParentItem(controller);
                // Delete the container of the applet, 
                // we need to delete it here as this view owns it
                delete container;
            }
        }

        // Iterate through the items in the data store and add items that don't exist in the layout
        foreach (DuiWidget *widget, q->model()->dataStores()->keys()) {
            bool alreadyInLayout = false;
            const int count = appletLayout->count();
            for (int i = 0; i < count; ++i) {
              DuiContainer *theContainer = dynamic_cast<DuiContainer *>(appletLayout->itemAt(i));
                if (widget == theContainer->centralWidget()) {
                    // Widget found from the layout, don't add again
                    alreadyInLayout = true;
                    break;
                }
            }

            if (!alreadyInLayout) {
                // Widget not found in the layout so add it
                DuiDataStore *store = q->model()->dataStores()->value(widget);

                // Create a container for the widget if container mode is enabled for the canvas
                DuiContainer *container = createWidgetContainer(widget);
                setContainerEnabled(container, q->style()->containerMode());

                // Add widget to the layout policy
                if (store->allKeys().contains("layoutIndex") && 
                    store->value("layoutIndex").type() == QVariant::Int) {
                    layoutPolicy->insertItem(store->value("layoutIndex").toInt(), container);
                } else {
                    layoutPolicy->addItem(container);
                }

                // Write the layout data to the permanent store
                store->createValue("layoutIndex", appletLayout->indexOf(container));
            }
        }
    }
}

void DuiMashupCanvasViewPrivate::setContainerEnabled(DuiContainer* container, bool enabled)
{
    container->setHeaderVisible(enabled);
    if (enabled) {
        container->setObjectName("");
    } else {
        container->setObjectName("DuiMashupCanvasInvisibleContainer");
    }        
}

DuiContainer* DuiMashupCanvasViewPrivate::createWidgetContainer(DuiWidget* widget) const
{
    DuiContainer *container = new DuiContainer(controller);
    container->setCentralWidget(widget);
    connectContainerToApplet(container, widget);    
    return container;
}

void DuiMashupCanvasViewPrivate::setupContainers(bool enabled)
{
    // Iterate through the items in the layout and add container widgets
    const int count = layoutPolicy->count();
    for (int i = 0; i < count; ++i) {
        DuiContainer *container = dynamic_cast<DuiContainer*>(layoutPolicy->itemAt(i));
        setContainerEnabled(container, enabled);
    }
}

DuiMashupCanvasView::DuiMashupCanvasView(DuiMashupCanvas *controller) :
    DuiWidgetView(* new DuiMashupCanvasViewPrivate, controller)
{
    Q_D(DuiMashupCanvasView);
    d->q_ptr = this;
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

    d->updateAppletLayout();
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
        d->updateAppletLayout();
    }
}

void DuiMashupCanvasView::setGeometry(const QRectF &rect)
{
    Q_D(DuiMashupCanvasView);

    // Set new geometry to the widget and to the layout
    DuiWidgetView::setGeometry(rect);

    // Geometry change might have affected the geometry of applets. Update all applets data stores.
    d->updateAppletData();
}

void DuiMashupCanvasViewPrivate::connectContainerToApplet(DuiContainer *container, DuiWidget *applet) const
{
    const QMetaObject *mob = applet->metaObject();

    // use applet properties to fill header of the container
    int iconProperty = mob->indexOfProperty("appletIcon");
    int titleProperty = mob->indexOfProperty("appletTitle");
    int textProperty = mob->indexOfProperty("appletText");

    if (iconProperty != -1) {
        container->setIconID((mob->property(iconProperty).read(applet)).toString());
    }

    if (titleProperty != -1) {
        container->setTitle((mob->property(titleProperty).read(applet)).toString());
        container->setHeaderVisible(true);
    }

    if (textProperty != -1) {
        container->setText((mob->property(textProperty).read(applet)).toString());
    }

    // connect signals from applet to the container
    if (mob->indexOfSignal("appletIconChanged(QString)") != -1) {
        QObject::connect(applet, SIGNAL(appletIconChanged(QString)), container, SLOT(setIconID(QString)));
    }

    if (mob->indexOfSignal("appletTitleChanged(QString)") != -1) {
        QObject::connect(applet, SIGNAL(appletTitleChanged(QString)), container, SLOT(setTitle(QString)));
    }

    if (mob->indexOfSignal("appletTextChanged(QString)") != -1) {
        QObject::connect(applet, SIGNAL(appletTextChanged(QString)), container, SLOT(setText(QString)));
    }

    // connect deprecated signals from applet to the container
    // TODO remove these after deprecation
    if (mob->indexOfSignal("setAppletIcon(QString)") != -1) {
        QObject::connect(applet, SIGNAL(setAppletIcon(QString)), container, SLOT(setIconID(QString)));
    }
    if (mob->indexOfSignal("setAppletTitle(QString)") != -1) {
        QObject::connect(applet, SIGNAL(setAppletTitle(QString)), container, SLOT(setTitle(QString)));
    }
    if (mob->indexOfSignal("setAppletText(QString)") != -1) {
        QObject::connect(applet, SIGNAL(setAppletText(QString)), container, SLOT(setText(QString)));
    }

    // Copying actions from the applet to the container.
    container->addActions(applet->actions());
}

DUI_REGISTER_VIEW_NEW(DuiMashupCanvasView, DuiMashupCanvas)
