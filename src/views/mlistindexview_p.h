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

class MApplicationPage;
class MListIndex;
class MListIndexModel;
class MListIndexView;
class MLabel;
class MWidgetController;

class QGraphicsLinearLayout;
class QPropertyAnimation;

class MListIndexViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MListIndexView)
public:
    MListIndexViewPrivate();
    virtual ~MListIndexViewPrivate();

    virtual void applyStyleToShortcuts();

    virtual void initLayout();
    virtual void updateLayout();

    virtual QModelIndex locateShortcutIndex(int y, int x = 0);

    virtual MLabel *createLabel(int index);
    virtual void clearVisible();
    virtual void updateVisible();

    virtual void createContainer();

    qreal delayTimelineDuration();
    qreal timelineDuration();
    void updateDisplayMode();

protected:
    void scrollToGroupHeader(int y);

protected Q_SLOTS:
    virtual void listParentChanged();
    virtual void listPanningStarted();
    virtual void listPanningStopped();
    virtual void visibilityTimerTimeout();
    virtual void hideAnimated();
    virtual void showAnimated();

    virtual void exposedContentRectChanged();

private:
    MListIndex *controller;
    const MListIndexModel *controllerModel;
    QGraphicsLinearLayout *layout;

    MApplicationPage *container;

    int shortcutHeight;
    int shortcutsCount;
    QRectF containerRect;

    QPropertyAnimation *autoVisibilityAnimation;
    QTimer autoVisibilityTimer;
    bool down;

    MListIndexView *q_ptr;
};

#endif

