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

#ifndef DUIOBJECTMENUVIEW_P_H
#define DUIOBJECTMENUVIEW_P_H

#include "duiscenewindowview_p.h"
#include "duiobjectmenuview.h"
#include <contentaction/contentaction.h>

class DuiButton;
class DuiAction;
class DuiWidget;
class DuiLayout;
class DuiPannableViewport;
class DuiLinearLayoutPolicy;
class DuiGridLayoutPolicy;

class DuiObjectMenuViewPrivate : public DuiSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(DuiObjectMenuView)

    void init();

    QHash<DuiAction *, DuiButton *> buttons;
    QHash<DuiAction*, ContentAction::Action> contentActions;

    DuiPannableViewport *viewport;
    DuiWidget *container;

    DuiLayout *layout;
    DuiLinearLayoutPolicy *portraitPolicy;
    DuiGridLayoutPolicy *landscapePolicy;
private:
    void contentActionTriggered();
};

#endif
