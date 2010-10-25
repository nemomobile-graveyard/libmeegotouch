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

#ifndef MPHYSICS2DPANNING_H
#define MPHYSICS2DPANNING_H

#include "mexport.h"

#include <QObject>

#include <QRectF>
#include <QPointF>
#include <QSizeF>

class MPhysics2DPanningPrivate;

/*!
 * \class MPhysics2DPanning
 *
 * \brief This class is an integrator which integrates an
 * object/position in 1D or 2D space using Newtonian physics it is
 * typically used by MPannableWidget.
 *
 * The user can change the behaviour of the kinetic panning that the
 * pannable widget is providing to some other algorithm. In order to
 * do that, a new class inheriting from MPhysics2DPanning should be
 * created.
 *
 * The main force in the physics is originating from the pointer via a
 * spring. Additional forces come into play if the position goes
 * outside the range (border springs are applied). There are various
 * friction constants slowing down the movement
 *
 * The kinetic action of the springs can be disabled with setEnabled()
 * methods. In case the physics is disabled, the viewport will not
 * keep panning after the pointer is released.
 *
 * By default, the kinetic action of the springs is enabled.
 */
class M_CORE_EXPORT MPhysics2DPanning : public QObject
{
    Q_OBJECT

    //! \brief Current enabled/disabled state.
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
    //! \brief Current panning orientations
    Q_PROPERTY(Qt::Orientations panDirection READ panDirection WRITE setPanDirection)
    //! \brief Current panning range
    Q_PROPERTY(QRectF range READ range WRITE setRange)
    //! \brief Current pointer spring K constant value
    Q_PROPERTY(qreal pointerSpringK READ pointerSpringK WRITE setPointerSpringK)
    //! \brief Current friction constant value
    Q_PROPERTY(qreal friction READ friction WRITE setFriction)
    //! \brief Current sliding friction constant value
    Q_PROPERTY(qreal slidingFriction READ slidingFriction WRITE setSlidingFriction)
    //! \brief Current border spring K constant value
    Q_PROPERTY(qreal borderSpringK READ borderSpringK WRITE setBorderSpringK)
    //! \brief Current border friction constant value
    Q_PROPERTY(qreal borderFriction READ borderFriction WRITE setBorderFriction)
    //! \brief Current position value
    Q_PROPERTY(QPointF position READ position WRITE setPosition)
    //! \brief Current maximum speed value
    Q_PROPERTY(qreal maximumVelocity READ maximumVelocity WRITE setMaximumVelocity)

public:

    /*!
     * \brief Constructs an integrator.
     */
    MPhysics2DPanning(QObject *parent);

    /*!
     * \brief Destructs an integrator.
     */
    virtual ~MPhysics2DPanning();

    /*!
     * \brief Returns the maximum speed of this simulation.
     */
    qreal maximumVelocity() const;

    /*!
     * \brief Sets the maximum speed.
     * \param speed The maximum speed this simulation can reach.
     */
    void setMaximumVelocity(qreal velocity);

    /*!
     * \brief Returns the enabled state of the physics engine.
     */
    bool enabled() const;

    /*!
     * \brief Sets the enabled/disabled state of the physics engine.
     * If the physics is disabled, the integration algorithm is not
     * started when the user moves the pointer on the screen.
     *
     * When disabled, the viewport will still be scrollable but it
     * will not keep panning after the pointer is released.
     *
     * By default, the physics engine is enabled.
     */
    void setEnabled(bool enabled);

    /*!
     * \brief Returns the allowed direction of panning movement.
     */
    Qt::Orientations panDirection() const;

    /*!
     * \brief Sets the allowed orientations of panning movement.
     */
    void setPanDirection(Qt::Orientations direction);

    /*!
     * \brief Returns the range of the physics.
     */
    QRectF range() const;

    /*!
     * \brief Sets the \a range of the physics.
     *
     * If the range is shrank so that the current position goes to
     * border, the border springs are activated.
     */
    void setRange(const QRectF &range);

    /*!
     * \brief Returns the physics parameter: K value for the pointer spring.
     */
    qreal pointerSpringK() const;

    /*!
     * \brief Sets a physics parameter: K value for the pointer spring.
     */
    void setPointerSpringK(qreal value);

    /*!
     * \brief Returns the physics parameter: friction when a pointer is down
     * and position is not in the border.
     */
    qreal friction() const;

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is
     * down and position is not in the border.
     */
    void setFriction(qreal value);

    /*!
     * \brief Returns the physics parameter: friction when a pointer is up
     * and position is not in the border.
     */
    qreal slidingFriction() const;

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is up
     * and position is not in border.
     */
    void setSlidingFriction(qreal value);

    /*!
     * \brief Returns the physics parameter: K value for the border spring.
     */
    qreal borderSpringK() const;

    /*!
     * \brief Sets a physics parameter: K value for the border spring.
     */
    void setBorderSpringK(qreal value);

    /*!
     * \brief Returns the physics parameter: Friction when a pointer is up
     * and position is in border.
     */
    qreal borderFriction() const;

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is up
     * and position is in border.
     */
    void setBorderFriction(qreal value);

    /*!
     * \brief Sets the \a position of the physics.
     *
     * If the new position is in the border, the border springs are
     * activated.
     */
    void setPosition(const QPointF &position);

    /*!
     * \brief Returns the current position of the physics.
     */
    QPointF position() const;

    /*!
     * \brief Returns the movement status of the integrated position.
     */
    bool inMotion() const;

    /*!
     * \brief Returns the current velocity of the physics.
     */
    QPointF velocity() const;

    /*!
     * \brief Tells physics that pointer was pressed.
     */
    virtual void pointerPress(const QPointF &pos);

    /*!
     * \brief Tells physics that pointer has moved.
     */
    virtual void pointerMove(const QPointF &pos);

    /*!
     * \brief Tells physics that pointer was released.
     */
    virtual void pointerRelease();

    /*!
     * \brief Stops the integration algorithm. The algorithm will
     * be restarted when pointer will be pressed again.
     */
    void stop();

Q_SIGNALS:
    /*!
     * \brief Signals a change in the \a position of the physics.
     */
    void positionChanged(const QPointF &position);

    /*!
     * \brief Signals that the panning movement has stopped.
     */
    void panningStopped();

protected:

    /*!
     * \brief Starts the physics algorithm. The engine will now
     * call the integrator() method periodically.
     */
    void start();

    /*!
     * \brief Single axis integration algorithm implementation.
     *
     * This method will be called for each integration axis that
     * is set by setPanDirection() method.
     * \a orientation - currently integrated axis.
     * \a position - position calculated in previous step of integration.
     * \a velocity - velocity calculated in previous step of integration.
     * \a acceleration - acceleration calculated in previous step of integration.
     * \a pointerDifference - distance between current mouse position and
     * previously received mouse position.
     * \a pointerPressed - state of the pointer.
     */
    virtual void integrateAxis( Qt::Orientation orientation,
                                qreal &position,
                                qreal &velocity,
                                qreal &acceleration,
                                qreal &pointerDifference,
                                bool pointerPressed
                                );

private:

    /*!
     * \brief Pointer to private implementation.
     */
    MPhysics2DPanningPrivate *const d_ptr;

    Q_DISABLE_COPY(MPhysics2DPanning)
    Q_DECLARE_PRIVATE(MPhysics2DPanning)
    Q_PRIVATE_SLOT(d_func(),void _q_integrator(QVariant))

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_MPhysics2DPanning;
#endif
};

#endif
