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

#include "duimashupcanvas.h"
#include "duimashupcanvas_p.h"
#include "duicomponentdata.h"
#include "duiappletinstancemanager.h"
#include "duidatastore.h"
#include "duimashupcanvasstyle.h"
#include "duilocale.h"
#include <DuiAction>
#include <DuiDebug>

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET_NO_CREATE(DuiMashupCanvas)

///////////////////
// PRIVATE CLASS //
///////////////////

QSet<QString> DuiMashupCanvasPrivate::allMashupCanvasIdentifiers;

DuiMashupCanvasPrivate::DuiMashupCanvasPrivate() : instanceManager(NULL)
{
}

DuiMashupCanvasPrivate::~DuiMashupCanvasPrivate()
{
    delete instanceManager;
}

void DuiMashupCanvasPrivate::init(const QString &identifier)
{
    Q_Q(DuiMashupCanvas);

    this->identifier = provideUniqueIdentifier(identifier);
    this->serviceAddress = DuiComponentData::serviceName() + "/DuiAppletInstanceManager/" + this->identifier;
    instanceManager = new DuiAppletInstanceManager(this->identifier);

    // Connect applet instance manager signals and restore applet instances
    q->connect(instanceManager, SIGNAL(appletInstantiated(DuiWidget *, DuiDataStore &)), SLOT(addWidget(DuiWidget *, DuiDataStore &)));
    q->connect(instanceManager, SIGNAL(appletRemoved(DuiWidget *)), SLOT(removeWidget(DuiWidget *)));
    instanceManager->restoreApplets();

    // Put the data stores into the model
    q->model()->setDataStores(&dataStores);
}

QString DuiMashupCanvasPrivate::provideUniqueIdentifier(const QString &identifier)
{
    QString result = identifier;
    if (identifier.isEmpty() || allMashupCanvasIdentifiers.contains(identifier)) {
        uint number = 1;
        QString tmpIdentifier = identifier + '_' + QString::number(number);
        while (allMashupCanvasIdentifiers.contains(tmpIdentifier)) {
            ++number;
            tmpIdentifier = identifier + '_' + QString::number(number);
        }

        result = tmpIdentifier;
    }

    allMashupCanvasIdentifiers.insert(result);
    return result;
}

void DuiMashupCanvasPrivate::addWidget(DuiWidget *widget, DuiDataStore &store)
{
    Q_Q(DuiMashupCanvas);

    // TODO: when the current ABI freeze ends:
    // - make DuiMashupCanvas inherit DuiExtensionArea
    // - remove dataStore handling in this function and call DuiExtensionAreaPrivate's addWidget instead
    //   (leave the DuiAction things here)
    if (!dataStores.contains(widget)) {
        // Add the remove action into the object menu of the applet widget
        //: Object menu command. Removes e.g. applet from canvas.
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.pdf
        //% "Remove Applet"
        DuiAction *action = new DuiAction(qtTrId("qtn_comm_removewidget"), q);
        instanceManager->connect(action, SIGNAL(triggered(bool)), SLOT(removeActionTriggered(bool)));
        widget->addAction(action);

        // Add data store to data stores map
        dataStores[widget] = &store;

        // Let the view know about the data store modification
        q->model()->dataStoresModified();
    } else {
        // Widget is already added to the mashup canvas. Bail out.
        duiWarning("DuiMashupCanvas") << "DuiMashupCanvas::addWidget() - Widget was already added to mashup canvas.";
    }
}

void DuiMashupCanvasPrivate::removeWidget(DuiWidget *widget)
{
    Q_Q(DuiMashupCanvas);

    // TODO: when the current ABI freeze ends:
    // - make DuiMashupCanvas inherit DuiExtensionArea
    // - remove this function (removeWidget in the base class works then)
    if (dataStores.contains(widget)) {
        // Remove data store from the data stores map
        dataStores.remove(widget);
        q->model()->dataStoresModified();
    }
}


//////////////////
// PUBLIC CLASS //
//////////////////
DuiMashupCanvas::DuiMashupCanvas(const QString &identifier, QGraphicsItem *parent) :
    DuiWidgetController(new DuiMashupCanvasPrivate, new DuiMashupCanvasModel, parent)
{
    // Initialize the private implementation
    Q_D(DuiMashupCanvas);
    d->q_ptr = this;
    d->init(identifier);
}

DuiMashupCanvas::DuiMashupCanvas(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier) :
    DuiWidgetController(dd, model, parent)
{
    Q_D(DuiMashupCanvas);

    // Initialize the private implementation
    d->init(identifier);
}

DuiMashupCanvas::~DuiMashupCanvas()
{
}

void DuiMashupCanvas::setCategories(const QStringList &categories)
{
    model()->setCategories(categories);
}

QStringList DuiMashupCanvas::categories() const
{
    return model()->categories();
}

QString DuiMashupCanvas::identifier() const
{
    Q_D(const DuiMashupCanvas);

    return d->identifier;
}

QString DuiMashupCanvas::serviceAddress() const
{
    Q_D(const DuiMashupCanvas);

    return d->serviceAddress;
}

void DuiMashupCanvas::addWidget(DuiWidget *widget, DuiDataStore &store)
{
    Q_D(DuiMashupCanvas);

    d->addWidget(widget, store);
}

void DuiMashupCanvas::removeWidget(DuiWidget *widget)
{
    Q_D(DuiMashupCanvas);

    d->removeWidget(widget);
}

DuiAppletInstanceManager *DuiMashupCanvas::appletInstanceManager() const
{
    Q_D(const DuiMashupCanvas);
    return d->instanceManager;
}
