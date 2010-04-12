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

#ifndef MMENUOBJECTVIEW_P_H
#define MMENUOBJECTVIEW_P_H

#include <QObject>
#include "mabstractlayoutpolicy.h"

class MWidget;
class MApplicationMenu;
class MScalableImage;
class MButton;
class MLinearLayoutPolicy;
class MLayout;
class MPannableViewport;
class MFreestyleLayoutPolicy;
class MAction;
class MWidget;

class MObjectMenuLayoutPolicy : public MAbstractLayoutPolicy
{

public:
    MObjectMenuLayoutPolicy(MLayout *layout);
    void setItemGeometry(QGraphicsLayoutItem *item, const QRectF &geom);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    void relayout();
    void removeAt(int index);
    virtual void addItem(QGraphicsLayoutItem *item);

private:
    QHash<QGraphicsLayoutItem *, QRectF> geometries;
};

class MMenuObjectViewPrivate : public QObject
{
    Q_DECLARE_PUBLIC(MMenuObjectView)

protected:
    MMenuObjectView *q_ptr;

public:
    MMenuObjectViewPrivate(MApplicationMenu *menu);

    virtual ~MMenuObjectViewPrivate();

    MApplicationMenu *controller;
    MPannableViewport *viewport;
    MWidget *container;

    QHash<MAction *, MWidget *> widgets;
    QHash<MAction *, MButton *> buttons;

    MLayout *layout;
    MLinearLayoutPolicy *linearPolicy;
    MObjectMenuLayoutPolicy *customPolicy;

    void addWidget(QAction *action);
    void removeWidget(QAction *action);
    void actionChanged(QAction *action);

    void init();
    void refreshLayout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
