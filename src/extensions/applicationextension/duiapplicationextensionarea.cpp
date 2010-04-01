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

#include "duiapplicationextensionarea.h"
#include "duiapplicationextensionarea_p.h"
#include "duiapplicationextensionmanager.h"
#include "duidatastore.h"
#include "duiwidgetcreator.h"
#include "duiapplicationextensioninterface.h"

DUI_REGISTER_WIDGET_NO_CREATE(DuiApplicationExtensionArea)

///////////////////
// PRIVATE CLASS //
///////////////////

DuiApplicationExtensionAreaPrivate::DuiApplicationExtensionAreaPrivate() : extensionManager(NULL)
{
}

DuiApplicationExtensionAreaPrivate::~DuiApplicationExtensionAreaPrivate()
{
}

void DuiApplicationExtensionAreaPrivate::init(const QString &interface, const bool enableInProcessExtensions)
{
    Q_Q(DuiApplicationExtensionArea);
    extensionManager = QSharedPointer<DuiApplicationExtensionManager>(new DuiApplicationExtensionManager(interface, enableInProcessExtensions));
    QObject::connect(extensionManager.data(), SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface *)), q, SIGNAL(extensionInstantiated(DuiApplicationExtensionInterface *)), Qt::QueuedConnection);
    QObject::connect(extensionManager.data(), SIGNAL(extensionRemoved(DuiApplicationExtensionInterface*)), q, SIGNAL(extensionRemoved(DuiApplicationExtensionInterface*)), Qt::QueuedConnection);
    QObject::connect(extensionManager.data(), SIGNAL(widgetCreated(DuiWidget*, DuiDataStore&)), q, SLOT(addWidget(DuiWidget*, DuiDataStore&)));
    QObject::connect(extensionManager.data(), SIGNAL(widgetRemoved(DuiWidget*)), q, SLOT(removeWidget(DuiWidget*)));
    extensionManager->init();
}

QList<DuiApplicationExtensionInterface*> DuiApplicationExtensionAreaPrivate::extensions()
{
    return extensionManager.data()->extensions();
}

//////////////////
// PUBLIC CLASS //
//////////////////
DuiApplicationExtensionArea::DuiApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) :
    DuiExtensionArea(new DuiApplicationExtensionAreaPrivate, new DuiApplicationExtensionAreaModel, parent)
{
    // Initialize the private implementation
    Q_D(DuiApplicationExtensionArea);
    d->q_ptr = this;
    d->init(interface, enableInProcessExtensions);
}

DuiApplicationExtensionArea::DuiApplicationExtensionArea(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model,
        QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) :
    DuiExtensionArea(dd, model, parent)
{
    Q_D(DuiApplicationExtensionArea);

    // Initialize the private implementation
    d->init(interface, enableInProcessExtensions);
}

DuiApplicationExtensionArea::~DuiApplicationExtensionArea()
{
}

QList<DuiApplicationExtensionInterface*> DuiApplicationExtensionArea::extensions()
{
    Q_D(DuiApplicationExtensionArea);
    return d->extensions();
}
