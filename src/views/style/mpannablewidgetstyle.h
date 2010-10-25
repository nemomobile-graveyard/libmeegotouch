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

#ifndef MPANNABLEWIDGETSTYLE_H
#define MPANNABLEWIDGETSTYLE_H

#include <mwidgetstyle.h>

/*!
    \class MPannableWidgetStyle
    \brief Style class for standard pannable widgets.

    The pannable widget provides following styling attributes:

      \li pan-threshold - defines threshold distance used to recognize
          panning gesture.
      \li pan-click-threshold - defines maximum speed of the panning that
          will be stopped by clicking. If the speed is lower, mousePress event
          will be relayed to panned widget.
      \li pointer-spring-k - k constant of the spring model used when the user
          is panning the view.
      \li friction-c - friction constant used when the user is panning the view.
      \li sliding-friction-c - friction constant used when the user released
          the finger and the view is moved by physics.
      \li border-spring-k - friction constant used when the view was moved
          beyond normal range by user and needs to return to the correct bounds.
      \li border-friction-c - friction constant used when the view was moved
          beyond normal range by user and needs to return to the correct bounds.
      \li maximum-velocity - maximum speed that the panning of the pannable viewport
          can reach.

    \sa MPannableWidget MWidgetStyle
*/
class M_VIEWS_EXPORT MPannableWidgetStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MPannableWidgetStyle)

    M_STYLE_ATTRIBUTE(qreal, panThreshold,     PanThreshold)
    M_STYLE_ATTRIBUTE(qreal, pointerSpringK,   PointerSpringK)
    M_STYLE_ATTRIBUTE(qreal, frictionC,        FrictionC)
    M_STYLE_ATTRIBUTE(qreal, slidingFrictionC, SlidingFrictionC)
    M_STYLE_ATTRIBUTE(qreal, borderSpringK,    BorderSpringK)
    M_STYLE_ATTRIBUTE(qreal, borderFrictionC,  BorderFrictionC)
    M_STYLE_ATTRIBUTE(qreal, panClickThreshold, PanClickThreshold)

    M_STYLE_ATTRIBUTE(qreal, maximumVelocity, MaximumVelocity)
};

class M_VIEWS_EXPORT MPannableWidgetStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MPannableWidgetStyle)
};

#endif
