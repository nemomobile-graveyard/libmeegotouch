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

#ifndef MLISTINDEXVIEW_H
#define MLISTINDEXVIEW_H

#include <MWidgetView>
#include "mlistindexmodel.h"
#include "mlistindexstyle.h"

class MListIndex;
class MListIndexViewPrivate;

class MListIndexView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MListIndexModel, MListIndexStyle)

public:
    MListIndexView(MListIndex *controller);
    virtual ~MListIndexView();

protected:
    //! \reimp
    virtual void setupModel();
    virtual void applyStyle();

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture);
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture *gesture);
    //! \reimp_end

    //! \internal
    MListIndexViewPrivate *d_ptr;
    //! \internal_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *> &modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MListIndexView)
    Q_DECLARE_PRIVATE(MListIndexView)

    Q_PRIVATE_SLOT(d_func(), void _q_listParentChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_exposedContentRectChanged())
    Q_PRIVATE_SLOT(d_func(), void _q_listPanningStarted())
    Q_PRIVATE_SLOT(d_func(), void _q_listPanningStopped())
    Q_PRIVATE_SLOT(d_func(), void _q_visibilityTimerTimeout())


#ifdef UNIT_TEST
    friend class Ut_MListIndex;
#endif
};

#endif
