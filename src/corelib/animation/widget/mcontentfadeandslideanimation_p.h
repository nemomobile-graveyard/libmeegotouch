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

#ifndef MCONTENTFADEANDSLIDEANIMATION_P_H
#define MCONTENTFADEANDSLIDEANIMATION_P_H

#include "mabstractwidgetanimation_p.h"

class MContentFadeAndSlideAnimation;
class QParallelAnimationGroup;
class QSequentialAnimationGroup;
class QPropertyAnimation;
class QPauseAnimation;

class SnapshotItem : public QGraphicsWidget
{
public:
    void updateSnapshot(QGraphicsWidget* target);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPixmap pixmap;
};

class MContentFadeAndSlideAnimationPrivate : public MAbstractWidgetAnimationPrivate
{
    Q_DECLARE_PUBLIC(MContentFadeAndSlideAnimation)
public:
    MContentFadeAndSlideAnimationPrivate();

    void init();
    void findContentItem();

    QPointer<SnapshotItem> snapshotItem;
    QPointer<QGraphicsWidget> contentItem;

    QParallelAnimationGroup* previous;
    QPropertyAnimation* fadeOut;
    QPropertyAnimation* slideOut;
    QParallelAnimationGroup* current;
    QPropertyAnimation* fadeIn;
    QPropertyAnimation* slideIn;
    QSequentialAnimationGroup* currentWithDelay;
    QPauseAnimation* delay;
};

#endif
