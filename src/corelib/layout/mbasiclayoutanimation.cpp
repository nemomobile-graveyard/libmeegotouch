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

#include "mbasiclayoutanimation.h"
#include "mbasiclayoutanimation_p.h"
#include "mbasiclayoutanimationstyle.h"
#include "mlayouthelper_p.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QEasingCurve>
MBasicLayoutAnimation::MBasicLayoutAnimation(MLayout *layout) :
    MLayoutAnimation(*(new MBasicLayoutAnimationPrivate()), layout)
{
    Q_D(MBasicLayoutAnimation);
    d->q_ptr = this;
    setLoopCount(-1);
    //Add any existing items to the animation
    const int itemCount = layout->count();
    for(int i = 0; i < itemCount; ++i)
        itemAddedToLayout(i);

}

MBasicLayoutAnimation::~MBasicLayoutAnimation() { }

void MBasicLayoutAnimation::itemAddedToLayout(int index) {
    Q_D(MBasicLayoutAnimation);
    QGraphicsItem *item = layout()->itemAt(index)->graphicsItem();
    if(item && item->isWidget())
        d->states.insert(index, MItemState(static_cast<QGraphicsWidget *>(item)));
    else
        d->states.insert(index, MItemState(NULL));
}
void MBasicLayoutAnimation::itemRemovedFromLayout(int index) {
    Q_D(MBasicLayoutAnimation);
    d->states.removeAt(index);
}

void MBasicLayoutAnimation::setItemGeometry(int index, const QRectF &geometry) {
    Q_D(MBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    MItemState &state = d->states[index];
    Q_ASSERT(state.item());
    if(state.isSet(MItemState::STATE_FLAG_SHOWING) && !state.isSet(MItemState::STATE_FLAG_TO_BE_HIDDEN)) {
        QPointF originalPositionCenter = state.item()->pos() + QPointF(state.item()->transform().dx(), state.item()->transform().dy());
        state.item()->setGeometry(geometry); //Note that the item's geometry doesn't have to be equal to geometry, since there are various constraints
        QPointF offset = originalPositionCenter - state.item()->pos();
        state.item()->setTransform( QTransform::fromTranslate(offset.x(), offset.y()) );
        state.setSourceTranslatePoint(offset);
    } else {
        state.item()->setTransform( QTransform() );
        state.item()->setGeometry(geometry); //Note that the item's geometry doesn't have to be equal to geometry, since there are various constraints
        d->doItemShownAnimation(&state);
        state.addFlags(MItemState::STATE_FLAG_SHOWING);
        state.removeFlags(MItemState::STATE_FLAG_TO_BE_HIDDEN);
    }

    if (state.isAnimationDone()) {//Set the geometry anyway, so that it can refresh
        if (style()->animateOpacity() && state.targetOpacity() != -1)
            state.item()->setOpacity(1);
        state.item()->setTransform( QTransform() );
    } else
        start();
}
void MBasicLayoutAnimation::hideItem(int index) {
    Q_D(MBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    MItemState &state = d->states[index];
    Q_ASSERT(state.item());
    //Check if its being hidden or if it is hiding (i.e. not shown)
    if (state.isSet(MItemState::STATE_FLAG_TO_BE_HIDDEN) ||
            (!state.isSet(MItemState::STATE_FLAG_SHOWING))) {
        return; //It's already hiding/hidden.  Nothing to do
    }
    state.hide();
    if (!state.item()->isVisibleTo(NULL)) {
        //It is not visible, so do not bother animating
        if (style()->animateOpacity() && state.targetOpacity() != -1)
            state.item()->setOpacity(1); //Restore the opacity to 1 if we have changed it, since we are hiding it now anyway
        hideItemNow(state.item());
        state.animationDone();
    }

    d->doItemHiddenAnimation(&state);
    state.hide(); //setTargetGeometry clears TO_BE_HIDDEN, so re-add
    start(); //start the timer to animate, if not already started
}
void MBasicLayoutAnimation::animatedDeleteItem(int index) {
    Q_D(MBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    MItemState &state = d->states[index];
    Q_ASSERT(state.item());
    if(state.isSet(MItemState::STATE_FLAG_TO_BE_DELETED))
        return;
    if(!state.isSet(MItemState::STATE_FLAG_SHOWING)) {
        //If it's not currently being shown, just delete straight away
        layout()->removeAt(index);
        return;
    }

    state.addFlags(MItemState::STATE_FLAG_TO_BE_DELETED);

    d->doItemHiddenAnimation(&state);
    start(); //start the timer to animate, if not already started
}
void MBasicLayoutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(MBasicLayoutAnimation);
    Q_UNUSED(oldState);
    if (newState == Running)
        d->timeSinceLastUpdate.restart();
}
void MBasicLayoutAnimation::updateCurrentTime(int msecs)
{
    Q_D(MBasicLayoutAnimation);
    Q_UNUSED(msecs);
    d->tick();
}

