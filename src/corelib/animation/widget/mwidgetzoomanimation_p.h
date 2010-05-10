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

#ifndef MWIDGETZOOMANIMATION_P_H
#define MWIDGETZOOMANIMATION_P_H

#include "mabstractwidgetanimation_p.h"
#include "mwidgetzoomanimation.h"

class QPropertyAnimation;
class QPauseAnimation;

class MWidgetZoomAnimationPrivate : MAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(MWidgetZoomAnimation)
public:
    void init();
    QPointF setupPositionAnimation(const QPointF &widgetPos);

    QPauseAnimation *startDelay;
    QPauseAnimation *opacityAnimationDelay;

    MWidgetZoomAnimation::TransitionDirection direction;
    QPointF origin;
    QPropertyAnimation *opacityAnimation;
    QPropertyAnimation *positionAnimation;
    QPropertyAnimation *scaleAnimation;
};

#endif // MWIDGETZOOMANIMATION_P_H
