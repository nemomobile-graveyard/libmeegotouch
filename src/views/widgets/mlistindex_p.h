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

#ifndef MLISTINDEX_P_H
#define MLISTINDEX_P_H

#include <QObject>

class MList;

class MListIndexPrivate
{
public:
    MListIndexPrivate();
    virtual ~MListIndexPrivate();

    void init();
    void updateListConnections(MList *list);

    void _q_updateVisibility();

private:
    Q_DECLARE_PUBLIC(MListIndex)
    MListIndex *q_ptr;
};

#endif // MLISTINDEX_P_H
