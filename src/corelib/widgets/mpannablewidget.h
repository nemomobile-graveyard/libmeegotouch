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

#ifndef MPANNABLEWIDGET_H
#define MPANNABLEWIDGET_H

#include "mwidgetcontroller.h"
#include "mphysics2dpanning.h"
#include "mpannablewidgetmodel.h"

class QTimerEvent;
class QGraphicsSceneMouseEvent;
class QPanGesture;
class MPannableWidgetPrivate;
class MOnDisplayChangeEvent;

/*!
 * \class MPannableWidget
 * \brief Base class which adds support for panning gesture
 *
 * MPannableWidget creates a glass on top of itself and using the
 * glass, intercepts events coming from QGraphicsScene onto its area
 * and interprets them. If the events form a panning gesture, a
 * physics class is used to create a natural Newtonian movement in the
 * direction of panning. Panning position is provided as an
 * overridable slot updatePosition(). If the events don't form a
 * panning gesture, they are forwarded underneath the glass.
 *
 * In order to change the current position of the viewport the user
 * can call setPosition() method. When this is done, integration engine
 * is started and in case that the position is outside specified bounds,
 * the physics engine will start spring action to move the viewport to
 * the allowed borders.
 *
 * The physics engine object can be obtained by calling physics() method.
 *
 * \sa MPhysics2DPanning, MPannableViewport
 */
class M_CORE_EXPORT MPannableWidget : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MPannableWidget)

    //! \brief Vertical panning policy status
    Q_PROPERTY(PanningPolicy verticalPanningPolicy READ verticalPanningPolicy WRITE setVerticalPanningPolicy)
    //! \brief Horizontal panning policy status
    Q_PROPERTY(PanningPolicy horizontalPanningPolicy READ horizontalPanningPolicy WRITE setHorizontalPanningPolicy)
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
     * This enum contains possible values of a panning policy.
     *
     * \li PanningAlwaysOn - the pannable widget will always react to pan gesture
     * \li PanningAlwaysOff - the pannable widget will never react to pan gesture
     * \li PanningAsNeeded - the pannable widget will only react to pan gesture if the panned
     *     widget is bigger than the viewport.
     *
     * \sa setHorizontalPanningPolicy, setVerticalPanningPolicy.
     */
    enum PanningPolicy {
        PanningAlwaysOn,
        PanningAlwaysOff,
        PanningAsNeeded
    };

    /*!
     * \brief Constructs a pannable widget with a \a parent.
     */
    MPannableWidget(QGraphicsItem *parent = 0);

    /*!
     * \brief Destroys the pannable widget.
     */
    virtual ~MPannableWidget();

    /*!
     * \brief Returns the physics class instance used in
     * panning.
     */
    MPhysics2DPanning *physics() const;

    /*!
     * \brief Sets the physics engine object used when the widget is
     * panning the view.
     *
     * This method can be used to modify the behaviour of the widget
     * so that the kinetic panning effect is different than default.
     *
     * The MPannableWidget will take ownership of the physics engine
     * object.
     */
    void setPhysics(MPhysics2DPanning *physics);

    /*!
     * \brief Sets the vertical panning policy of the pannable widget.
     *
     * This method allows for finegrained control over the panning
     * functionality in the MPannableWidget. The policy argument
     * will define the result of a vertical pan gesture executed
     * over pannable widget.
     *
     * This method does not reset horizontal panning policy.
     *
     * \sa verticalPanningPolicy and setPanDirection.
     */
    void setVerticalPanningPolicy(PanningPolicy policy);

    /*!
     * \brief Sets the horizontal panning policy of the pannable widget.
     *
     * This method allows for finegrained control over the panning
     * functionality in the MPannableWidget. The policy argument
     * will define the result of a horizontal pan gesture executed
     * over pannable widget.
     *
     * This method does not reset vertical panning policy.
     *
     * \sa horizontalPanningPolicy and setPanDirection.
     */
    void setHorizontalPanningPolicy(PanningPolicy policy);

    /*!
     * \brief Returns the vertical interactivity policy of the pannable widget.
     */
    PanningPolicy verticalPanningPolicy() const;

    /*!
     * \brief Returns the interactivity policy of the pannable widget.
     */
    PanningPolicy horizontalPanningPolicy() const;

    /*!
     * \brief Sets the \a range of panning.
     *
     * \sa range.
     */
    virtual void setRange(const QRectF &range);

    /*!
     * \brief Returns the range of panning.
     *
     * The range of panning defines the allowed positions of
     * the viewport over the panned widget. The panning range
     * is smaller than the panned widget size by the size of
     * the viewport.
     *
     * \note When range is zero along some axis, user can still make a
     * panning gesture on that direction but the position snaps back
     * to 0.0. If the range is shrank so that the current position
     * goes to border, the border springs are activated.
     *
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
     * This method allows enabling or disabling panning directions
     * of the pannable widget. It is an equivalent of methods
     * for setting panning policies: enabling a panning direction
     * has the same effect as setting PanningAlwaysOn policy for that
     * direction.Disabling a panning direction is exactly the same
     * as setting PanningAlwaysOff policy.
     *
     * The accepted values are Qt::Horizontal and/or Qt::Vertical.
     *
     * \sa setVerticalPanningPolicy and setHorizontalPanningPolicy.
     */
    virtual void setPanDirection(const Qt::Orientations &panDirection);

    /*!
     * \brief Returns the enabled panning directions.
     *
     * A panning direction is enabled if the policy of panning for
     * that direction is either PanningAlwaysOn or PanningAsNeeded.
     *
     * By default, the panning is enabled in Qt::Vertical direction.
     *
     * \deprecated Please use const version instead. Since 0.20.50
     */
    Qt::Orientations panDirection();

    /*!
     * \brief Returns the enabled panning directions.
     *
     * A panning direction is enabled if the policy of panning for
     * that direction is either PanningAlwaysOn or PanningAsNeeded.
     *
     * By default, the panning is enabled in Qt::Vertical direction.
     */
    Qt::Orientations panDirection() const;

    /*!
     * \brief Deprecated since 0.20
     */
    void setPanThreshold(qreal value);

    /*!
     * \brief Deprecated since 0.20.
     */
    qreal panThreshold();

    //! \reimp
    void setEnabled(bool enabled);
    bool isEnabled();
    //! \reimp_end

public Q_SLOTS:
    /*!
     * \brief Virtual slot for receiving position changes from
     * physics.
     */
    virtual void updatePosition(const QPointF &position);

Q_SIGNALS:

    /*!
     * \brief This signal is emitted when the position of the viewport changes.
     */
    void positionChanged(const QPointF &position);

    /*!
     * \brief When panned range changes this signal is emitted.
     */
    void rangeChanged(const QRectF &range);

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
    MPannableWidget(MPannableWidgetPrivate *dd, MPannableWidgetModel *model,
                      QGraphicsItem *parent);

    //! \reimp
    virtual void cancelEvent(MCancelEvent *event);
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);
    virtual void panGestureEvent(QGestureEvent *event, QPanGesture* state);
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture);
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MPannableWidget)
    Q_DECLARE_PRIVATE(MPannableWidget)

    Q_PRIVATE_SLOT(d_func(), void _q_resetPhysics())

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MPannableWidget;
#endif

    friend class MPannableWidgetGlass;

    // We are using a hack which will allow reimplementing the panGestureEvent
    // method in the pannable widget without recompilation of all client applications.
    // This needs to be deleted when API unfreeze will finally happen.
    friend class MWidgetController;
};

#endif
