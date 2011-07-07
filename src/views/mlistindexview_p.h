/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MLISTINDEXVIEW_P_H
#define MLISTINDEXVIEW_P_H

#include <QModelIndex>
#include <QRectF>
#include <QVector>
#include <QTimer>

class MLabel;
class MListIndex;
class MListIndexModel;
class MListIndexView;
class MListIndexTooltip;
class MPannableViewport;
class MSceneWindow;
class MWidgetController;
class MWindow;

class QGraphicsAnchorLayout;
class QGraphicsLinearLayout;
class QGraphicsWidget;
class QPropertyAnimation;

class MListIndexViewPrivate
{
public:
    MListIndexViewPrivate();
    virtual ~MListIndexViewPrivate();

    void init();
    void initLayout();
    void connectToList();

    void setGroupTitle(int item, int index);
    MLabel *createGroupTitleLabel();

    MListIndexTooltip *tooltip();
    MWindow *currentWindow();

    void setDisplayMode(int mode);

    void beginFastScrolling(const QPointF &pos);
    void updateFastScrolling(const QPointF &pos);
    void endFastScrolling();

    void updateGroupTitleHeight();

    void showTitlesPanel();

    void hideViewportPositionIndicator();
    void showViewportPositionIndicator();

protected:
    void scrollToGroupHeader(int y);

    void _q_attachToListContainer();
    void _q_attachToViewport();

    void _q_showIfNeeded();
    void _q_appearOnSceneWindow();
    void _q_hideIfNeeded();

    void _q_updateGeometry();

    void _q_updateTitles();

    void _q_scrollToLastRow();

    void _q_updatePositionIndicatorPosition(const QSizeF &viewportSize);
    void _q_updatePositionIndicatorPosition(const QPointF &viewportPosition);
    void _q_updatePositionIndicatorPosition(const QRectF &viewportRange);

private:
    Q_DECLARE_PUBLIC(MListIndexView)
    MListIndexView *q_ptr;

    MListIndex *controller;
    QGraphicsWidget *panel;

    QGraphicsAnchorLayout *layout;
    QGraphicsLinearLayout *panelLayout;

    MSceneWindow *container;
    QRectF containerRect;

    QPropertyAnimation *appearanceAnimation;
    QPropertyAnimation *tooltipAppearanceAnimation;
    QPropertyAnimation *panelAppearanceAnimation;

    MListIndexTooltip *tooltipWidget;

    MList *list;
    MPannableViewport *viewport;

    QTimer scrollToQueueTimer;

    QVector<MLabel *> groupTitleLabelWidgets;
    int groupTitleHeight;

    bool isFastScrolling;
    int lastFastScrollRow;

    qreal contentOpacity;

    QSizeF viewportSize;
    QRectF viewportAdjustedRange;
    QPoint positionIndicatorPosition;

    const int ScrollToDelay;
};

#endif

