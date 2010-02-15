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

#ifndef DUIONDISPLAYCHANGEEVENT_H
#define DUIONDISPLAYCHANGEEVENT_H

#include <QGraphicsSceneEvent>
#include "duiexport.h"

class DuiOnDisplayChangeEventPrivate;

/*!
 * This event is sent to a DuiWidget so that it can notify subscribers
 * about changes on its presence on display. The event can be triggered by
 * e.g. panning or switching a DuiApplicationPage or obscuring the DuiApplicationWindow
 * with another window. Note that this differs from Qt's explicit hide()/show()
 * semantics. DuiWidget forwards this event to its children.
 */
class DUI_EXPORT DuiOnDisplayChangeEvent : public QGraphicsSceneEvent
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
    DuiOnDisplayChangeEvent(State state, const QRectF &viewRect);

    /*!
     * \brief Convenience constructor
     * \param visible true : FullyOnDisplay, false : FullyOffDisplay
     * \param Viewport rectangle. Used to resolve the visibility if
     *        widget may be only partially on the display
     */
    DuiOnDisplayChangeEvent(bool visible, const QRectF &viewRect);

    //! Destructor
    virtual ~DuiOnDisplayChangeEvent();

    /*!
     * \brief Return the state.
     */
    State state() const;

    /*!
     * \brief Return the viewport rectangle.
     */
    QRectF viewRect() const;

protected:
    DuiOnDisplayChangeEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(DuiOnDisplayChangeEvent)
    Q_DISABLE_COPY(DuiOnDisplayChangeEvent)
};

#endif // DUIONDISPLAYCHANGEEVENT_H
