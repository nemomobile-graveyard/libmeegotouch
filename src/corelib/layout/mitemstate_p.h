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

#ifndef MITEMSTATE_P_H
#define MITEMSTATE_P_H

#include <QRectF>
#include "mitemstate.h"

class QGraphicsLayoutItem;

class MItemStatePrivate
{
public:
    MItemStatePrivate(QGraphicsWidget *item);

    /*!
     * \brief The item to layout.
     */
    QGraphicsWidget *item;

    /*!
     * \brief The position of the item in the layout.
     *
     * This position is the one the item had before the currently
     * active change was issued.
     */
    QRectF sourceGeometry;

    /*!
     * \brief The position of the item in the layout.
     *
     * This is the position the item should have once all in-flight
     * operations are finished.
     */
    QRectF targetGeometry;

    qreal sourceOpacity;
    qreal targetOpacity;

    /*!
     * \brief State of the item
     */
    MItemState::Flags flags;

    /*!
     * \brief Current progress of animation of the geometry
     * Between 0 and 1.  1 indicating finished.
     */
    qreal geometryProgress;
    /*!
     * \brief Current progress of animation of the opacity
     * Between 0 and 1.  1 indicating finished.
     */
    qreal opacityProgress;
    /*! \brief True is the animation has completed */
    bool isAnimationDone;
};
#endif // Header Guard

