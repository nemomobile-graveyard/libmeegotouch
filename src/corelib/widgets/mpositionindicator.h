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

#ifndef MPOSITIONINDICATOR_H
#define MPOSITIONINDICATOR_H


#include "mwidgetcontroller.h"
#include <mpositionindicatormodel.h>

class MPositionIndicatorPrivate;

/*!
 * \class MPositionIndicator
 *
 * \brief MPositionIndicator visualizes a position indicator. It is
 * typically used by MPannableViewport (which automatically creates
 * one) to show the panning position but works also separately.
 *
 * The size of MPositionIndicator is set by the layout it is in but
 * it only draws on the area near the edges of its area.
 *
 * MPositionIndicator slots setViewportSize, setRange and setPosition
 * need to be called when size of viewport, panned area inside
 * the viewport or panning position changes.
 *
 * MPositionIndicator is a non-interactive widget. It only shows the
 * current position, and it doesn't react to mouse events.
*/


class M_CORE_EXPORT MPositionIndicator : public MWidgetController
{
    Q_OBJECT

    M_CONTROLLER(MPositionIndicator)

    Q_PROPERTY(QSizeF viewportSize READ viewportSize WRITE setViewportSize)
    Q_PROPERTY(QRectF range READ range WRITE setRange)
    Q_PROPERTY(QPointF position READ position WRITE setPosition)

public:
    /*!
     * \brief Constructs a MPositionIndicator with the specified
     * panning direction and \a parent.
     */
    MPositionIndicator(QGraphicsItem *parent = 0);

    /*!
     * Destructs the MPositionIndicator.
     */
    virtual ~MPositionIndicator();

    /*!
     * \brief Returns the latest stored size information of the
     * pannable viewport to which the position indicator is attached
     * to.
     */
    QSizeF viewportSize() const;

    /*!
     * \brief Returns the latest stored size of the panned range
     * (normally the size of the widget that is being panned, see
     * MPannableViewport).
     */
    QRectF range() const;

    /*!
     * \brief Returns the latest stored current position of the panned
     * widget.
     */
    QPointF position() const;

public Q_SLOTS:

    /*!
     * \brief Updates the position data.
     */
    void setPosition(const QPointF &newPosition);

    /*!
     * \brief Updates the viewport size data.
     */
    void setViewportSize(const QSizeF &newViewportSize);

    /*!
     * \brief Updates the panned range data.
     */
    void setRange(const QRectF &newRange);

protected:

    MPositionIndicator(MPositionIndicatorPrivate *dd, MPositionIndicatorModel *model, QGraphicsItem *parent);

private:
    Q_DECLARE_PRIVATE(MPositionIndicator)
    Q_DISABLE_COPY(MPositionIndicator)
};

#endif
