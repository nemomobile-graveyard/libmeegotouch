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

#ifndef DUITEXTEDITVIEWZOOM_H
#define DUITEXTEDITVIEWZOOM_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimeLine>
#include <QTimer>

#include "duitexteditview.h"

class DuiSceneWindow;
// NOT A PUBLIC CLASS

class DuiTextEditViewZoom: public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    DuiTextEditViewZoom(DuiTextEditView *textEditView, const QPointF &startPoint);
    virtual ~DuiTextEditViewZoom();

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = 0);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

private slots:
    void zoom(int frame);
    void scroll();

private:
    void updateTransform();

    DuiTextEditView *zoomableView;
    QPointF mousePoint;
    QPointF zoomTarget;
    qreal zoomFactor;
    QTimeLine zoomInTimeLine;
    QTimeLine zoomOutTimeLine;
    QTimer scrollTimer;
    DuiSceneWindow *parent;
};

#endif
