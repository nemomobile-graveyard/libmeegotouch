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
#include <limits.h>
#include "mextensionareaview.h"
#include "mextensionareaview_p.h"
#include "mextensionarea.h"
#include "mdatastore.h"
#include "mcontainer.h"

static const QString LAYOUT_INDEX = "layoutIndex";

MExtensionAreaViewPrivate::MExtensionAreaViewPrivate()
    : controller(0),
      layout(0),
      q_ptr(NULL)
{
}

MExtensionAreaViewPrivate::~MExtensionAreaViewPrivate()
{
}

void MExtensionAreaViewPrivate::setContainerEnabled(MContainer &container, const bool enabled) const
{
    container.setHeaderVisible(enabled);
    if (enabled) {
        container.setObjectName("");
    } else {
        container.setObjectName("MExtensionAreaInvisibleContainer");
    }
}

MContainer *MExtensionAreaViewPrivate::createWidgetContainer(QGraphicsWidget *widget) const
{
    Q_Q(const MExtensionAreaView);

    MContainer *container = new MContainer(controller);
    container->setCentralWidget(widget);
    connectContainerToWidget(container, widget);
    setContainerEnabled(*container, q->style()->containerMode());

    return container;
}

void MExtensionAreaViewPrivate::setupContainers(bool enabled)
{
    // Iterate through the items in the layout and add container widgets
    const int count = layout->count();
    for (int i = 0; i < count; ++i) {
        MContainer *container = dynamic_cast<MContainer *>(layout->itemAt(i));
        if (container) {
            setContainerEnabled(*container, enabled);
        }
    }
}

void MExtensionAreaViewPrivate::updateData()
{
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

        // Use a map for sorting the containers based on desired position in layout
        QMap<int, MContainer*> orderedLayoutItems;
        QMap<int, MContainer*> indexedLayoutItems;

        // Construct the correct order for the containers, create containers for new widgets
        foreach(QGraphicsWidget *widget, dsMap->keys()) {
            int currentLayoutIndex = findLayoutIndex(widget);
            int storedLayoutIndex  = getIntFromDataStore(widget, LAYOUT_INDEX);
            int order              = getIntFromDataStore(widget, "order");

            MContainer *container = currentLayoutIndex == -1 ?
                                    createWidgetContainer(widget) :
                                    dynamic_cast<MContainer*>(layout->itemAt(currentLayoutIndex));

            if(order != -1) {
                orderedLayoutItems.insert(order, container);
            } else if(storedLayoutIndex != -1) {
                indexedLayoutItems.insert(storedLayoutIndex, container);
            } else {
                indexedLayoutItems.insertMulti(INT_MAX, container);
            }
        }

        // Remove all from layout temporarily
        for(int i = layout->count() - 1; i >= 0; --i) {
            layout->removeAt(i);
        }

        // Put the containers back into the layout in correct order
        int i = 0;
        foreach(MContainer *container, orderedLayoutItems.values()) {
            if(container != NULL) {
                addToLayout(container, i++);
            }
        }
        foreach(MContainer *container, indexedLayoutItems.values()) {
            if(container != NULL) {
                addToLayout(container, i);
                dsMap->value(container->centralWidget())->createValue(LAYOUT_INDEX, i);
                i++;
            }
        }
    }
}

int MExtensionAreaViewPrivate::getIntFromDataStore(QGraphicsWidget *widget, QString key)
{
    Q_Q(MExtensionAreaView);

    MDataStore *store = q->model()->dataStores()->value(widget);

    if(store == NULL) return -1;

    bool found = false;
    int value = store->value(key).toInt(&found);
    if(!found) {
        value = -1;
    }

    return value;
}

int MExtensionAreaViewPrivate::findLayoutIndex(QGraphicsWidget *widget)
{
    const int count = layout->count();
    for (int i = 0; i < count; ++i) {
        MContainer *c = dynamic_cast<MContainer *>(layout->itemAt(i));
        if (widget == c->centralWidget()) {
            return i;
        }
    }
    return -1;
}

void MExtensionAreaViewPrivate::addToLayout(QGraphicsWidget *, int)
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
    if (d) {
        d->q_ptr = this;
    }
}

MExtensionAreaView::MExtensionAreaView(MExtensionAreaViewPrivate *dd, MExtensionArea *controller) :
    MWidgetView(controller),
    d_ptr(dd)
{
    Q_D(MExtensionAreaView);
    if (d) {
        d->q_ptr = this;
    }
}

MExtensionAreaView::~MExtensionAreaView()
{
    Q_D(MExtensionAreaView);
    delete d;
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

    MWidgetView::applyStyle();

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
