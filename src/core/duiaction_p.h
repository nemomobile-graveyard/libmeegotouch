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

#ifndef DUIACTION_P_H
#define DUIACTION_P_H

#include "duiaction.h"

class DuiActionPrivate
{
    Q_DECLARE_PUBLIC(DuiAction)

public:
    DuiActionPrivate();

    virtual ~DuiActionPrivate();

    void sendDataChanged();

    QString iconID;

    DuiAction::Locations location;

    bool styleAction;

protected:

    DuiAction *q_ptr;
};

#endif
