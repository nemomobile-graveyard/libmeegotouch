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

#ifndef DUIPOSITIONINDICATOR_H
#define DUIPOSITIONINDICATOR_H


#include "duiwidgetcontroller.h"
#include <duipositionindicatormodel.h>

class DuiPositionIndicatorPrivate;

/*!
 * \class DuiPositionIndicator
 *
 * \brief DuiPositionIndicator visualizes a position indicator. It is
 * typically used by DuiPannableViewport (which automatically creates
 * one) to show the panning position but works also separately.
 *
 * The size of DuiPositionIndicator is set by the layout it is in but
 * it only draws on the area near the edges of its area.
 *
 * DuiPositionIndicator has a slot updateSizePosData() which needs to
 * be called when size of viewport, panned area inside the viewport or
 * panning position changes.
 *
 * DuiPositionIndicator is a non-interactive widget. It only shows the
 * current position, and it doesn't react to mouse events.
*/


class DUI_EXPORT DuiPositionIndicator : public DuiWidgetController
{
    Q_OBJECT

    DUI_CONTROLLER(DuiPositionIndicator)

    Q_PROPERTY(QSizeF viewportSize READ viewportSize)
    Q_PROPERTY(QRectF pannedRange READ pannedRange)
    Q_PROPERTY(QPointF pannedPos READ pannedPos)

public:
    /*!
     * \brief Constructs a DuiPositionIndicator with the specified
     * panning direction and \a parent.
     */
    DuiPositionIndicator(QGraphicsItem *parent = 0);

    /*!
     * Destructs the DuiPositionIndicator.
     */
    virtual ~DuiPositionIndicator();

    /*!
     * \brief Returns the latest stored size information of the
     * pannable viewport to which the position indicator is attached
     * to.
     */
    QSizeF viewportSize() const;

    /*!
     * \brief Returns the latest stored size of the panned range
     * (normally the size of the widget that is being panned, see
     * DuiPannableViewport).
     */
    QRectF pannedRange() const;

    /*!
     * \brief Returns the latest stored current position of the panned
     * widget.
     */
    QPointF pannedPos() const;

public Q_SLOTS:

    /*!
     * \brief Updates the size and position data.
     */
    void updateSizePosData(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);

protected:

    DuiPositionIndicator(DuiPositionIndicatorPrivate *dd, DuiPositionIndicatorModel *model, QGraphicsItem *parent);

private:
    Q_DECLARE_PRIVATE(DuiPositionIndicator)
    Q_DISABLE_COPY(DuiPositionIndicator)
};

#endif
