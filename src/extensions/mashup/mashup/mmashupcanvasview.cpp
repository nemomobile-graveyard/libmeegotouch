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

#include "mviewcreator.h"
#include <MPannableViewport>
#include <MButton>
#include <MLayout>
#include <MContainer>
#include <mflowlayoutpolicy.h>
#include <MModalSceneWindow>
#include <MApplication>
#include <MApplicationWindow>
#include <MSceneManager>
#include <QMetaProperty>
#include <QGraphicsLinearLayout>
#include <MDebug>
#include "mmashupcanvasview.h"
#include "mmashupcanvasview_p.h"
#include "mmashupcanvas.h"
#include "mappletinventory.h"
#include "mappletinstancemanager.h"
#include "mdatastore.h"
#include "mlocale.h"
#include <MApplicationExtensionArea>

MMashupCanvasViewPrivate::MMashupCanvasViewPrivate() :
    controller(NULL),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    appletInventoryWindow(NULL)
{
}

MMashupCanvasViewPrivate::~MMashupCanvasViewPrivate()
{
    // Destroying the window destroys its children (the viewport, layouts, button...)
    delete appletInventoryWindow;
}

void MMashupCanvasViewPrivate::init()
{
    MLayout *dl = new MLayout;
    dl->setAnimation(NULL);
    layout = dl;
    mainLayout->addItem(layout);
    layoutPolicy = new MFlowLayoutPolicy(dl);
    appletInventoryButton = new MButton;
    appletInventory = new MAppletInventory;
    appletInventoryWindow = new MModalSceneWindow;
    appletInventoryViewport = new MPannableViewport(appletInventoryWindow);

    // Put the applet inventory inside a viewport
    appletInventoryViewport->setWidget(appletInventory);

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active MWindow's scene.
    QSize sceneSize = MApplication::activeWindow()->visibleSceneSize();

    appletInventoryViewport->setMinimumSize(sceneSize);
    appletInventoryViewport->setMaximumSize(sceneSize);

    // Create a dialog layout
    QGraphicsLinearLayout *dialogLayout = new QGraphicsLinearLayout();
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addItem(appletInventoryViewport);
    appletInventoryWindow->setLayout(dialogLayout);
    appletInventoryWindow->setObjectName("MAppletInventoryWindow");

    // Create an applet inventory button
    appletInventoryButton->setObjectName("MAppletInventoryButton");
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

void MMashupCanvasViewPrivate::addToLayout(QGraphicsWidget *widget, int index)
{
    if (index >= 0) {
        layoutPolicy->insertItem(index, widget);
    } else {
        layoutPolicy->addItem(widget);
    }
}

void MMashupCanvasViewPrivate::connectContainerToWidget(MContainer *container, QGraphicsWidget *widget) const
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

    MExtensionAreaViewPrivate::connectContainerToWidget(container, widget);
}


MMashupCanvasView::MMashupCanvasView(MMashupCanvas *controller) :
    MExtensionAreaView(new MMashupCanvasViewPrivate, controller)
{
    Q_D(MMashupCanvasView);
    d->q_ptr = this;
    d->init();

    init(controller);
}

MMashupCanvasView::MMashupCanvasView(MMashupCanvasViewPrivate &dd, MMashupCanvas *controller) :
    MExtensionAreaView(&dd, controller)
{
    init(controller);
}

MMashupCanvasView::MMashupCanvasView(MMashupCanvasViewPrivate *dd, MMashupCanvas *controller) :
    MExtensionAreaView(dd, controller)
{
    init(controller);
}

MMashupCanvasView::~MMashupCanvasView()
{
}

void MMashupCanvasView::init(MMashupCanvas *controller)
{
    Q_D(MMashupCanvasView);
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
    // temporarily uses currently active MWindow's scene.
    connect(MApplication::activeWindow(), SIGNAL(orientationChanged(M::Orientation)), this, SLOT(orientationChanged(M::Orientation)));
}

void MMashupCanvasView::orientationChanged(M::Orientation)
{
    Q_D(MMashupCanvasView);

    // TODO: FIXME - this needs to have the scene specified,
    // temporarily uses currently active MWindow's scene.
    QSize sceneSize = MApplication::activeWindow()->visibleSceneSize();

    // Set the applet inventory viewport to the size of the display
    d->appletInventoryViewport->setMinimumSize(sceneSize);
    d->appletInventoryViewport->setMaximumSize(sceneSize);
}

void MMashupCanvasView::showAppletInventory()
{
    Q_D(MMashupCanvasView);

    d->controller->sceneManager()->appearSceneWindow(d->appletInventoryWindow);
    d->appletInventory->setEnabled(true);
}

void MMashupCanvasView::hideAppletInventory()
{
    Q_D(MMashupCanvasView);

    // Disable the applet inventory, so that during the disappear animation of
    // the dialog it's not possible to launch another applet
    d->appletInventory->setEnabled(false);

    d->controller->sceneManager()->disappearSceneWindow(d->appletInventoryWindow);
}

void MMashupCanvasView::setupModel()
{
    Q_D(MMashupCanvasView);

    MWidgetView::setupModel();

    d->updateLayout();
    d->appletInventory->setCategories(model()->categories());
}

void MMashupCanvasView::applyStyle()
{
    Q_D(MMashupCanvasView);
    d->setupContainers(style()->containerMode());
}

void MMashupCanvasView::updateData(const QList<const char *>& modifications)
{
    Q_D(MMashupCanvasView);

    bool appletLayoutUpdateNeeded = false;
    MWidgetView::updateData(modifications);
    foreach(const char * member, modifications) {
        if (member == MMashupCanvasModel::DataStores) {
            appletLayoutUpdateNeeded = true;
        } else if (member == MMashupCanvasModel::Categories) {
            d->appletInventory->setCategories(model()->categories());
        }
    }

    if (appletLayoutUpdateNeeded) {
        d->updateLayout();
    }
}

void MMashupCanvasView::setGeometry(const QRectF &rect)
{
    Q_D(MMashupCanvasView);

    // Set new geometry to the widget and to the layout
    MWidgetView::setGeometry(rect);

    // Geometry change might have affected the geometry of applets. Update all applets data stores.
    d->updateData();
}

M_REGISTER_VIEW_NEW(MMashupCanvasView, MMashupCanvas)
