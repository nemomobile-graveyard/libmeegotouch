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

#ifndef DUINAVIGATIONBARVIEW_P_H
#define DUINAVIGATIONBARVIEW_P_H

#include "duiscenewindowview_p.h"

class DuiNavigationBar;
class DuiApplicationMenuButton;
class DuiWidget;
class DuiToolBar;
class QGraphicsGridLayout;
class QGraphicsLinearLayout;

class DuiNavigationBarViewPrivate : public DuiSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(DuiNavigationBarView)
public:
    DuiNavigationBarViewPrivate();
    virtual ~DuiNavigationBarViewPrivate();

    void init();

    void setMenuButtonwidth();

    void finalizeEscapeButtonTransition();

    void escapeModeChanged();
    void notificationFlagChanged();
    void toolBarChanged();

    QGraphicsGridLayout *layout;
    QGraphicsLinearLayout *toolbarPlaceholderLayout;

    DuiApplicationMenuButton *applicationMenuButton;
    DuiWidget *toolbarPlaceholder;
    DuiToolBar *toolBar;
};

#endif
