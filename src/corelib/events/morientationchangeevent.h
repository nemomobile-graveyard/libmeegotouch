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

#ifndef MORIENTATIONCHANGEEVENT_H
#define MORIENTATIONCHANGEEVENT_H

#include <QGraphicsSceneEvent>
#include "mexport.h"
#include "mnamespace.h"

class MOrientationChangeEventPrivate;

/*!
 * \brief This class contains event parameters for orientation change events.
 *
 * The orientation change event occurs whenever the viewport changes its
 * orientation between landscape and portrait (note that it doesn't occur
 * when changing angle e.g. from 0 directly to 180 degrees). It is also sent to MApplicationPage
 * when it is about to be shown for the first time - MApplicationPage::appear().
 *
 * It is used to notify widgets about the orientation change. It can be asked
 * about the new orientation of the viewport. You can handle this event
 * either by catching it in QGraphicsItem::event() or reimplementing
 * MWidget::orientationChangeEvent().
 *
 * \sa MWidget::orientationChangeEvent()
 */
class M_CORE_EXPORT MOrientationChangeEvent : public QGraphicsSceneEvent
{
public:

    /*!
     * Orientation change event definition. Used to identify the type of the event.
     */
    static QEvent::Type eventNumber();

    /*!
     * Creates a new orientation change event with the orientation specified by \a newOrientation.
     */
    explicit MOrientationChangeEvent(M::Orientation newOrientation = M::Landscape);

    /*!
     * Destructor
     */
    virtual ~MOrientationChangeEvent();

    /*!
     * Holds the new orientation of the viewport.
     */
    M::Orientation orientation() const;

protected:
    MOrientationChangeEventPrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(MOrientationChangeEvent)
    Q_DISABLE_COPY(MOrientationChangeEvent)
};

#endif
