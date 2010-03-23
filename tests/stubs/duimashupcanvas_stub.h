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

#ifndef DUIMASHUPCANVAS_STUB
#define DUIMASHUPCANVAS_STUB

#include "duimashupcanvas.h"
#include <stubbase.h>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiMashupCanvasStub : public StubBase
{
public:
    virtual void DuiMashupCanvasConstructor(const QString &identifier, QGraphicsItem *parent);
    virtual void DuiMashupCanvasDestructor();
    virtual DuiAppletInstanceManager *appletInstanceManager() const;
    virtual void setCategories(const QStringList &categories);
    virtual QStringList categories() const;
    virtual QString identifier() const;
    virtual QString serviceAddress() const;
    virtual void DuiMashupCanvasConstructor(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier);
    virtual void addWidget(DuiWidget *widget, DuiDataStore &store);
    virtual void removeWidget(DuiWidget *widget);
};

// 2. IMPLEMENT STUB
void DuiMashupCanvasStub::DuiMashupCanvasConstructor(const QString &identifier, QGraphicsItem *parent)
{
    Q_UNUSED(identifier);
    Q_UNUSED(parent);

}
void DuiMashupCanvasStub::DuiMashupCanvasDestructor()
{

}
DuiAppletInstanceManager *DuiMashupCanvasStub::appletInstanceManager() const
{
    stubMethodEntered("appletInstanceManager");
    return stubReturnValue<DuiAppletInstanceManager *>("appletInstanceManager");
}

void DuiMashupCanvasStub::setCategories(const QStringList &categories)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QStringList & >(categories));
    stubMethodEntered("setCategories", params);
}

QStringList DuiMashupCanvasStub::categories() const
{
    stubMethodEntered("categories");
    return stubReturnValue<QStringList>("categories");
}

QString DuiMashupCanvasStub::identifier() const
{
    stubMethodEntered("identifier");
    return stubReturnValue<QString>("identifier");
}

QString DuiMashupCanvasStub::serviceAddress() const
{
    stubMethodEntered("serviceAddress");
    return stubReturnValue<QString>("serviceAddress");
}

void DuiMashupCanvasStub::DuiMashupCanvasConstructor(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier)
{
    Q_UNUSED(dd);
    Q_UNUSED(model);
    Q_UNUSED(parent);
    Q_UNUSED(identifier);
}

void DuiMashupCanvasStub::addWidget(DuiWidget *widget, DuiDataStore &store)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    params.append(new Parameter<DuiDataStore & >(store));
    stubMethodEntered("addWidget", params);
}

void DuiMashupCanvasStub::removeWidget(DuiWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidget * >(widget));
    stubMethodEntered("removeWidget", params);
}

// 3. CREATE A STUB INSTANCE
DuiMashupCanvasStub gDefaultDuiMashupCanvasStub;
DuiMashupCanvasStub *gDuiMashupCanvasStub = &gDefaultDuiMashupCanvasStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiMashupCanvas::DuiMashupCanvas(const QString &identifier, QGraphicsItem *parent)
{
    gDuiMashupCanvasStub->DuiMashupCanvasConstructor(identifier, parent);
}

DuiMashupCanvas::~DuiMashupCanvas()
{
    gDuiMashupCanvasStub->DuiMashupCanvasDestructor();
}

DuiAppletInstanceManager *DuiMashupCanvas::appletInstanceManager() const
{
    return gDuiMashupCanvasStub->appletInstanceManager();
}

void DuiMashupCanvas::setCategories(const QStringList &categories)
{
    gDuiMashupCanvasStub->setCategories(categories);
}

QStringList DuiMashupCanvas::categories() const
{
    return gDuiMashupCanvasStub->categories();
}

QString DuiMashupCanvas::identifier() const
{
    return gDuiMashupCanvasStub->identifier();
}

QString DuiMashupCanvas::serviceAddress() const
{
    return gDuiMashupCanvasStub->serviceAddress();
}

DuiMashupCanvas::DuiMashupCanvas(DuiMashupCanvasPrivate *dd, DuiMashupCanvasModel *model, QGraphicsItem *parent, const QString &identifier)
{
    gDuiMashupCanvasStub->DuiMashupCanvasConstructor(dd, model, parent, identifier);
}

#endif
