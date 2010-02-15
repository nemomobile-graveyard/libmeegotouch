/****************************************************************************
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

#include "duiserviceaction.h"
#include "duiserviceaction_p.h"
#include "duiserviceinvoker.h"



DuiServiceActionPrivate::DuiServiceActionPrivate()
{
}

DuiServiceActionPrivate::~DuiServiceActionPrivate()
{
}



DuiServiceAction::DuiServiceAction(QObject *parent) : DuiAction(*new DuiServiceActionPrivate, parent)
{
    Q_D(DuiServiceAction);

    d->q_ptr = this;

    connect(this, SIGNAL(triggered()), DuiServiceInvoker::instance(), SLOT(invoke()));
}


DuiServiceAction::DuiServiceAction(DuiServiceActionPrivate &dd, QObject *parent) :
    DuiAction(dd, parent)
{
    connect(this, SIGNAL(triggered()), DuiServiceInvoker::instance(), SLOT(invoke()));
}

DuiServiceAction::~DuiServiceAction()
{
}

void DuiServiceAction::executeService()
{
}
