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

#include <QGraphicsLayout>
#include "mextensionareaview.h"
#include "mextensionareaview_p.h"
#include "mextensionarea.h"
#include "mdatastore.h"
#include "mcontainer.h"

MExtensionAreaViewPrivate::MExtensionAreaViewPrivate()
    : controller(0),
      layout(0),
      q_ptr(NULL)
{
}

MExtensionAreaViewPrivate::~MExtensionAreaViewPrivate()
{
}

void MExtensionAreaViewPrivate::setContainerEnabled(MContainer *container, bool enabled)
{
    container->setHeaderVisible(enabled);
    if (enabled) {
        container->setObjectName("");
    } else {
        container->setObjectName("MExtensionAreaInvisibleContainer");
    }
}

MContainer *MExtensionAreaViewPrivate::createWidgetContainer(QGraphicsWidget *widget) const
{
    MContainer *container = new MContainer(controller);
    container->setCentralWidget(widget);
    connectContainerToWidget(container, widget);
    return container;
}

void MExtensionAreaViewPrivate::setupContainers(bool enabled)
{
    // Iterate through the items in the layout and add container widgets
    const int count = layout->count();
    for (int i = 0; i < count; ++i) {
        MContainer *container = dynamic_cast<MContainer *>(layout->itemAt(i));
        setContainerEnabled(container, enabled);
    }
}

void MExtensionAreaViewPrivate::updateData()
{
    Q_Q(MExtensionAreaView);

    DataStoreMap *dsMap = q->model()->dataStores();
    if (dsMap != NULL) {
        // Iterate through the items in the layout and write their new geometries to data store.
        const int count = layout->count();
        for (int i = 0; i < count; ++i) {
            QGraphicsLayoutItem *layoutItem = layout->itemAt(i);
            MContainer *container = dynamic_cast<MContainer *>(layoutItem);
            QGraphicsWidget *centralWidget = container->centralWidget();

            // Change the data in the store if item is still valid and geometry has changed.
            if (centralWidget != NULL && dsMap->contains(centralWidget) &&
                    (*dsMap)[centralWidget]->value("layoutIndex") != i) {
                (*dsMap)[centralWidget]->createValue("layoutIndex", i);
            }
        }
    }
}

void MExtensionAreaViewPrivate::updateLayout()
{
    Q_Q(MExtensionAreaView);

    DataStoreMap *dsMap = q->model()->dataStores();
    if (dsMap != NULL) {
        // Iterate through the items in the layout and remove items
        // that don't exist in the data store
        for (int i = layout->count() - 1; i >= 0; --i) {
            MContainer *container = dynamic_cast<MContainer *>(layout->itemAt(i));
            QGraphicsWidget *centralWidget = container->centralWidget();

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
        foreach(QGraphicsWidget * widget, dsMap->keys()) {
            bool alreadyInLayout = false;
            const int count = layout->count();
            for (int i = 0; i < count; ++i) {
                MContainer *theContainer = dynamic_cast<MContainer *>(layout->itemAt(i));
                if (widget == theContainer->centralWidget()) {
                    // Widget found from the layout, don't add again
                    alreadyInLayout = true;
                    break;
                }
            }

            if (!alreadyInLayout) {
                // Widget not found in the layout so add it
                MDataStore *store = dsMap->value(widget);

                // Create a container for the widget if container mode is enabled for the canvas
                MContainer *container = createWidgetContainer(widget);
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

void MExtensionAreaViewPrivate::addToLayout(MWidget *, int)
{
}

void MExtensionAreaViewPrivate::connectContainerToWidget(MContainer *container, QGraphicsWidget *widget) const
{
    // Copying actions from the widget to the container.
    container->addActions(widget->actions());
}

MExtensionAreaView::MExtensionAreaView(MExtensionArea *controller) :
    MWidgetView(controller),
    d_ptr(new MExtensionAreaViewPrivate())
{
}

MExtensionAreaView::MExtensionAreaView(MExtensionAreaViewPrivate &dd, MExtensionArea *controller) :
    MWidgetView(controller),
    d_ptr(& dd)
{
    Q_D(MExtensionAreaView);
    if (d)
        d->q_ptr = this;
}

MExtensionAreaView::~MExtensionAreaView()
{
}

void MExtensionAreaView::setupModel()
{
    Q_D(MExtensionAreaView);

    MWidgetView::setupModel();

    d->updateLayout();
}

void MExtensionAreaView::applyStyle()
{
    Q_D(MExtensionAreaView);
    d->setupContainers(style()->containerMode());
}

void MExtensionAreaView::setGeometry(const QRectF &rect)
{
    Q_D(MExtensionAreaView);

    // Set new geometry to the widget and to the layout
    MWidgetView::setGeometry(rect);

    // Geometry change might have affected the geometry of widgets. Update all widgets data stores.
    d->updateData();
}

void MExtensionAreaView::updateData(const QList<const char *>& modifications)
{
    Q_D(MExtensionAreaView);

    bool layoutUpdateNeeded = false;
    MWidgetView::updateData(modifications);
    foreach(const char * member, modifications) {
        if (member == MExtensionAreaModel::DataStores) {
            layoutUpdateNeeded = true;
        }
    }

    if (layoutUpdateNeeded) {
        d->updateLayout();
    }
}
