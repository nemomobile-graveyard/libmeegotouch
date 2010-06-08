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

#include "mextensionarea.h"
#include "mextensionarea_p.h"

#include <QTapAndHoldGesture>

#include "mdatastore.h"
#include "mdebug.h"


MExtensionAreaPrivate::MExtensionAreaPrivate()
    : q_ptr(NULL)
{
}

MExtensionAreaPrivate::~MExtensionAreaPrivate()
{
}

void MExtensionAreaPrivate::addWidget(QGraphicsWidget *widget, MDataStore &store)
{
    Q_Q(MExtensionArea);

    if (!dataStores.contains(widget)) {
        // Add data store to data stores map
        dataStores[widget] = &store;

        // Let the view know about the data store modification
        q->model()->dataStoresModified();
    } else {
        // Widget is already added to the extension area. Bail out.
        mWarning("MExtensionArea") << "MExtensionArea::addWidget() - Widget was already added to extension area.";
    }
}

void MExtensionAreaPrivate::removeWidget(QGraphicsWidget *widget)
{
    Q_Q(MExtensionArea);

    if (dataStores.contains(widget)) {
        // Remove data store from the data stores map
        dataStores.remove(widget);
        q->model()->dataStoresModified();
    }
}

void MExtensionAreaPrivate::init()
{
    Q_Q(MExtensionArea);

    // Put the data stores into the model
    q->model()->setDataStores(&dataStores);
}

void MExtensionAreaPrivate::gestureEvent(QGestureEvent *event)
{
    foreach(QGesture* state, event->gestures()) {
        if (Qt::TapAndHoldGesture == state->gestureType()) {
            QTapAndHoldGesture* tapAndHoldState = static_cast<QTapAndHoldGesture *>(state);
            tapAndHoldGesture(event,tapAndHoldState);
        }
    }
}

void MExtensionAreaPrivate::tapAndHoldGesture(QGestureEvent *event, QTapAndHoldGesture *state)
{
    Q_UNUSED(event);
    Q_UNUSED(state);
}

MExtensionArea::MExtensionArea(MExtensionAreaPrivate *dd, MWidgetModel *model, QGraphicsItem *parent)
    : MWidgetController(model, parent),
      d_ptr(dd)
{
    // Initialize the private implementation
    Q_D(MExtensionArea);
    if (d) {
        d->q_ptr = this;
        d->init();
    }
}

MExtensionArea::MExtensionArea(QGraphicsItem *parent) :
    MWidgetController(new MExtensionAreaModel, parent),
    d_ptr(new MExtensionAreaPrivate())
{
    // Initialize the private implementation
    Q_D(MExtensionArea);
    d->q_ptr = this;
    d->init();
}

MExtensionArea::~MExtensionArea()
{
    Q_D(MExtensionArea);
    delete d;
}

void MExtensionArea::addWidget(QGraphicsWidget *widget, MDataStore &store)
{
    Q_D(MExtensionArea);

    d->addWidget(widget, store);
}

void MExtensionArea::removeWidget(QGraphicsWidget *widget)
{
    Q_D(MExtensionArea);

    d->removeWidget(widget);
}
