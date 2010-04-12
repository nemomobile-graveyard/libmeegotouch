/****************************************************************************
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

#include "mserviceaction.h"
#include "mserviceaction_p.h"
#include "mserviceinvoker.h"



MServiceActionPrivate::MServiceActionPrivate()
{
}

MServiceActionPrivate::~MServiceActionPrivate()
{
}



MServiceAction::MServiceAction(QObject *parent) : MAction(*new MServiceActionPrivate, parent)
{
    Q_D(MServiceAction);

    d->q_ptr = this;

    connect(this, SIGNAL(triggered()), MServiceInvoker::instance(), SLOT(invoke()));
}


MServiceAction::MServiceAction(MServiceActionPrivate &dd, QObject *parent) :
    MAction(dd, parent)
{
    connect(this, SIGNAL(triggered()), MServiceInvoker::instance(), SLOT(invoke()));
}

MServiceAction::~MServiceAction()
{
}

void MServiceAction::executeService()
{
}
