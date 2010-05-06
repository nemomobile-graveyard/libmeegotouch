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

#ifndef MWIDGETMODEL_P_H
#define MWIDGETMODEL_P_H

#include <QQueue>

class MWidgetModelPrivate
{
public:
    MWidgetModelPrivate();
    virtual ~MWidgetModelPrivate();
    QQueue<const char *> modifiedMembers;
    bool transactionInProgress;
    int referenceCount;
};

#endif

