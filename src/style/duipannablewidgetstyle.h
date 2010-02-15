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

#ifndef DUIPANNABLEWIDGETSTYLE_H
#define DUIPANNABLEWIDGETSTYLE_H

#include <duiwidgetstyle.h>

/*!
    \class DuiPannableWidgetStyle
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

    \sa DuiPannableWidget DuiWidgetStyle
*/
class DUI_EXPORT DuiPannableWidgetStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE_INTERNAL(DuiPannableWidgetStyle)

    DUI_STYLE_ATTRIBUTE(qreal, panThreshold,     PanThreshold)
    DUI_STYLE_ATTRIBUTE(qreal, pointerSpringK,   PointerSpringK)
    DUI_STYLE_ATTRIBUTE(qreal, frictionC,        FrictionC)
    DUI_STYLE_ATTRIBUTE(qreal, slidingFrictionC, SlidingFrictionC)
    DUI_STYLE_ATTRIBUTE(qreal, borderSpringK,    BorderSpringK)
    DUI_STYLE_ATTRIBUTE(qreal, borderFrictionC,  BorderFrictionC)
    DUI_STYLE_ATTRIBUTE(qreal, panClickThreshold, PanClickThreshold)
};

class DUI_EXPORT DuiPannableWidgetStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER_INTERNAL(DuiPannableWidgetStyle)
};

#endif
