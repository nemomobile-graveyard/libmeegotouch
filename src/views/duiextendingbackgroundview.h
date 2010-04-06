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

#ifndef DUIEXTENDINGBACKGROUNDVIEW_H
#define DUIEXTENDINGBACKGROUNDVIEW_H

#include <DuiWidgetView>
#include <DuiWidgetModel>
#include "duiextendingbackgroundstyle.h"

class DuiExtendingBackgroundViewPrivate;

/*!
 * The extending background view draws a background that can extend beyond
 * the edges of the bounding rectangle.
 */
class DUI_EXPORT DuiExtendingBackgroundView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiWidgetModel, DuiExtendingBackgroundStyle)

public:
    /*!
     * Constructs an DuiExtendingBackgroundView.
     *
     * \param controller the DuiWidgetController to be used
     */
    DuiExtendingBackgroundView(DuiWidgetController *container);

    /*!
     * Destroys the DuiExtendingBackgroundView.
     */
    virtual ~DuiExtendingBackgroundView();

protected:
    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

    /*!
     * Constructs an DuiExtendingBackgroundView. Should be used from derived classes.
     *
     * \param dd the private class
     * \param controller the DuiWidgetController to be used
     */
    DuiExtendingBackgroundView(DuiExtendingBackgroundViewPrivate &dd, DuiWidgetController *controller);
    DuiExtendingBackgroundViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiExtendingBackgroundView)
    Q_DECLARE_PRIVATE(DuiExtendingBackgroundView)
};

#endif // DUIEXTENDINGBACKGROUNDVIEW_H
