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

#ifndef DUIACTION_STUB
#define DUIACTION_STUB

#include "duiaction.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiActionStub : public StubBase
{
public:
    virtual void DuiActionConstructor(QObject *parent);
    virtual void DuiActionConstructor(const QString &text, QObject *parent);
    virtual void DuiActionConstructor(const QString &iconID, const QString &text, QObject *parent);
    virtual void DuiActionDestructor();
    virtual const QString &iconID() const;
    virtual void setIconID(const QString &id);
    virtual DuiAction::Locations location() const;
    virtual void setLocation(DuiAction::Locations location);
    virtual void setStyleAction(bool styleAction);
    virtual bool isStyleAction() const;
    DuiActionPrivate *d_ptr ;
    virtual void DuiActionConstructor(DuiActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void DuiActionStub::DuiActionConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiActionStub::DuiActionConstructor(const QString &text, QObject *parent)
{
    Q_UNUSED(text);
    Q_UNUSED(parent);

}
void DuiActionStub::DuiActionConstructor(const QString &iconID, const QString &text, QObject *parent)
{
    Q_UNUSED(iconID);
    Q_UNUSED(text);
    Q_UNUSED(parent);

}
void DuiActionStub::DuiActionDestructor()
{

}
const QString &DuiActionStub::iconID() const
{
    stubMethodEntered("iconID");
    return stubReturnValue<QString>("iconID");
}

void DuiActionStub::setIconID(const QString &id)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(id));
    stubMethodEntered("setIconID", params);
}

DuiAction::Locations DuiActionStub::location() const
{
    stubMethodEntered("location");
    return stubReturnValue<DuiAction::Locations>("location");
}

void DuiActionStub::setLocation(DuiAction::Locations location)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiAction::Locations >(location));
    stubMethodEntered("setLocation", params);
}

void DuiActionStub::setStyleAction(bool styleAction)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(styleAction));
    stubMethodEntered("setStyleAction", params);
}

bool DuiActionStub::isStyleAction() const
{
    stubMethodEntered("isStyleAction");
    return stubReturnValue<bool>("isStyleAction");
}

void DuiActionStub::DuiActionConstructor(DuiActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
DuiActionStub gDefaultDuiActionStub;
DuiActionStub *gDuiActionStub = &gDefaultDuiActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiAction::DuiAction(QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gDuiActionStub->DuiActionConstructor(parent);
}

DuiAction::DuiAction(const QString &text, QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gDuiActionStub->DuiActionConstructor(text, parent);
}

DuiAction::DuiAction(const QString &iconID, const QString &text, QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gDuiActionStub->DuiActionConstructor(iconID, text, parent);
}

DuiAction::~DuiAction()
{
    gDuiActionStub->DuiActionDestructor();
}

QString DuiAction::iconID() const
{
    return gDuiActionStub->iconID();
}

void DuiAction::setIconID(const QString &id)
{
    gDuiActionStub->setIconID(id);
}

DuiAction::Locations DuiAction::location() const
{
    return gDuiActionStub->location();
}

void DuiAction::setLocation(DuiAction::Locations location)
{
    gDuiActionStub->setLocation(location);
}

void DuiAction::setStyleAction(bool styleAction)
{
    gDuiActionStub->setStyleAction(styleAction);
}

bool DuiAction::isStyleAction() const
{
    return gDuiActionStub->isStyleAction();
}

DuiAction::DuiAction(DuiActionPrivate &dd, QObject *parent) : QAction(parent), d_ptr(&dd)
{
    gDuiActionStub->DuiActionConstructor(dd, parent);
}


#endif
