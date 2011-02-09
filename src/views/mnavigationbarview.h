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

#ifndef MNAVIGATIONBARVIEW_H
#define MNAVIGATIONBARVIEW_H

#include "mscenewindowview.h"
#include "mnavigationbarmodel.h"
#include "mnavigationbarstyle.h"

class MNavigationBarViewPrivate;
class MNavigationBar;

class M_VIEWS_EXPORT MNavigationBarView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MNavigationBarModel, MNavigationBarStyle)

public:
    MNavigationBarView(MNavigationBar *controller);
    virtual ~MNavigationBarView();

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void setupModel();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MNavigationBarView)
    Q_DECLARE_PRIVATE(MNavigationBarView)
    Q_PRIVATE_SLOT(d_func(), void _q_updateIsEmptyAndJustEscapeButtonProperties())
    Q_PRIVATE_SLOT(d_func(), void _q_toolBarModelChanged(QList<const char *>))

    friend class Ut_MNavigationBarView;
};

#endif
