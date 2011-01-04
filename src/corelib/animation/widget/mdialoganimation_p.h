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

#ifndef MDIALOGANIMATION_P_H
#define MDIALOGANIMATION_P_H

#include "mabstractwidgetanimation_p.h"
#include "mdialoganimation.h"

class QPropertyAnimation;
class QPauseAnimation;

class MDialogAnimationPrivate : public MAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(MDialogAnimation)
public:
    MDialogAnimationPrivate();
    void init();

    void findComponentsForAnimation();
    QGraphicsWidget* findDialogBox(QGraphicsItem *parentItem);
    virtual void setupDurations();
    virtual void setupEasingCurves();

    void setupTitleBarAnimation();
    void setupContentsViewportAnimation();
    void setupButtonBoxAnimation();

    virtual QPointF setupPositionAnimation(const QPointF &widgetPos);

    MDialogAnimation::TransitionDirection direction;
    QPointF origin;

    QSequentialAnimationGroup *delayedTitleBarAnimation;
    QSequentialAnimationGroup *delayedContentsViewportAnimation;
    QSequentialAnimationGroup *delayedButtonBoxAnimation;

    QPauseAnimation *titleBarAnimationDelay;
    QPauseAnimation *contentsViewportAnimationDelay;
    QPauseAnimation *buttonBoxAnimationDelay;

    QPropertyAnimation *titleBarOpacityAnimation;
    QPropertyAnimation *titleBarPositionAnimation;
    QPropertyAnimation *contentsViewportOpacityAnimation;
    QPropertyAnimation *contentsViewportScaleAnimation;
    QPropertyAnimation *contentsViewportPositionAnimation;
    QPropertyAnimation *buttonBoxOpacityAnimation;
    QPropertyAnimation *buttonBoxPositionAnimation;

    QGraphicsWidget *dialogBox;
    MWidgetController *titleBar;
    MWidgetController *contentsViewport;
    MWidgetController *buttonBox;

    static const QString DialogBoxObjectName;
};

#endif // MDIALOGANIMATION_P_H
