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

#include "duiserviceinvoker.h"
#include <duiserviceaction.h>

DuiServiceInvoker *DuiServiceInvoker::theInstance = NULL;

DuiServiceInvoker::DuiServiceInvoker()
{

}

DuiServiceInvoker *DuiServiceInvoker::instance()
{
    if (theInstance == NULL) {
        theInstance = new DuiServiceInvoker();
    }

    return theInstance;
}

void DuiServiceInvoker::invoke()
{
    DuiServiceAction *action = qobject_cast<DuiServiceAction *>(sender());
    if (action != NULL) {
        action->executeService();
    }
}

