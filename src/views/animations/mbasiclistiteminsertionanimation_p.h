/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MBASICLISTITEMINSERTIONANIMATION_P_H
#define MBASICLISTITEMINSERTIONANIMATION_P_H

#include <QObject>
#include <QPointF>
#include <QHash>
#include <QWeakPointer>

class MBasicListItemInsertionAnimation;
class MWidget;

class QPauseAnimation;
class QPropertyAnimation;

class MBasicListItemInsertionAnimationPrivate
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
    QHash<QWeakPointer<MWidget>*, TargetDefaultProperties> targets;

public:
    MBasicListItemInsertionAnimationPrivate();
    virtual ~MBasicListItemInsertionAnimationPrivate();

    void _q_resetAnimation();

    void appendTargetWidget(MWidget *cell);
    void resetTargetWidgets();

    QPropertyAnimation *createOpacityAnimation(MWidget *cell);
    QPropertyAnimation *createScaleAnimation(MWidget *cell);
    QPropertyAnimation *createPositionAnimation(MWidget *cell, const QPointF &targetPos);

    QPauseAnimation *createDelayAnimation(int delay);

private:
    MBasicListItemInsertionAnimation *q_ptr;
    Q_DECLARE_PUBLIC(MBasicListItemInsertionAnimation)
};

#endif // MBASICLISTITEMINSERTIONANIMATION_P_H
