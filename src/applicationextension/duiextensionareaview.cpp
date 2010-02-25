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

#include <QGraphicsLayout>
#include "duiextensionareaview.h"
#include "duiextensionareaview_p.h"
#include "duiextensionarea.h"
#include "duidatastore.h"
#include "duicontainer.h"

DuiExtensionAreaViewPrivate::DuiExtensionAreaViewPrivate()
{
}

DuiExtensionAreaViewPrivate::~DuiExtensionAreaViewPrivate()
{
}

void DuiExtensionAreaViewPrivate::setContainerEnabled(DuiContainer *container, bool enabled)
{
    container->setHeaderVisible(enabled);
    if (enabled) {
        container->setObjectName("");
    } else {
        container->setObjectName("DuiExtensionAreaInvisibleContainer");
    }
}

DuiContainer *DuiExtensionAreaViewPrivate::createWidgetContainer(DuiWidget *widget) const
{
    DuiContainer *container = new DuiContainer(controller);
    container->setCentralWidget(widget);
    connectContainerToWidget(container, widget);
    return container;
}

void DuiExtensionAreaViewPrivate::setupContainers(bool enabled)
{
    // Iterate through the items in the layout and add container widgets
    const int count = layout->count();
    for (int i = 0; i < count; ++i) {
        DuiContainer *container = dynamic_cast<DuiContainer *>(layout->itemAt(i));
        setContainerEnabled(container, enabled);
    }
}

void DuiExtensionAreaViewPrivate::updateData()
{
    Q_Q(DuiExtensionAreaView);

    DataStoreMap *dsMap = q->model()->dataStores();
    if (dsMap != NULL) {
        // Iterate through the items in the layout and write their new geometries to data store.
        const int count = layout->count();
        for (int i = 0; i < count; ++i) {
            QGraphicsLayoutItem *layoutItem = layout->itemAt(i);
            DuiContainer *container = dynamic_cast<DuiContainer *>(layoutItem);
            DuiWidget    *centralWidget = container->centralWidget();

            // Change the data in the store if item is still valid and geometry has changed.
            if (centralWidget != NULL && dsMap->contains(centralWidget) &&
                    (*dsMap)[centralWidget]->value("layoutIndex") != i) {
                (*dsMap)[centralWidget]->createValue("layoutIndex", i);
            }
        }
    }
}

void DuiExtensionAreaViewPrivate::updateLayout()
{
    Q_Q(DuiExtensionAreaView);

    DataStoreMap *dsMap = q->model()->dataStores();
    if (dsMap != NULL) {
        // Iterate through the items in the layout and remove items
        // that don't exist in the data store
        for (int i = layout->count() - 1; i >= 0; --i) {
            DuiContainer *container = dynamic_cast<DuiContainer *>(layout->itemAt(i));
            DuiWidget *centralWidget = container->centralWidget();

            if (container != NULL && !dsMap->contains(centralWidget)) {
                // Remove widget from layout
                layout->removeAt(i);
                centralWidget->setParentItem(controller);
                // Delete the container of the widget,
                // we need to delete it here as this view owns it
                delete container;
            }
        }

        // Iterate through the items in the data store and add items that don't exist in the layout
        foreach(DuiWidget * widget, dsMap->keys()) {
            bool alreadyInLayout = false;
            const int count = layout->count();
            for (int i = 0; i < count; ++i) {
                DuiContainer *theContainer = dynamic_cast<DuiContainer *>(layout->itemAt(i));
                if (widget == theContainer->centralWidget()) {
                    // Widget found from the layout, don't add again
                    alreadyInLayout = true;
                    break;
                }
            }

            if (!alreadyInLayout) {
                // Widget not found in the layout so add it
                DuiDataStore *store = dsMap->value(widget);

                // Create a container for the widget if container mode is enabled for the canvas
                DuiContainer *container = createWidgetContainer(widget);
                setContainerEnabled(container, q->style()->containerMode());

                int layoutIndex = 0;

                // Add widget to the layout policy
                if (store->allKeys().contains("layoutIndex") &&
                        store->value("layoutIndex").type() == QVariant::Int) {
                    layoutIndex = store->value("layoutIndex").toInt();
                    addToLayout(container, layoutIndex);
                } else {
                    layoutIndex = layout->count();
                    addToLayout(container);
                }

                // Write the layout data to the permanent store
                store->createValue("layoutIndex", layoutIndex);
            }
        }
    }
}

void DuiExtensionAreaViewPrivate::addToLayout(DuiWidget *, int)
{
}

void DuiExtensionAreaViewPrivate::connectContainerToWidget(DuiContainer *container, DuiWidget *widget) const
{
    // Copying actions from the widget to the container.
    container->addActions(widget->actions());
}

DuiExtensionAreaView::DuiExtensionAreaView(DuiExtensionArea *controller) :
    DuiWidgetView(* new DuiExtensionAreaViewPrivate, controller)
{
}

DuiExtensionAreaView::DuiExtensionAreaView(DuiExtensionAreaViewPrivate &dd, DuiExtensionArea *controller) :
    DuiWidgetView(dd, controller)
{
}

DuiExtensionAreaView::~DuiExtensionAreaView()
{
}

void DuiExtensionAreaView::setupModel()
{
    Q_D(DuiExtensionAreaView);

    DuiWidgetView::setupModel();

    d->updateLayout();
}

void DuiExtensionAreaView::applyStyle()
{
    Q_D(DuiExtensionAreaView);
    d->setupContainers(style()->containerMode());
}

void DuiExtensionAreaView::setGeometry(const QRectF &rect)
{
    Q_D(DuiExtensionAreaView);

    // Set new geometry to the widget and to the layout
    DuiWidgetView::setGeometry(rect);

    // Geometry change might have affected the geometry of widgets. Update all widgets data stores.
    d->updateData();
}

void DuiExtensionAreaView::updateData(const QList<const char *>& modifications)
{
    Q_D(DuiExtensionAreaView);

    bool layoutUpdateNeeded = false;
    DuiWidgetView::updateData(modifications);
    foreach(const char * member, modifications) {
        if (member == DuiExtensionAreaModel::DataStores) {
            layoutUpdateNeeded = true;
        }
    }

    if (layoutUpdateNeeded) {
        d->updateLayout();
    }
}
