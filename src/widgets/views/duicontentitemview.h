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

#ifndef DUICONTENTITEMVIEW_H__
#define DUICONTENTITEMVIEW_H__

#include "duiwidgetview.h"
#include "duicontentitemmodel.h"
#include "duicontentitemstyle.h"

class DuiContentItem;
class DuiContentItemViewPrivate;

/*!
    \class DuiContentItemView
    \brief View class for DuiContentItem.

    \ingroup views

    \section DuiContentItemView Overview
        DuiContentItemView draws DuiContentItem as a 2 text lines and a thumbnail and any combination of those depending
        on the style.

        DuiContentItemView supports 4 different layouts for text and thumnbail. They have to be set through DuiContentItem::setItemStyle().

        \image html content-item-styles.png

        DuiContentItemView supports different modes which has to be set through DuiContentItem::setItemMode()

        \image html content-item-modes.png
  */
class DUI_EXPORT DuiContentItemView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiContentItemModel, DuiContentItemStyle)

public:
    DuiContentItemView(DuiContentItem *controller);
    virtual ~DuiContentItemView();

protected slots:
    virtual void updateData(const QList<const char *> &modifications);

protected:
    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    virtual void applyStyle();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
    //! \reimp_end

    void setSelected(bool selected);

    //! \cond
    DuiContentItemView(DuiContentItemViewPrivate &dd, DuiContentItem *controller);
    //! \cond_end

private:
    Q_DISABLE_COPY(DuiContentItemView)
    Q_DECLARE_PRIVATE(DuiContentItemView)
};

#endif
