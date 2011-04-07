/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSAREASLIDEANIMATION_P_H
#define MSTATUSAREASLIDEANIMATION_P_H

#include "mabstractwidgetanimation_p.h"
#include "mstatusareaslideanimation.h"
#include <QPointF>
#include <QWeakPointer>

class QPauseAnimation;
class QPropertyAnimation;

class MStatusAreaSlideAnimationPrivate : public MAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(MStatusAreaSlideAnimation)
public:
    void init();
    virtual ~MStatusAreaSlideAnimationPrivate() {}

    void _q_onDelayFinished();
    void findStatusBar(QGraphicsItem* parentItem);
    void restoreStatusBarOriginalState();
private:
    QPointF originalPos;
    bool played;
    MStatusAreaSlideAnimation::TransitionDirection direction;

    QPauseAnimation* widgetAnimationDelay;
    QPropertyAnimation* widgetPosAnimation;
    QSequentialAnimationGroup* widgetAnimation;

    QPauseAnimation* statusBarAnimationDelay;
    QPropertyAnimation* statusBarPosAnimation;
    QSequentialAnimationGroup* statusBarAnimation;

    QWeakPointer<MWidgetController> statusBarPointer;
};

#endif
