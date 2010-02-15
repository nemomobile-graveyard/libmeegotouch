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
#include "duiappletinstancemanager.h"
#include "duidatastore.h"
#include "duiwidgetcreator.h"

DUI_REGISTER_WIDGET_NO_CREATE(DuiApplicationExtensionArea)

///////////////////
// PRIVATE CLASS //
///////////////////

DuiApplicationExtensionAreaPrivate::DuiApplicationExtensionAreaPrivate() : instanceManager(NULL)
{
}

DuiApplicationExtensionAreaPrivate::~DuiApplicationExtensionAreaPrivate()
{
    delete instanceManager;
}

void DuiApplicationExtensionAreaPrivate::init(const QString &interface)
{
    Q_Q(DuiApplicationExtensionArea);

    instanceManager = new DuiAppletInstanceManager(interface);

    // Connect applet instance manager signals and restore applet instances
    q->connect(instanceManager, SIGNAL(appletInstantiated(DuiWidget *, DuiDataStore &)), SLOT(addWidget(DuiWidget *, DuiDataStore &)));
    instanceManager->restoreApplets();
}

//////////////////
// PUBLIC CLASS //
//////////////////
DuiApplicationExtensionArea::DuiApplicationExtensionArea(const QString &interface, QGraphicsItem *parent) :
    DuiExtensionArea(new DuiApplicationExtensionAreaPrivate, new DuiApplicationExtensionAreaModel, parent)
{
    // Initialize the private implementation
    Q_D(DuiApplicationExtensionArea);
    d->q_ptr = this;
    d->init(interface);
}

DuiApplicationExtensionArea::DuiApplicationExtensionArea(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model,
        QGraphicsItem *parent, const QString &interface) :
    DuiExtensionArea(dd, model, parent)
{
    Q_D(DuiApplicationExtensionArea);

    // Initialize the private implementation
    d->init(interface);
}

DuiApplicationExtensionArea::~DuiApplicationExtensionArea()
{
}
