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
    this->identifier = provideUniqueIdentifier(identifier);
    this->serviceAddress = DuiComponentData::serviceName() + "/DuiAppletInstanceManager/" + this->identifier;
    instanceManager = new DuiAppletInstanceManager(this->identifier);
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

    // Initialize this object
    init();
}

DuiMashupCanvas::DuiMashupCanvas(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier) :
    DuiWidgetController(dd, model, parent)
{
    Q_D(DuiMashupCanvas);

    // Initialize the private implementation
    d->init(identifier);

    // Initialize this object
    init();
}

DuiMashupCanvas::~DuiMashupCanvas()
{
}

void DuiMashupCanvas::init()
{
    Q_D(DuiMashupCanvas);

    // Connect applet instance manager signals and restore applet instances
    connect(d->instanceManager, SIGNAL(appletInstantiated(DuiWidget*,DuiDataStore&)), this, SLOT(addWidget(DuiWidget*,DuiDataStore&)));
    connect(d->instanceManager, SIGNAL(appletRemoved(DuiWidget*)), this, SLOT(removeWidget(DuiWidget*)));
    d->instanceManager->restoreApplets();

    // Put the data stores into the model
    model()->setDataStores(&d->dataStores);
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

    if (!d->dataStores.contains(widget)) {
        // Add data store to data stores map
        d->dataStores[widget] = &store;

        // Add the remove action into the object menu of the applet widget
        //: Object menu command. Removes e.g. applet from canvas.
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.7.pdf
        //% "Remove Applet"
        DuiAction *action = new DuiAction( qtTrId( "qtn_comm_removewidget"), this);
        connect(action, SIGNAL(triggered(bool)), d->instanceManager, SLOT(removeActionTriggered(bool)));
        widget->addAction(action);

        // Let the view know about the data store modification
        model()->dataStoresModified();
    } else {
        // Widget is already added to the mashup canvas. Bail out.
        duiWarning("DuiMashupCanvas") << "DuiMashupCanvas::addWidget() - Widget was already added to mashup canvas.";
    }
}

void DuiMashupCanvas::removeWidget(DuiWidget *widget)
{
    Q_D(DuiMashupCanvas);

    if (d->dataStores.contains(widget)) {
        // Remove data store from the data stores map
        d->dataStores.remove(widget);
        model()->dataStoresModified();
    }
}

DuiAppletInstanceManager *DuiMashupCanvas::appletInstanceManager() const
{
    Q_D(const DuiMashupCanvas);
    return d->instanceManager;
}
