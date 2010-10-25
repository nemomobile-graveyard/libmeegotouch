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

#ifndef MCANCELEVENT_H
#define MCANCELEVENT_H

#include <QGraphicsSceneEvent>
#include "mexport.h"

class MCancelEventPrivate;

/*!
 * Class instantiated when the result of previously sent event
 * should be cancelled.
 * The cancel event can for example be sent when the user presses
 * the touchscreen and starts to move his/her finger to pan the view.
 * At the moment of first press event the mousePressEvent is sent
 * to the widgets below pannable viewport, but it needs to be
 * "cancelled" if the user wanted to pan the viewport.
 */
class M_CORE_EXPORT MCancelEvent : public QGraphicsSceneEvent
{
public:

    /*!
     * \brief Cancel Event definition.
     */
    static QEvent::Type eventNumber();

    /*!
     * \brief Default constructor, no need for any parameters.
     */
    explicit MCancelEvent();

protected:
    MCancelEventPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MCancelEvent)
};

#endif
