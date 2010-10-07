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

#include "mmashupcanvas.h"
#include "mmashupcanvas_p.h"
#include "mcomponentdata.h"
#include "mappletinstancemanager.h"
#include "mdatastore.h"
#include "mmashupcanvasstyle.h"
#include "mlocale.h"
#include <MAction>
#include <MDebug>

#include "mwidgetcreator.h"
M_REGISTER_WIDGET_NO_CREATE(MMashupCanvas)

///////////////////
// PRIVATE CLASS //
///////////////////

QSet<QString> MMashupCanvasPrivate::allMashupCanvasIdentifiers;

MMashupCanvasPrivate::MMashupCanvasPrivate()
    : instanceManager(NULL)
{
}

MMashupCanvasPrivate::~MMashupCanvasPrivate()
{
    delete instanceManager;
}

void MMashupCanvasPrivate::init(const QString &identifier)
{
    Q_Q(MMashupCanvas);

    this->identifier = provideUniqueIdentifier(identifier);
    this->serviceAddress = MComponentData::serviceName() + "/MAppletInstanceManager/" + this->identifier;
    instanceManager = new MAppletInstanceManager(this->identifier);

    // Connect applet instance manager signals and restore applet instances
    q->connect(instanceManager, SIGNAL(appletInstantiated(QGraphicsWidget *, MDataStore &)), SLOT(addWidget(QGraphicsWidget *, MDataStore &)));
    q->connect(instanceManager, SIGNAL(appletRemoved(QGraphicsWidget *)), SLOT(removeWidget(QGraphicsWidget *)));
    instanceManager->restoreApplets();

    // Put the data stores into the model
    q->model()->setDataStores(&dataStores);
}

QString MMashupCanvasPrivate::provideUniqueIdentifier(const QString &identifier)
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

void MMashupCanvasPrivate::addWidget(QGraphicsWidget *widget, MDataStore &store)
{
    Q_Q(MMashupCanvas);

    if (!dataStores.contains(widget)) {
        // Add the remove action into the object menu of the applet widget
        //: Object menu command. Removes e.g. applet from canvas.
        //~ uispec-document DirectUI_Common_Strings_UI_Specification_0.21.pdf
        //% "Remove"
        MAction *action = new MAction(qtTrId("qtn_comm_removewidget"), q);
        instanceManager->connect(action, SIGNAL(triggered(bool)), SLOT(removeActionTriggered(bool)));
        widget->addAction(action);
    }

    MExtensionAreaPrivate::addWidget(widget, store);
}



//////////////////
// PUBLIC CLASS //
//////////////////
MMashupCanvas::MMashupCanvas(const QString &identifier, QGraphicsItem *parent) :
    MExtensionArea(new MMashupCanvasPrivate, new MMashupCanvasModel, parent)
{
    // Initialize the private implementation
    Q_D(MMashupCanvas);
    d->init(identifier);
}

MMashupCanvas::MMashupCanvas(MMashupCanvasPrivate *dd, MMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier) :
    MExtensionArea(dd, model, parent)
{
    Q_D(MMashupCanvas);
    // Initialize the private implementation
    d->init(identifier);
}

MMashupCanvas::~MMashupCanvas()
{
}

void MMashupCanvas::setCategories(const QStringList &categories)
{
    model()->setCategories(categories);
}

QStringList MMashupCanvas::categories() const
{
    return model()->categories();
}

QString MMashupCanvas::identifier() const
{
    Q_D(const MMashupCanvas);

    return d->identifier;
}

QString MMashupCanvas::serviceAddress() const
{
    Q_D(const MMashupCanvas);

    return d->serviceAddress;
}

MAppletInstanceManager *MMashupCanvas::appletInstanceManager() const
{
    Q_D(const MMashupCanvas);
    return d->instanceManager;
}
