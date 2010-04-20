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

#ifndef MOBJECTMENUVIEW_P_H
#define MOBJECTMENUVIEW_P_H

#include "mobjectmenuview.h"

#ifdef HAVE_CONTENTACTION
#include <contentaction/contentaction.h>
#endif

class MButton;
class MAction;
class QGraphicsWidget;
class MLayout;
class MPannableViewport;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;

class MObjectMenuViewPrivate
{
    Q_DECLARE_PUBLIC(MObjectMenuView)

protected:
    MObjectMenuView *q_ptr;

public:
    MObjectMenuViewPrivate();
    virtual ~MObjectMenuViewPrivate();

    void init();

    QHash<MAction *, MButton *> buttons;

#ifdef HAVE_CONTENTACTION
    QHash<MAction*, ContentAction::Action> contentActions;
#endif

    MPannableViewport *viewport;
    QGraphicsWidget *container;

    MLayout *layout;
    MLinearLayoutPolicy *portraitPolicy;
    MGridLayoutPolicy *landscapePolicy;

    MObjectMenu *controller;

private:
    void contentActionTriggered();
};

#endif
