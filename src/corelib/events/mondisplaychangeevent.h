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

#ifndef MONDISPLAYCHANGEEVENT_H
#define MONDISPLAYCHANGEEVENT_H

#include <QGraphicsSceneEvent>
#include "mexport.h"

class MOnDisplayChangeEventPrivate;

/*!
 * This event is sent to a MWidget so that it can notify subscribers
 * about changes on its presence on display. The event can be triggered by
 * e.g. panning or switching a MApplicationPage or obscuring the MApplicationWindow
 * with another window. Note that this differs from Qt's explicit hide()/show()
 * semantics. MWidget forwards this event to its children.
 */
class M_CORE_EXPORT MOnDisplayChangeEvent : public QGraphicsSceneEvent
{
public:

    /*!
     * \brief Possible visibility states.
     */
    enum State {FullyOnDisplay = 0, /*!< Widget is definitely fully on display  */
                FullyOffDisplay,    /*!< Widget is definitely fully off display  */
                MustBeResolved,      /*!< Widget must use viewRect to verify its presence on the display,
                                          as well as of all its children. */
                PartiallyOnDisplay /*!< Widget is partially present on the display, according to its
                                        bounding rectangle. Widget may still use viewRect to get a more
                                        precise (and therefore more computationally expensive) assertion
                                        by comparing it against its shape, for instance. */
               };

    /*!
     * \brief Cancel Event definition.
     */
    static QEvent::Type eventNumber();

    /*!
     * \brief Constructor
     * \param newState new visibility state
     * \param Viewport rectangle. Used to resolve the visibility if
     *        widget may be only partially on the display
     */
    MOnDisplayChangeEvent(State state, const QRectF &viewRect);

    /*!
     * \brief Convenience constructor
     * \param visible true : FullyOnDisplay, false : FullyOffDisplay
     * \param Viewport rectangle. Used to resolve the visibility if
     *        widget may be only partially on the display
     */
    MOnDisplayChangeEvent(bool visible, const QRectF &viewRect);

    //! Destructor
    virtual ~MOnDisplayChangeEvent();

    /*!
     * \brief Return the state.
     */
    State state() const;

    /*!
     * \brief Return the viewport rectangle.
     */
    QRectF viewRect() const;

protected:
    MOnDisplayChangeEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(MOnDisplayChangeEvent)
    Q_DISABLE_COPY(MOnDisplayChangeEvent)
};

#endif // MONDISPLAYCHANGEEVENT_H
