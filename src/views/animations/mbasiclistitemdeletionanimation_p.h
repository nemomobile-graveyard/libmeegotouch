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


#ifndef MBASICLISTITEMDELETIONANIMATION_P_H
#define MBASICLISTITEMDELETIONANIMATION_P_H

#include <QObject>
#include <QPointF>
#include <QHash>

class MBasicListItemDeletionAnimation;
class MWidget;

class QPauseAnimation;
class QPropertyAnimation;

class MBasicListItemDeletionAnimationPrivate
{
private:
    struct TargetDefaultProperties {
        TargetDefaultProperties(qreal zValue, qreal opacity, qreal scale, bool visible) : zValue(zValue), opacity(opacity), scale(scale), visible(visible) {}
        TargetDefaultProperties() : zValue(0), opacity(1.f), scale(1.f), visible(true) {}
        TargetDefaultProperties(const TargetDefaultProperties &copy) : zValue(copy.zValue), opacity(copy.opacity), scale(copy.scale), visible(copy.visible) {}
        qreal zValue;
        qreal opacity;
        qreal scale;
        bool visible;
    };
    QHash<MWidget*, TargetDefaultProperties> targets;

public:
    MBasicListItemDeletionAnimationPrivate();
    virtual ~MBasicListItemDeletionAnimationPrivate();

    void _q_resetAnimation();

    void appendTargetWidget(MWidget *cell);
    void resetTargetWidgets();

    QPropertyAnimation *createOpacityAnimation(MWidget *cell);
    QPropertyAnimation *createScaleAnimation(MWidget *cell);
    QPropertyAnimation *createPositionAnimation(MWidget *cell, const QPointF &targetPos);

    QPauseAnimation *createDelayAnimation(int delay);

private:
    MBasicListItemDeletionAnimation *q_ptr;
    Q_DECLARE_PUBLIC(MBasicListItemDeletionAnimation)
};

#endif // MBASICLISTITEMDELETIONANIMATION_P_H
