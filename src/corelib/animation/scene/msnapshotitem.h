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
#ifndef MSNAPSHOTITEM_H
#define MSNAPSHOTITEM_H

#include <QGraphicsObject>

class QGLFramebufferObject;
class QGraphicsScene;
class QRectF;
class QImage;
class QPixmap;

class MSnapshotItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MSnapshotItem(const QRectF &targetRect,
            QGraphicsItem *parent = 0);

    virtual ~MSnapshotItem();

    void updateSnapshot();

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    //! \reimp_end

protected:
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
     virtual bool eventFilter(QObject *obj, QEvent *event);
#endif

private:
    QRectF m_boundingRect;
    QPixmap pixmap;
    QGLFramebufferObject *framebufferObject;
};

#endif
