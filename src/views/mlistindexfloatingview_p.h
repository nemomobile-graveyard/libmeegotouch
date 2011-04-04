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

#ifndef MLISTINDEXFLOATINGVIEW_P_H
#define MLISTINDEXFLOATINGVIEW_P_H

#include "private/mwidgetview_p.h"
#include "mlistindexfloatingview.h"

#include <QTimer>

class MSceneWindow;
class MLabel;
class MListIndex;
class MListIndexFloatingView;
class MPannableViewport;
class MListIndexTooltip;

class QGraphicsLinearLayout;
class QPropertyAnimation;

class MListIndexFloatingViewPrivate : public MWidgetViewPrivate
{
public:
    MListIndexFloatingViewPrivate();
    virtual ~MListIndexFloatingViewPrivate();

    void init();
    void initLayout();
    void configureController();
    void connectToList();

    void updateLayout();
    void updateContentHeight();

    void attachToContainer();

    MListIndexTooltip *tooltip();

    void updateTooltipPosition(int y);
    void updateTooltipData();

    void scrollToGroupHeader(int y);
    void queueListScrollTo();

    void _q_recalculateListIndexRegion();
    void _q_recalculateTooltipOffsets();
    void _q_scrollListToCurrentIndex();
    void _q_listParentChanged();

    void beginFastScrolling(const QPointF &pos);
    void updateFastScrolling(const QPointF &offset);
    void endFastScrolling();

private:
    Q_DECLARE_PUBLIC(MListIndexFloatingView)
    MListIndex *controller;

    MSceneWindow *container;
    MList *list;
    QRectF containerRect;

    MListIndexTooltip *tooltipWidget;
    QModelIndex currentScrollToIndex;
    QTimer scrollToDelayQueue;

    qreal tooltipVerticalOffset;
    qreal contentHeight;

    QPointF fastScrollPosition;

#ifdef UNIT_TEST
    friend class Ut_MListIndexFloatingView;
#endif
};

#endif // MLISTINDEXFLOATINGVIEW_P_H
