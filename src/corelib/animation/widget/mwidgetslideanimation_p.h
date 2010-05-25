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

#ifndef MWIDGETSLIDEANIMATION_P_H
#define MWIDGETSLIDEANIMATION_P_H

#include "mabstractwidgetanimation_p.h"
#include "mwidgetslideanimation.h"
#include <QPointF>

class QPauseAnimation;
class QPropertyAnimation;

class MWidgetSlideAnimationPrivate : public MAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(MWidgetSlideAnimation)
public:
    void init();
    virtual ~MWidgetSlideAnimationPrivate() {}

    void _q_onDelayFinished();
private:
    QPointF originalPos;
    bool played;
    MWidgetSlideAnimation::TransitionDirection direction;
    QPauseAnimation *delay;
    QPropertyAnimation *positionAnimation;
};

#endif
