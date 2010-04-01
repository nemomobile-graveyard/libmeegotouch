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

#include "duiextensionarea.h"
#include "duiextensionarea_p.h"

#include <QTapAndHoldGesture>

#include "duidatastore.h"
#include "duidebug.h"


DuiExtensionAreaPrivate::DuiExtensionAreaPrivate()
    : q_ptr(NULL)
{
}

DuiExtensionAreaPrivate::~DuiExtensionAreaPrivate()
{
}

void DuiExtensionAreaPrivate::addWidget(DuiWidget *widget, DuiDataStore &store)
{
    Q_Q(DuiExtensionArea);

    if (!dataStores.contains(widget)) {
        // Add data store to data stores map
        dataStores[widget] = &store;

        // Let the view know about the data store modification
        q->model()->dataStoresModified();
    } else {
        // Widget is already added to the mashup canvas. Bail out.
        duiWarning("DuiExtensionArea") << "DuiExtensionArea::addWidget() - Widget was already added to extension area.";
    }
}

void DuiExtensionAreaPrivate::removeWidget(DuiWidget *widget)
{
    Q_Q(DuiExtensionArea);

    if (dataStores.contains(widget)) {
        // Remove data store from the data stores map
        dataStores.remove(widget);
        q->model()->dataStoresModified();
    }
}

void DuiExtensionAreaPrivate::init()
{
    Q_Q(DuiExtensionArea);

    // Put the data stores into the model
    q->model()->setDataStores(&dataStores);
}

void DuiExtensionAreaPrivate::gestureEvent(QGestureEvent *event)
{
    foreach(QGesture* state, event->gestures()) {
        if (Qt::TapAndHoldGesture == state->gestureType()) {
            QTapAndHoldGesture* tapAndHoldState = static_cast<QTapAndHoldGesture *>(state);
            tapAndHoldGesture(event,tapAndHoldState);
        }
    }
}

void DuiExtensionAreaPrivate::tapAndHoldGesture(QGestureEvent *event, QTapAndHoldGesture *state)
{
    Q_UNUSED(event);
    Q_UNUSED(state);
}

DuiExtensionArea::DuiExtensionArea(DuiExtensionAreaPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent)
    : DuiWidgetController(model, parent),
      d_ptr(dd)
{
    // Initialize the private implementation
    Q_D(DuiExtensionArea);
    if (d)
        d->q_ptr = this;
    d->init();
}

DuiExtensionArea::DuiExtensionArea(QGraphicsItem *parent) :
    DuiWidgetController(new DuiExtensionAreaModel, parent),
    d_ptr(new DuiExtensionAreaPrivate())
{
    // Initialize the private implementation
    Q_D(DuiExtensionArea);
    d->q_ptr = this;
    d->init();
}

DuiExtensionArea::~DuiExtensionArea()
{
}

void DuiExtensionArea::addWidget(DuiWidget *widget, DuiDataStore &store)
{
    Q_D(DuiExtensionArea);

    d->addWidget(widget, store);
}

void DuiExtensionArea::removeWidget(DuiWidget *widget)
{
    Q_D(DuiExtensionArea);

    d->removeWidget(widget);
}
