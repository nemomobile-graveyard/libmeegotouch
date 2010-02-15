/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUIMENUOBJECTVIEW_P_H
#define DUIMENUOBJECTVIEW_P_H

#include "duiscenewindowview_p.h"
#include <QObject>
#include "duiabstractlayoutpolicy.h"

class DuiWidget;
class DuiApplicationMenu;
class DuiScalableImage;
class DuiButton;
class DuiLinearLayoutPolicy;
class DuiLayout;
class DuiPannableViewport;
class DuiFreestyleLayoutPolicy;
class DuiAction;
class DuiWidget;

class DuiObjectMenuLayoutPolicy : public DuiAbstractLayoutPolicy
{

public:
    DuiObjectMenuLayoutPolicy(DuiLayout *layout);
    void setItemGeometry(QGraphicsLayoutItem *item, const QRectF &geom);
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
    void relayout();
    void removeAt(int index);
    virtual void addItem(QGraphicsLayoutItem *item);

private:
    QHash<QGraphicsLayoutItem *, QRectF> geometries;
};

class DuiMenuObjectViewPrivate : public DuiSceneWindowViewPrivate, public QObject
{
    Q_DECLARE_PUBLIC(DuiMenuObjectView)

public:
    DuiMenuObjectViewPrivate(DuiApplicationMenu *menu);

    virtual ~DuiMenuObjectViewPrivate();

    DuiApplicationMenu *controller;
    DuiPannableViewport *viewport;
    DuiWidget *container;

    QHash<DuiAction *, DuiWidget *> widgets;
    QHash<DuiAction *, DuiButton *> buttons;

    DuiLayout *layout;
    DuiLinearLayoutPolicy *linearPolicy;
    DuiObjectMenuLayoutPolicy *customPolicy;

    void addWidget(QAction *action);
    void removeWidget(QAction *action);
    void actionChanged(QAction *action);

    void init();
    void refreshLayout();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif
