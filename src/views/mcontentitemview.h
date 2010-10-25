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

#ifndef MCONTENTITEMVIEW_H__
#define MCONTENTITEMVIEW_H__

#include "mwidgetview.h"
#include "mcontentitemmodel.h"
#include "mcontentitemstyle.h"

class MContentItem;
class MContentItemViewPrivate;

/*!
    \class MContentItemView
    \brief View class for MContentItem.

    \ingroup views

    \section MContentItemView Overview
        MContentItemView draws MContentItem as a 2 text lines and a thumbnail and any combination of those depending
        on the style.

        MContentItemView supports 4 different layouts for text and thumnbail. They have to be set through MContentItem::setItemStyle().

        \image html content-item-styles.png

        MContentItemView supports different modes which has to be set through MContentItem::setItemMode()

        \image html content-item-modes.png
  */
class M_VIEWS_EXPORT MContentItemView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MContentItemModel, MContentItemStyle)

public:
    MContentItemView(MContentItem *controller);
    virtual ~MContentItemView();

protected slots:
    virtual void updateData(const QList<const char *> &modifications);

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture);
    //! \reimp_end

    void setSelected(bool selected);

    //! \internal
    MContentItemView(MContentItemViewPrivate &dd, MContentItem *controller);
    //! \internal_end

private:
    Q_DISABLE_COPY(MContentItemView)
    Q_DECLARE_PRIVATE(MContentItemView)
    Q_PRIVATE_SLOT(d_func(), void _q_applyQueuedStyleModeChange())
};

#endif
