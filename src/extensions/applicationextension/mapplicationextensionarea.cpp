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

#include "mapplicationextensionarea.h"
#include "mapplicationextensionarea_p.h"
#include "mapplicationextensionmanager.h"
#include "mdatastore.h"
#include "mwidgetcreator.h"
#include "mapplicationextensioninterface.h"

M_REGISTER_WIDGET_NO_CREATE(MApplicationExtensionArea)

///////////////////
// PRIVATE CLASS //
///////////////////

MApplicationExtensionAreaPrivate::MApplicationExtensionAreaPrivate() : extensionManager(NULL)
{
}

MApplicationExtensionAreaPrivate::~MApplicationExtensionAreaPrivate()
{
}

void MApplicationExtensionAreaPrivate::init(const QString &interface, const bool enableInProcessExtensions)
{
    Q_Q(MApplicationExtensionArea);
    extensionManager = QSharedPointer<MApplicationExtensionManager>(new MApplicationExtensionManager(interface, enableInProcessExtensions));
    QObject::connect(extensionManager.data(), SIGNAL(extensionInstantiated(MApplicationExtensionInterface *)), q, SIGNAL(extensionInstantiated(MApplicationExtensionInterface *)), Qt::QueuedConnection);
    QObject::connect(extensionManager.data(), SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), q, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), Qt::QueuedConnection);
    QObject::connect(extensionManager.data(), SIGNAL(widgetCreated(MWidget*, MDataStore&)), q, SLOT(addWidget(MWidget*, MDataStore&)));
    QObject::connect(extensionManager.data(), SIGNAL(widgetRemoved(MWidget*)), q, SLOT(removeWidget(MWidget*)));
    extensionManager->init();
}

QList<MApplicationExtensionInterface*> MApplicationExtensionAreaPrivate::extensions()
{
    return extensionManager.data()->extensions();
}

//////////////////
// PUBLIC CLASS //
//////////////////
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) :
    MExtensionArea(new MApplicationExtensionAreaPrivate, new MApplicationExtensionAreaModel, parent)
{
    // Initialize the private implementation
    Q_D(MApplicationExtensionArea);
    d->q_ptr = this;
    d->init(interface, enableInProcessExtensions);
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model,
        QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) :
    MExtensionArea(dd, model, parent)
{
    Q_D(MApplicationExtensionArea);

    // Initialize the private implementation
    d->init(interface, enableInProcessExtensions);
}

MApplicationExtensionArea::~MApplicationExtensionArea()
{
}

QList<MApplicationExtensionInterface*> MApplicationExtensionArea::extensions()
{
    Q_D(MApplicationExtensionArea);
    return d->extensions();
}
