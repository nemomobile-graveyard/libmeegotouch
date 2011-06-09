/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MLISTITEMVIEW_P_H
#define MLISTITEMVIEW_P_H

#include <private/mwidgetview_p.h>

class MListItem;
class MTapStateMachine;

class MListItemViewPrivate : public MWidgetViewPrivate
{
public:
    MListItemViewPrivate(MWidgetController *controller);
    virtual ~MListItemViewPrivate();
    
    void init();
    void click();
    void longTap(const QPointF &pos);

    void _q_applyPressedStyle();
    void _q_applyReleasedStyle();

private:
    bool down;
    bool tapAndHoldStarted;
    bool expectMouseReleaseEvent;
    MListItem *controller;

    MTapStateMachine* tapStateMachine;
private:
    Q_DECLARE_PUBLIC(MListItemView)
};

#endif // MLISTITEMVIEW_P_H
