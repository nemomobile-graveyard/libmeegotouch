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
#include "mstylablewidgetstyle.h"
#include "mstylablewidget.h"

class MNavigationBar;
class MApplicationMenuButton;
class MWidget;
class MLayout;
class MLinearLayoutPolicy;
class QGraphicsLinearLayout;
class MButton;
class MEscapeButtonSlot;
class MNavigationBarView;

class MNavigationBarViewPrivate : public MSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(MNavigationBarView)

public:
    MNavigationBarViewPrivate();
    virtual ~MNavigationBarViewPrivate();

    void init();

    void finalizeEscapeButtonTransition();

    void notificationFlagChanged();
    void toolBarChanged();
    void updateEscapeButton();
    void updateMenuButton();
    bool isEscapeVisible();
    void updateLayout();
    void updateToolBarAlignment();

    MLayout *layout;

    // parts of policy name describe which components are visible and in what order
    MLinearLayoutPolicy *menuToolbarEscapePolicy;

    MLinearLayoutPolicy *escapeToolbarMenuPolicy;
    MLinearLayoutPolicy *escapeToolbarPolicy;
    MLinearLayoutPolicy *toolbarPolicy;
    MLinearLayoutPolicy *toolbarMenuPolicy;

    QGraphicsWidget *toolBarSlot;
    QGraphicsLinearLayout *toolBarLayout;

    MApplicationMenuButton *applicationMenuButton;
    QPointer<MToolBar> toolBar;

    MEscapeButtonSlot* escapeButtonSlot;
    MButton *backButton;
    MButton *closeButton;

 protected:
    MNavigationBarView *q_ptr;
};

class MEscapeButtonSlot : public MStylableWidget
{
  Q_OBJECT

public:
    MEscapeButtonSlot(QGraphicsItem *parent = 0) : MStylableWidget(parent) {}
private:
    M_STYLABLE_WIDGET(MStylableWidgetStyle)
};

#endif
