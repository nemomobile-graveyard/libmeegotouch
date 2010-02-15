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

#ifndef DUIWIDGETRECYCLER_P_H
#define DUIWIDGETRECYCLER_P_H

#include <QHash>
#include <QMultiHash>

class DuiWidget;

class DuiWidgetRecyclerPrivate
{

public:
    DuiWidgetRecyclerPrivate();
    ~DuiWidgetRecyclerPrivate();

    void resetWidgetState(DuiWidget *widget);
    bool hasEnoughSpaceFor(DuiWidget *widget);
    void put(DuiWidget *widget);
    DuiWidget *take(const QString &className);

    QMultiHash<QString, DuiWidget *> widgets;
    int widgetCount;
    int maxWidgetCount;
};

#endif
