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
class QParallelAnimationGroup;
class MSheetView;
class MSheetSlot;
class MSheetSpacer;
class MPannableViewport;

class MSheetViewPrivate : public MSceneWindowViewPrivate
{
public:
    MSheetViewPrivate();
    virtual ~MSheetViewPrivate();

    void init();
    void updateStyle();

    void updateHeaderVisibility();

    void setupAnimation();

private:
    Q_DECLARE_PUBLIC(MSheetView)
    MSheetView *q_ptr;

    QGraphicsLinearLayout* rootLayout;
    MSheetSpacer* rootLayoutHeaderSpacer;

    MSheetSlot *headerSlot;
    MSheetSlot *centralSlot;
    MPannableViewport *centralSlotPannableViewport;
    QParallelAnimationGroup* animationGroup;
};

class MSheetSlot : public MStylableWidget
{
  Q_OBJECT

public:
    MSheetSlot(QGraphicsItem *parent = 0);
    virtual ~MSheetSlot();

    void setWidget(QGraphicsWidget *widget);

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void resizeChildWidget();

    QWeakPointer<QGraphicsWidget> widgetPointer;
    M_STYLABLE_WIDGET(MStylableWidgetStyle)
};

class MSheetSpacer : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight)
};

#endif // MSHEETVIEW_P_H
