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

#ifndef DUIPANNABLEWIDGET_H
#define DUIPANNABLEWIDGET_H

#include "duiwidgetcontroller.h"
#include "duiphysics2dpanning.h"
#include <duipannablewidgetmodel.h>

class QTimerEvent;
class QGraphicsSceneMouseEvent;
class DuiPannableWidgetPrivate;
class DuiOnDisplayChangeEvent;

/*!
 * \class DuiPannableWidget
 * \brief Base class which adds support for panning gesture
 *
 * DuiPannableWidget creates a glass on top of itself and using the
 * glass, intercepts events coming from QGraphicsScene onto its area
 * and interprets them. If the events form a panning gesture, a
 * physics class is used to create a natural Newtonian movement in the
 * direction of panning. Panning position is provided as an
 * overridable slot UpdatePosition(). If the events don't form a
 * panning gesture, they are forwarded underneath the glass.
 */
class DUI_EXPORT DuiPannableWidget : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiPannableWidget)

    //! \brief Enabled status
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    //! \brief Enabled panning directions
    Q_PROPERTY(Qt::Orientations panDirection READ panDirection WRITE setPanDirection)
    //! \brief Panning range
    Q_PROPERTY(QRectF range READ range WRITE setRange)
    //! \brief Panning position
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged USER true)
    //! \brief Panning threshold
    Q_PROPERTY(qreal panThreshold READ panThreshold WRITE setPanThreshold)

public:
    /*!
     * \brief Constructs a pannable widget with a \a parent.
     */
    DuiPannableWidget(QGraphicsItem *parent = 0);

    /*!
     * \brief Destroys the pannable widget.
     */
    virtual ~DuiPannableWidget();

    /*!
     * \brief Returns the physics class instance used in
     * panning.
     */
    DuiPhysics2DPanning *physics() const;

    /*!
     * \brief Sets the physics engine object used when the widget is
     * panning the view.
     *
     * This method can be used to modify the behaviour of the widget
     * so that the kinetic panning effect is different than default.
     *
     * The DuiPannableWidget will take ownership of the physics engine
     * object.
     */
    void setPhysics(DuiPhysics2DPanning *physics);

    /*!
     * \brief Sets the enabled status of the pannable widget.
     */
    void setEnabled(bool enabled);

    /*!
     * \brief Returns the enabled status of the pannable widget.
     */
    bool isEnabled();

    /*!
     * \brief Sets the \a range of the panning.
     *
     * When range is zero along some axis, user can still make a
     * panning gesture on that direction but the position snaps back
     * to 0.0. If the range is shrank so that the current position
     * goes to border, the border springs are activated.
     *
     * By default, the range is QRectF(0,0).
     */
    virtual void setRange(const QRectF &range);

    /*!
     * \brief Returns the range of the panning.
     */
    virtual QRectF range() const;

    /*!
     * \brief Sets the \a position of the panning.
     *
     * If the new position is in the border, the border springs are
     * activated.
     *
     * By default, the position is QPointF(0,0).
     */
    virtual void setPosition(const QPointF &position);

    /*!
     * \brief Returns the current position of the panning.
     */
    virtual QPointF position() const;

    /*!
     * \brief Sets the enabled panning directions.
     *
     * The accepted values are Qt::Horizontal and/or Qt::Vertical.
     */
    virtual void setPanDirection(const Qt::Orientations &panDirection);

    /*!
     * \brief Returns the enabled panning directions.
     *
     * By default, the panning is enabled in Qt::Vertical direction.
     */
    Qt::Orientations panDirection();

    /*!
     * \brief Sets the panning threshold. This method should be used
     * only by the view. For setting this value, use css.
     */
    void setPanThreshold(qreal value);

    /*!
     * \brief Returns the current panning threshold.
     */
    qreal panThreshold();

public Q_SLOTS:
    /*!
     * \brief Virtual slot for receiving position changes from
     * physics.
     */
    virtual void updatePosition(const QPointF &position);

Q_SIGNALS:

    /*!
     * \brief this signal is emitted when the position of the pannableWidget changes
     */
    void positionChanged(QPointF newPosition);

    /*!
     * \brief Signals that the panning has stopped.
     */
    void panningStopped();

    /*!
     * \brief Signals that physics engine changed.
     */
    void physicsChanged();

protected:
    /*!
     * Protected constructor for derived classes.
     */
    DuiPannableWidget(DuiPannableWidgetPrivate *dd, DuiPannableWidgetModel *model,
                      QGraphicsItem *parent);

    //! \reimp
    virtual void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiPannableWidget)
    Q_DECLARE_PRIVATE(DuiPannableWidget)

    /*!
     * \brief Method for handling the press events from the glass.
     */
    void glassMousePressEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method for handling the release events from the glass.
     */
    void glassMouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method for handling the move events from the glass.
     */
    void glassMouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method for handling situation when some underlying
     * widget grabs mouse.
     */
    void glassUngrabMouseEvent(QEvent *event);

    /*!
     * \brief Method for delivering tap&hold gesture.
     */
    void glassLongTapEvent();

    /*!
     * \brief Method for checking if the event is on the resent list
     * and should pass the glass when it hits it the second time.
     */
    bool checkForResent(QEvent *event);

    /*!
     * \brief Method for posting a captured event back to viewport.
     */
    void resendEvent(QGraphicsSceneMouseEvent *event);

    /*!
     * \brief Method for sending cancel event to viewport.
     */
    void sendCancel(QGraphicsSceneMouseEvent *baseEvent);

    /*!
     * \brief Init method.
     */
    void init();

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiPannableWidget;
#endif

    friend class DuiPannableWidgetGlass;
};

#endif
