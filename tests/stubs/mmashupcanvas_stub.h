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

#ifndef MMASHUPCANVAS_STUB
#define MMASHUPCANVAS_STUB

#include "mmashupcanvas.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MMashupCanvasStub : public StubBase
{
public:
    virtual void MMashupCanvasConstructor(const QString &identifier, QGraphicsItem *parent);
    virtual void MMashupCanvasDestructor();
    virtual MAppletInstanceManager *appletInstanceManager() const;
    virtual void setCategories(const QStringList &categories);
    virtual QStringList categories() const;
    virtual QString identifier() const;
    virtual QString serviceAddress() const;
    virtual void MMashupCanvasConstructor(MMashupCanvasPrivate *dd, MMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier);
    virtual void addWidget(MWidget *widget, MDataStore &store);
    virtual void removeWidget(MWidget *widget);
};

// 2. IMPLEMENT STUB
void MMashupCanvasStub::MMashupCanvasConstructor(const QString &identifier, QGraphicsItem *parent)
{
    Q_UNUSED(identifier);
    Q_UNUSED(parent);

}
void MMashupCanvasStub::MMashupCanvasDestructor()
{

}
MAppletInstanceManager *MMashupCanvasStub::appletInstanceManager() const
{
    stubMethodEntered("appletInstanceManager");
    return stubReturnValue<MAppletInstanceManager *>("appletInstanceManager");
}

void MMashupCanvasStub::setCategories(const QStringList &categories)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(categories));
    stubMethodEntered("setCategories", params);
}

QStringList MMashupCanvasStub::categories() const
{
    stubMethodEntered("categories");
    return stubReturnValue<QStringList>("categories");
}

QString MMashupCanvasStub::identifier() const
{
    stubMethodEntered("identifier");
    return stubReturnValue<QString>("identifier");
}

QString MMashupCanvasStub::serviceAddress() const
{
    stubMethodEntered("serviceAddress");
    return stubReturnValue<QString>("serviceAddress");
}

void MMashupCanvasStub::MMashupCanvasConstructor(MMashupCanvasPrivate *dd, MMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier)
{
    Q_UNUSED(dd);
    Q_UNUSED(model);
    Q_UNUSED(parent);
    Q_UNUSED(identifier);
}

void MMashupCanvasStub::addWidget(MWidget *widget, MDataStore &store)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    params.append(new Parameter<MDataStore & >(store));
    stubMethodEntered("addWidget", params);
}

void MMashupCanvasStub::removeWidget(MWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidget * >(widget));
    stubMethodEntered("removeWidget", params);
}

// 3. CREATE A STUB INSTANCE
MMashupCanvasStub gDefaultMMashupCanvasStub;
MMashupCanvasStub *gMMashupCanvasStub = &gDefaultMMashupCanvasStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MMashupCanvas::MMashupCanvas(const QString &identifier, QGraphicsItem *parent)
{
    gMMashupCanvasStub->MMashupCanvasConstructor(identifier, parent);
}

MMashupCanvas::~MMashupCanvas()
{
    gMMashupCanvasStub->MMashupCanvasDestructor();
}

MAppletInstanceManager *MMashupCanvas::appletInstanceManager() const
{
    return gMMashupCanvasStub->appletInstanceManager();
}

void MMashupCanvas::setCategories(const QStringList &categories)
{
    gMMashupCanvasStub->setCategories(categories);
}

QStringList MMashupCanvas::categories() const
{
    return gMMashupCanvasStub->categories();
}

QString MMashupCanvas::identifier() const
{
    return gMMashupCanvasStub->identifier();
}

QString MMashupCanvas::serviceAddress() const
{
    return gMMashupCanvasStub->serviceAddress();
}

MMashupCanvas::MMashupCanvas(MMashupCanvasPrivate *dd, MMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier)
{
    gMMashupCanvasStub->MMashupCanvasConstructor(dd, model, parent, identifier);
}

#endif
