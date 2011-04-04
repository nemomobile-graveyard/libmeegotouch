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

#ifndef MLISTINDEXVIEW_P_H
#define MLISTINDEXVIEW_P_H

#include <QModelIndex>
#include <QRectF>
#include <QVector>
#include <QTimer>

class MSceneWindow;
class MListIndex;
class MListIndexModel;
class MListIndexView;
class MLabel;
class MWidgetController;

class QGraphicsLinearLayout;
class QPropertyAnimation;

class MListIndexViewPrivate
{
public:
    MListIndexViewPrivate();
    virtual ~MListIndexViewPrivate();

    void init();

    virtual void applyStyleToShortcuts();

    void initLayout();
    void updateLayout();
    void connectToList();

    virtual QModelIndex locateShortcutIndex(int y, int x = 0);

    virtual MLabel *createLabel(int index);
    virtual void clearVisible();
    virtual void updateVisible();

    virtual void createContainer();

    qreal delayTimelineDuration();
    qreal timelineDuration();
    void updateDisplayMode();

    void beginFastScrolling(const QPointF &pos);
    void updateFastScrolling(const QPointF &offset);
    void endFastScrolling();

protected:
    void scrollToGroupHeader(int y);

    virtual void _q_listParentChanged();
    virtual void _q_listPanningStarted();
    virtual void _q_listPanningStopped();
    virtual void _q_visibilityTimerTimeout();

    virtual void _q_hideAnimated();
    virtual void _q_showAnimated();

    virtual void _q_startVisibilityTimer();
    virtual void _q_stopVisibilityTimer();

    virtual void _q_exposedContentRectChanged();

private:
    Q_DECLARE_PUBLIC(MListIndexView)
    MListIndexView *q_ptr;

    MListIndex *controller;
    QGraphicsLinearLayout *layout;

    MSceneWindow *container;
    MList *list;

    int shortcutHeight;
    int shortcutsCount;
    QRectF containerRect;

    QPropertyAnimation *autoVisibilityAnimation;
    QTimer autoVisibilityTimer;
    bool down;
    QModelIndex currentIndex;
    QPointF fastScrollPosition;
};

#endif

