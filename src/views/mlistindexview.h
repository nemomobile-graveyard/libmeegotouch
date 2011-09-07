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

#ifndef MLISTINDEXVIEW_H
#define MLISTINDEXVIEW_H

#include <MWidgetView>
#include <MSceneWindow>
#include "mlistindexmodel.h"
#include "mlistindexstyle.h"

class MListIndex;
class MListIndexViewPrivate;

class MListIndexView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MListIndexModel, MListIndexStyle)

    Q_PROPERTY(qreal contentOpacity READ contentOpacity WRITE setContentOpacity)

public:
    MListIndexView(MListIndex *controller);
    virtual ~MListIndexView();

protected:
    //! \reimp
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual void setupModel();
    virtual void applyStyle();

    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

    virtual void panGestureEvent(QGestureEvent *event, QPanGesture *gesture);
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *> &modifications);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    //! \reimp_end

    qreal contentOpacity() const;
    void setContentOpacity(qreal opacity);

private:
    Q_DISABLE_COPY(MListIndexView)
    Q_DECLARE_PRIVATE(MListIndexView)
    MListIndexViewPrivate *d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_attachToListContainer())
    Q_PRIVATE_SLOT(d_func(), void _q_dettachFromListContainer())

    Q_PRIVATE_SLOT(d_func(), void _q_attachToViewport())
    Q_PRIVATE_SLOT(d_func(), void _q_dettachFromViewport())

    Q_PRIVATE_SLOT(d_func(), void _q_showIfNeeded())
    Q_PRIVATE_SLOT(d_func(), void _q_appearOnSceneWindow())
    Q_PRIVATE_SLOT(d_func(), void _q_hideIfNeeded())
    Q_PRIVATE_SLOT(d_func(), void _q_updateGeometry())

    Q_PRIVATE_SLOT(d_func(), void _q_updateTitles())

    Q_PRIVATE_SLOT(d_func(), void _q_scrollToLastRow())

    Q_PRIVATE_SLOT(d_func(), void _q_updatePositionIndicatorPosition(QSizeF))
    Q_PRIVATE_SLOT(d_func(), void _q_updatePositionIndicatorPosition(QPointF))
    Q_PRIVATE_SLOT(d_func(), void _q_updatePositionIndicatorPosition(QRectF))

#ifdef UNIT_TEST
    friend class Ut_MListIndex;
#endif
};

#endif
