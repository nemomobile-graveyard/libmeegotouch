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

#ifndef DUIPANNABLEWIDGETVIEW_P_H
#define DUIPANNABLEWIDGETVIEW_P_H

class DuiPannableWidget;

class DuiPannableWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiPannableWidgetView)

protected:
    DuiPannableWidgetView *q_ptr;

public:

    DuiPannableWidgetViewPrivate();
    virtual ~DuiPannableWidgetViewPrivate();

    DuiPannableWidget *controller;

    void _q_applyStyleToPhysics();
};

#endif
