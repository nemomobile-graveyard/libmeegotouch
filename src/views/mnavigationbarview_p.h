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

#ifndef MNAVIGATIONBARVIEW_P_H
#define MNAVIGATIONBARVIEW_P_H

#include "mscenewindowview_p.h"
#include "mtoolbar.h"
#include <QPointer>

class MNavigationBar;
class MApplicationMenuButton;
class MWidget;
class MLayout;
class MLinearLayoutPolicy;
class QGraphicsLinearLayout;

class MNavigationBarViewPrivate : public MSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(MNavigationBarView)
public:
    MNavigationBarViewPrivate();
    virtual ~MNavigationBarViewPrivate();

    void init();

    void setMenuButtonwidth();

    void finalizeEscapeButtonTransition();

    void escapeModeChanged();
    void notificationFlagChanged();
    void toolBarChanged();

    MLayout *layout;
    MLinearLayoutPolicy *policyTitle;
    MLinearLayoutPolicy *policySimple;
    QGraphicsLinearLayout *toolBarLayout;

    MApplicationMenuButton *applicationMenuButton;
    QPointer<MToolBar> toolBar;
};

#endif
