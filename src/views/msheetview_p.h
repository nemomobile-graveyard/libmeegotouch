/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSHEETVIEW_P_H
#define MSHEETVIEW_P_H

#include "mscenewindowview_p.h"
#include <mstylablewidget.h>
#include <mstylablewidgetstyle.h>
#include <QWeakPointer>

class QGraphicsLinearLayout;
class QGraphicsAnchorLayout;
class MSheetView;
class MSheetSlot;
class MAbstractWidgetAnimation;
class MPannableViewport;

class MSheetViewPrivate : public MSceneWindowViewPrivate
{
public:
    MSheetViewPrivate();
    virtual ~MSheetViewPrivate();

    void init();
    void updateStyle();

    void updateHeaderVisibility();

private:
    Q_DECLARE_PUBLIC(MSheetView)
    MSheetView *q_ptr;

    QGraphicsAnchorLayout *rootLayout;

    MSheetSlot *headerSlot;
    MSheetSlot *centralSlot;
    MPannableViewport *centralSlotPannableViewport;
    MAbstractWidgetAnimation *headerAnimation;
};

class MSheetSlot : public MStylableWidget
{
  Q_OBJECT

public:
    MSheetSlot(QGraphicsItem *parent = 0);
    virtual ~MSheetSlot();

    void setWidget(QGraphicsWidget *widget);

private Q_SLOTS:
    void resizeChildWidget();

private:
    QWeakPointer<QGraphicsWidget> widgetPointer;
    M_STYLABLE_WIDGET(MStylableWidgetStyle)
};

#endif // MSHEETVIEW_P_H
