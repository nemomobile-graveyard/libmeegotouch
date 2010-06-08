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

void MApplicationExtensionAreaPrivate::init(const QString &interface, Qt::ConnectionType connectionType)
{
    Q_Q(MApplicationExtensionArea);
    extensionManager = QSharedPointer<MApplicationExtensionManager>(new MApplicationExtensionManager(interface));
    QObject::connect(extensionManager.data(), SIGNAL(extensionInstantiated(MApplicationExtensionInterface *)), q, SIGNAL(extensionInstantiated(MApplicationExtensionInterface *)), connectionType);
    QObject::connect(extensionManager.data(), SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), q, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)), connectionType);
    QObject::connect(extensionManager.data(), SIGNAL(widgetCreated(QGraphicsWidget*, MDataStore&)), q, SLOT(addWidget(QGraphicsWidget*, MDataStore&)));
    QObject::connect(extensionManager.data(), SIGNAL(widgetRemoved(QGraphicsWidget*)), q, SLOT(removeWidget(QGraphicsWidget*)));
}

QList<MApplicationExtensionInterface*> MApplicationExtensionAreaPrivate::extensions()
{
    return extensionManager.data()->extensions();
}

//////////////////
// PUBLIC CLASS //
//////////////////
MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, QGraphicsItem *parent) :
    MExtensionArea(new MApplicationExtensionAreaPrivate(), new MApplicationExtensionAreaModel, parent)
{
    Q_D(MApplicationExtensionArea);
    d->q_ptr = this;
    d->init(interface);
}

MApplicationExtensionArea::MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions, QGraphicsItem *parent) :
    MExtensionArea(new MApplicationExtensionAreaPrivate(), new MApplicationExtensionAreaModel, parent)
{
    Q_D(MApplicationExtensionArea);
    d->q_ptr = this;
    d->init(interface, Qt::QueuedConnection);

    if (!enableInProcessExtensions) {
        d->extensionManager->setInProcessFilter(QRegExp("$^"));
    }

    d->extensionManager->init();
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model, QGraphicsItem *parent, const QString &interface) :
    MExtensionArea(dd, model, parent)
{
    Q_D(MApplicationExtensionArea);
    d->init(interface);
}

MApplicationExtensionArea::MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model,
        QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions) :
    MExtensionArea(dd, model, parent)
{
    Q_D(MApplicationExtensionArea);
    d->init(interface, Qt::QueuedConnection);

    if (!enableInProcessExtensions) {
        d->extensionManager->setInProcessFilter(QRegExp("$^"));
    }
}

MApplicationExtensionArea::~MApplicationExtensionArea()
{
}

void MApplicationExtensionArea::setInProcessFilter(const QRegExp &inProcessFilter)
{
    Q_D(MApplicationExtensionArea);
    d->extensionManager->setInProcessFilter(inProcessFilter);
}

void MApplicationExtensionArea::setOutOfProcessFilter(const QRegExp &outOfProcessFilter)
{
    Q_D(MApplicationExtensionArea);
    d->extensionManager->setOutOfProcessFilter(outOfProcessFilter);
}

void MApplicationExtensionArea::setOrder(const QStringList &order)
{
    Q_D(MApplicationExtensionArea);
    d->extensionManager->setOrder(order);
}

bool MApplicationExtensionArea::init()
{
    Q_D(MApplicationExtensionArea);
    return d->extensionManager->init();
}

QList<MApplicationExtensionInterface*> MApplicationExtensionArea::extensions()
{
    Q_D(MApplicationExtensionArea);
    return d->extensions();
}
