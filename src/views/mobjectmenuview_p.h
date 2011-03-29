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
#include "mstylablewidgetstyle.h"
#include "mstylablewidget.h"

#ifdef HAVE_CONTENTACTION
#include <contentaction/contentaction.h>
#endif

class MButton;
class MAction;
class QGraphicsGridLayout;
class QGraphicsLinearLayout;
class MLabel;
class MImageWidget;
class MSeparator;
class MPannableViewport;

class MObjectMenuViewPrivate
{
    Q_DECLARE_PUBLIC(MObjectMenuView)

protected:
    MObjectMenuView *q_ptr;

public:
    MObjectMenuViewPrivate();
    virtual ~MObjectMenuViewPrivate();

    void init();

    void updateIcon();
    void updateTitleAreaVisibility();

    void updateActionViewportSize();

    QHash<MAction *, MButton *> buttons;
    QList<QPair<int, MAction *> > delayedActionEvents;
#ifdef HAVE_CONTENTACTION
    QHash<MAction*, ContentAction::Action> contentActions;
#endif

    QGraphicsLinearLayout* mainLayout;
    QGraphicsLinearLayout* actionLayout;
    QGraphicsGridLayout* titleLayout;

    MObjectMenu *controller;

    MWidgetController* titleArea;
    MLabel* titleLabel;
    MImageWidget* titleIcon;
    MSeparator* titleSeparator;

    MPannableViewport* actionViewport;
    QGraphicsWidget* actionWidget;

private:
    void contentActionTriggered();
    void appearing();
};

class MObjectMenuTitleArea : public MStylableWidget
{
    Q_OBJECT
public:
    MObjectMenuTitleArea(QGraphicsItem *parent = 0) : MStylableWidget(parent) {}
private:
    M_STYLABLE_WIDGET(MStylableWidgetStyle)
};
#endif
