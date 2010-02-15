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

class QSizeF;
class QRectF;
class QPointF;

class DuiPannableWidget;
class DuiPhysics2DPanningPrivate;

/*!
 * @note Currently the physics works best using units which are not
 * consistent. The panning gestures are interpreted as meters, the
 * mass of the position works best when 1 (kg) and the time delta used
 * in formulas is 1 sec (although the integration is done 50
 * times/sec). This could be re-checked and values could be adjusted
 * to sensible magnitudes (gestures interpreted as millimeters, time delta
 * as 50 times/sec etc.)
 */

class DUI_EXPORT DuiPhysics2DIntegrationStrategy
{
public:

    /*!
     * \brief Destructs an integrator strategy object.
     */
    virtual ~DuiPhysics2DIntegrationStrategy();

    struct IntegrationData {
        bool  pointer;
        qreal pointerSpringK;
        qreal frictionC;
        qreal slideFrictionC;
        qreal borderSpringK;
        qreal borderFrictionC;
    };

    /*!
     * \brief Method used to calculate values for next frame of
     * panning movement.
     */
    virtual void integrate(qreal &position,
                           qreal &velocity,
                           qreal &pointerSpring,
                           qreal &acceleration,
                           qreal rangeStart,
                           qreal rangeEnd,
                           IntegrationData &data);
};

/*!
 * \class DuiPhysics2DPanning
 *
 * \brief This class is an integrator which integrates an
 * object/position in 1D or 2D space using Newtonian physics it is
 * typically used by DuiPannableWidget.
 *
 * The main force in the physics is originating from the pointer via a
 * spring. Additional forces come into play if the position goes
 * outside the range (border springs are applied). There are various
 * friction constants slowing down the movement
 */
class DUI_EXPORT DuiPhysics2DPanning : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Constructs an integrator used by \a parentPannableWidget.
     */
    DuiPhysics2DPanning(DuiPannableWidget *parentPannableWidget);

    /*!
     * \brief Destructs an integrator.
     */
    virtual ~DuiPhysics2DPanning();

    /*!
     * \brief Sets the integration strategy object that will be used
     * during simulation.
     *
     * In case that NULL object is provided, no action is taken and
     * old integration strategy object will still be used.
     * The DuiPhysics2DPanning class takes responsibility
     * for deletion of the object.
     */
    void setIntegrationStrategy(DuiPhysics2DIntegrationStrategy *strategy);

    /*!
     * \brief Returns the currently used integration strategy object.
     */
    DuiPhysics2DIntegrationStrategy *integrationStrategy() const;

    /*!
     * \brief Sets the \a range of the physics.
     *
     * If the range is shrank so that the current position goes to
     * border, the border springs are activated.
     */
    void setRange(const QRectF &range);

    /*!
     * \brief Returns the range of the physics.
     */
    QRectF range() const;

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
     * \brief Returns the current velocity of the physics.
     */
    QPointF velocity() const;

    /*!
     * \brief Tells physics that pointer is pressed.
     */
    void pointerPress(const QPointF &pos);

    /*!
     * \brief Tells physics that pointer is moved.
     *
     * Tenses the pointer spring.
     */
    void pointerMove(const QPointF &pos);

    /*!
     * \brief Tells physics that pointer is release.
     */
    void pointerRelease();

    /*!
     * \brief Sets a physics parameter: K value for the pointer spring.
     */
    void setPointerSpringK(qreal value);

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is
     * down and position is not in border.
     */
    void setFriction(qreal value);

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is up
     * and position is not in border.
     */
    void setSlidingFriction(qreal value);

    /*!
     * \brief Sets a physics parameter: K value for the border spring.
     */
    void setBorderSpringK(qreal value);

    /*!
     * \brief Sets a physics parameter: Friction when a pointer is up
     * and position is in border.
     */
    void setBorderFriction(qreal value);

    /*!
     * \brief Returns the movement status of the integrated position.
     */
    bool inMotion() const;

    /*!
     * \brief Stops the physics.
     */
    void stop();

public Q_SLOTS:

    /*!
     * \brief Integrator for the physics.
     *
     * Called periodically by an internal timer.
     */
    void integrator(int frame);

Q_SIGNALS:
    /*!
     * \brief Signals a change in the \a position of the physics.
     */
    void updatePosition(const QPointF &position);

    /*!
     * \brief Signals that the panning animation has stopped
     */
    void panningStopped();

protected:
    /*!
     * \brief Pointer to private implementation.
     */
    DuiPhysics2DPanningPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiPhysics2DPanning)
    Q_DECLARE_PRIVATE(DuiPhysics2DPanning)

    /*!
     * \brief Starts the physics by setting up a repetitive timer.
     */
    void start();

#ifdef UNIT_TEST
    //! Test unit is defined as a friend of production code to access private members
    friend class Ut_DuiPhysics2DPanning;
#endif
};

#endif
