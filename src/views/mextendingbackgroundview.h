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

#ifndef MEXTENDINGBACKGROUNDVIEW_H
#define MEXTENDINGBACKGROUNDVIEW_H

#include <MWidgetView>
#include <MWidgetModel>
#include "mextendingbackgroundstyle.h"

class MExtendingBackgroundViewPrivate;

/*!
 * The extending background view draws a background that can extend beyond
 * the edges of the bounding rectangle.
 */
class M_VIEWS_EXPORT MExtendingBackgroundView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, MExtendingBackgroundStyle)

public:
    /*!
     * Constructs an MExtendingBackgroundView.
     *
     * \param controller the MWidgetController to be used
     */
    MExtendingBackgroundView(MWidgetController *container);

    /*!
     * Destroys the MExtendingBackgroundView.
     */
    virtual ~MExtendingBackgroundView();

protected:
    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

    /*!
     * Constructs an MExtendingBackgroundView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param controller the MWidgetController to be used
     */
    MExtendingBackgroundView(MExtendingBackgroundViewPrivate &dd, MWidgetController *controller);
    MExtendingBackgroundViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MExtendingBackgroundView)
    Q_DECLARE_PRIVATE(MExtendingBackgroundView)
};

#endif // MEXTENDINGBACKGROUNDVIEW_H
