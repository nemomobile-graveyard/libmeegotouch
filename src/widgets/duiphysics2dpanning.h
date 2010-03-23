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

#ifndef DUIPHYSICS2DPANNING_H
#define DUIPHYSICS2DPANNING_H

#include "duiexport.h"

#include <QObject>

#include <QRectF>
#include <QPointF>
#include <QSizeF>

class DuiPhysics2DPanningPrivate;

/*!
 * \class DuiPhysics2DPanning
 *
 * \brief This class is an integrator which integrates an
 * object/position in 1D or 2D space using Newtonian physics it is
 * typically used by DuiPannableWidget.
 *
 * The user can change the behaviour of the kinetic panning that the
 * pannable widget is providing to some other algorithm. In order to
 * do that, a new class inheriting from DuiPhysics2DPanning should be
 * created.
 *
 * The main force in the physics is originating from the pointer via a
 * spring. Additional forces come into play if the position goes
 * outside the range (border springs are applied). There are various
 * friction constants slowing down the movement
 */
class DUI_EXPORT DuiPhysics2DPanning : public QObject
{
    Q_OBJECT

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

public:

    /*!
     * \brief Constructs an integrator.
     */
    DuiPhysics2DPanning(QObject *parent);

    /*!
     * \brief Destructs an integrator.
     */
    virtual ~DuiPhysics2DPanning();

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
    virtual void setPosition(const QPointF &position);

    /*!
     * \brief Returns the current position of the physics.
     */
    virtual QPointF position() const;

    /*!
     * \brief Returns the movement status of the integrated position.
     */
    virtual bool inMotion() const;

    /*!
     * \brief Returns the current velocity of the physics.
     */
    virtual QPointF velocity() const;

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
    virtual void stop();

Q_SIGNALS:
    /*!
     * \brief Signals a change in the \a position of the physics.
     */
    void updatePosition(const QPointF &position);

    /*!
     * \brief Signals that the panning movement has stopped.
     */
    void panningStopped();

protected Q_SLOTS:

    /*!
     * \brief Integrator for the physics.
     *
     * Called periodically by an internal timer.
     */
    virtual void integrator(int frame);

protected:

    /*!
     * \brief Starts the physics algorithm. The engine will now
     * call the integrator() method periodically.
     */
    virtual void start();

private:

    /*!
     * \brief Pointer to private implementation.
     */
    DuiPhysics2DPanningPrivate *const d_ptr;

    Q_DISABLE_COPY(DuiPhysics2DPanning)
    Q_DECLARE_PRIVATE(DuiPhysics2DPanning)

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiPhysics2DPanning;
#endif
};

#endif
