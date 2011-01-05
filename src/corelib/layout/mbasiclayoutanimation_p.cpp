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

#include "mbasiclayoutanimation_p.h"
#include "mlayout_p.h"

MBasicLayoutAnimationPrivate::MBasicLayoutAnimationPrivate()
{
    recordedTimeSinceLastUpdate = 0;
}

MBasicLayoutAnimationPrivate::~MBasicLayoutAnimationPrivate()
{ }

void MBasicLayoutAnimationPrivate::tick()
{
    Q_Q(MBasicLayoutAnimation);
    recordedTimeSinceLastUpdate = timeSinceLastUpdate.restart();
    bool layout_is_done(true);
    for(int i = states.count() - 1; i >= 0; --i) {
        MItemState &state = states[i];
        if (state.isAnimationDone())
            continue;
        animate(state);

        if (state.isAnimationDone()) {
            if(state.isSet(MItemState::STATE_FLAG_TO_BE_DELETED))
                q->layout()->removeAt(i);
        } else {
            layout_is_done = false;
        }

    }
    if (layout_is_done)
        q->stop();
}
void MBasicLayoutAnimationPrivate::animate(MItemState &item)
{
    Q_Q(MBasicLayoutAnimation);

    qreal delta_progress = recordedTimeSinceLastUpdate / q->style()->duration(); //Both times are in milliseconds

    //First animate the geometry if needed
    if (item.translateProgress() != 1) {
        //animate the geometry
        qreal new_geometry_progress = item.translateProgress() + delta_progress;
        if (new_geometry_progress >= 1.0) {
            item.setTransformProgress(1.0);
            item.item()->setTransform( QTransform() );
        } else {
            item.setTransformProgress(new_geometry_progress);
            qreal x_translation_animation_value = q->style()->xTranslationEasingCurve().valueForProgress(new_geometry_progress);
            qreal y_translation_animation_value = q->style()->yTranslationEasingCurve().valueForProgress(new_geometry_progress);
            qreal x_offset = item.sourceTranslate().x() * (1 - x_translation_animation_value);
            qreal y_offset = item.sourceTranslate().y() * (1 - y_translation_animation_value);
            item.item()->setTransform( QTransform::fromTranslate(x_offset, y_offset) );
        }
    }

    //Then animate the opacity if needed
    if (item.opacityProgress() != 1 && item.targetOpacity() != -1) { //-1 target opacity means to not animate anyway
        //animate the opacity
        qreal new_opacity_progress = qMin(item.opacityProgress() + delta_progress, (qreal)1.0); //between 0 and 1
        item.setOpacityProgress(new_opacity_progress);
        qreal opacity_animation_value = q->style()->opacityEasingCurve().valueForProgress(new_opacity_progress);

        qreal current_opacity = item.item()->opacity();
        if (current_opacity != -1) {
            qreal new_opacity = item.sourceOpacity() + (item.targetOpacity() - item.sourceOpacity()) * opacity_animation_value;
            new_opacity = qAbs(new_opacity); //make sure it is between 0 and 1
            if (new_opacity > 1)
                new_opacity = qMax((float)qAbs(2 - new_opacity), 1.0f);
            item.item()->setOpacity(new_opacity);
        }
    }

    //Have we finished with this item now?
    if (item.opacityProgress() == 1 && item.translateProgress() == 1) {
        if (item.isSet(MItemState::STATE_FLAG_TO_BE_HIDDEN)) {
            if (item.targetOpacity() != -1)
                item.item()->setOpacity(1); //Restore the opacity to 1, since we are hiding it now anyway
            q->hideItemNow(item.item());
        }
        item.animationDone();
    }
}

void MBasicLayoutAnimationPrivate::doItemHiddenAnimation(MItemState *itemstate)
{
    Q_Q(MBasicLayoutAnimation);
    Q_ASSERT(itemstate->item());
    if (!q->style()->animateOpacity())
        return;
    QGraphicsItem *graphicsItem = itemstate->item()->graphicsItem();
    if (graphicsItem) {
        qreal finalOpacity = q->style()->finalHidingOpacity();
        if (finalOpacity >= 0 && finalOpacity < 1) //If it ==1 there's no need to do anything
            itemstate->setTargetOpacity(finalOpacity);
    }
}
void MBasicLayoutAnimationPrivate::doItemShownAnimation(MItemState *itemstate)
{
    Q_Q(MBasicLayoutAnimation);
    Q_ASSERT(itemstate->item());
    q->showItemNow(itemstate->item());
    if (!q->style()->animateOpacity())
        return;
    QGraphicsItem *graphicsItem = itemstate->item()->graphicsItem();
    if(!graphicsItem)
        return;
    if (itemstate->isSet(MItemState::STATE_FLAG_SHOWING)) {
        //If the item is already visible, this can be called because we were currently doing a hide animation.
        //So just make sure that we make everything visible
        itemstate->setTargetOpacity(1);
        graphicsItem->setOpacity(1);
        return;
    }

    qreal initialOpacity = q->style()->initialShowingOpacity();
    if (initialOpacity >= 0 && initialOpacity < 1) { //If it ==1 there's no need to do anything
        graphicsItem->setOpacity(initialOpacity);
        itemstate->setTargetOpacity(1);
    }
}

