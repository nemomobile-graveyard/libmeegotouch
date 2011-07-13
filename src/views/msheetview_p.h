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
class MSheetCentralSlot;
class MSheetView;
class MSheetSlot;
class MSheetSpacer;
class MPannableViewport;
class MSlotLayout;

class MSheetViewPrivate : public MSceneWindowViewPrivate
{
public:
    MSheetViewPrivate();
    virtual ~MSheetViewPrivate();

    void init();
    void updateStyle();

    void updateHeaderVisibility();

    void setupHeaderHidingAnimation();

private:
    Q_DECLARE_PUBLIC(MSheetView)
    MSheetView *q_ptr;

    QGraphicsLinearLayout* rootLayout;
    MSheetSpacer* rootLayoutHeaderSpacer;

    MSheetSlot *headerSlot;
    QParallelAnimationGroup* headerHidingAnimation;
    MSheetCentralSlot *centralSlot;

#ifdef UNIT_TEST
    friend class Ut_MSheetView;
#endif // UNIT_TEST
};

class MSheetSlot : public MStylableWidget
{
  Q_OBJECT

public:
    MSheetSlot(QGraphicsItem *parent = 0);
    virtual ~MSheetSlot();

    void setWidget(QGraphicsWidget *widget);

public slots:
    void hideSlot();

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void resizeChildWidget();

    QWeakPointer<QGraphicsWidget> widgetPointer;
    M_STYLABLE_WIDGET(MStylableWidgetStyle)

#ifdef UNIT_TEST
    friend class Ut_MSheetView;
#endif // UNIT_TEST
};

class MSheetSpacer : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight)
};

class MSheetCentralSlot : public MStylableWidget
{
  Q_OBJECT

public:
    MSheetCentralSlot(QGraphicsItem *parent = 0);
    virtual ~MSheetCentralSlot();

    void setWidget(QGraphicsWidget *widget);

    void setPositionIndicatorStyleName(const QString& name);

    void setWidgetSizePolicyRespected(bool enabled);

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:
    void destroyPannableViewportAndPannedSlot();
    void createPannableViewportAndPannedSlot();

    MSlotLayout *rootLayout;

    // The sole purpose of this internal pannable is to guarantee
    // proper input widget relocation if the central widget doesn't
    // have a pannable viewport.
    MPannableViewport *pannableViewport;
    // We put the widget inside a slot instead of directly into the
    // pannable viewport to ensure that its size hints are not obeyed
    // by our internal pannable viewport.
    QGraphicsWidget *pannedSlot;


    QWeakPointer<QGraphicsWidget> widgetPointer;
    M_STYLABLE_WIDGET(MStylableWidgetStyle)

#ifdef UNIT_TEST
    friend class Ut_MSheetView;
#endif // UNIT_TEST
};

#endif // MSHEETVIEW_P_H
