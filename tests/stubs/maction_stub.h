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

#ifndef MACTION_STUB
#define MACTION_STUB

#include "maction.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MActionStub : public StubBase
{
public:
    virtual void MActionConstructor(QObject *parent);
    virtual void MActionConstructor(const QString &text, QObject *parent);
    virtual void MActionConstructor(const QString &iconID, const QString &text, QObject *parent);
    virtual void MActionDestructor();
    virtual const QString &iconID() const;
    virtual void setIconID(const QString &id);
    virtual MAction::Locations location() const;
    virtual void setLocation(MAction::Locations location);
    virtual void setStyleAction(bool styleAction);
    virtual bool isStyleAction() const;
    MActionPrivate *d_ptr ;
    virtual void MActionConstructor(MActionPrivate &dd, QObject *parent);
};

// 2. IMPLEMENT STUB
void MActionStub::MActionConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MActionStub::MActionConstructor(const QString &text, QObject *parent)
{
    Q_UNUSED(text);
    Q_UNUSED(parent);

}
void MActionStub::MActionConstructor(const QString &iconID, const QString &text, QObject *parent)
{
    Q_UNUSED(iconID);
    Q_UNUSED(text);
    Q_UNUSED(parent);

}
void MActionStub::MActionDestructor()
{

}
const QString &MActionStub::iconID() const
{
    stubMethodEntered("iconID");
    return stubReturnValue<QString>("iconID");
}

void MActionStub::setIconID(const QString &id)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(id));
    stubMethodEntered("setIconID", params);
}

MAction::Locations MActionStub::location() const
{
    stubMethodEntered("location");
    return stubReturnValue<MAction::Locations>("location");
}

void MActionStub::setLocation(MAction::Locations location)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MAction::Locations >(location));
    stubMethodEntered("setLocation", params);
}

void MActionStub::setStyleAction(bool styleAction)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(styleAction));
    stubMethodEntered("setStyleAction", params);
}

bool MActionStub::isStyleAction() const
{
    stubMethodEntered("isStyleAction");
    return stubReturnValue<bool>("isStyleAction");
}

void MActionStub::MActionConstructor(MActionPrivate &dd, QObject *parent)
{
    Q_UNUSED(dd);
    Q_UNUSED(parent);

}


// 3. CREATE A STUB INSTANCE
MActionStub gDefaultMActionStub;
MActionStub *gMActionStub = &gDefaultMActionStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MAction::MAction(QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gMActionStub->MActionConstructor(parent);
}

MAction::MAction(const QString &text, QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gMActionStub->MActionConstructor(text, parent);
}

MAction::MAction(const QString &iconID, const QString &text, QObject *parent) : QAction(parent), d_ptr(NULL)
{
    gMActionStub->MActionConstructor(iconID, text, parent);
}

MAction::~MAction()
{
    gMActionStub->MActionDestructor();
}

QString MAction::iconID() const
{
    return gMActionStub->iconID();
}

void MAction::setIconID(const QString &id)
{
    gMActionStub->setIconID(id);
}

MAction::Locations MAction::location() const
{
    return gMActionStub->location();
}

void MAction::setLocation(MAction::Locations location)
{
    gMActionStub->setLocation(location);
}

void MAction::setStyleAction(bool styleAction)
{
    gMActionStub->setStyleAction(styleAction);
}

bool MAction::isStyleAction() const
{
    return gMActionStub->isStyleAction();
}

MAction::MAction(MActionPrivate &dd, QObject *parent) : QAction(parent), d_ptr(&dd)
{
    gMActionStub->MActionConstructor(dd, parent);
}


#endif
