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

#include "duibasiclayoutanimation.h"
#include "duibasiclayoutanimation_p.h"
#include "duibasiclayoutanimationstyle.h"
#include "duilayouthelper_p.h"

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsScene>
#include <QEasingCurve>
DuiBasicLayoutAnimation::DuiBasicLayoutAnimation(DuiLayout *layout) :
    DuiLayoutAnimation(*(new DuiBasicLayoutAnimationPrivate()), layout)
{
    Q_D(DuiBasicLayoutAnimation);
    d->q_ptr = this;
    setLoopCount(-1);
    //Add any existing items to the animation
    const int itemCount = layout->count();
    for(int i = 0; i < itemCount; ++i)
        itemAddedToLayout(i);

}

DuiBasicLayoutAnimation::~DuiBasicLayoutAnimation() { }

void DuiBasicLayoutAnimation::itemAddedToLayout(int index) {
    Q_D(DuiBasicLayoutAnimation);
    QGraphicsItem *item = layout()->itemAt(index)->graphicsItem();
    if(item && item->isWidget())
        d->states.insert(index, DuiItemState(static_cast<QGraphicsWidget *>(item)));
    else
        d->states.insert(index, DuiItemState(NULL));
}
void DuiBasicLayoutAnimation::itemRemovedFromLayout(int index) {
    Q_D(DuiBasicLayoutAnimation);
    d->states.removeAt(index);
}

void DuiBasicLayoutAnimation::setItemGeometry(int index, const QRectF &geometry) {
    Q_D(DuiBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    DuiItemState &state = d->states[index];
    Q_ASSERT(state.item());
    state.setTargetGeometry(geometry);
    if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN)) {
        d->doItemShownAnimation(&state);
        //we only want to call the item shown animation once, so clear the shown flags
        state.removeFlags(DuiItemState::STATE_FLAG_TO_BE_SHOWN);
        state.addFlags(DuiItemState::STATE_FLAG_SHOWING);
    }
    if (state.isAnimationDone()) {//Set the geometry anyway, so that it can refresh
        state.item()->setOpacity(1); //Restore the opacity to 1, since we are hiding it now anyway
        state.item()->setGeometry(state.targetGeometry());
    } else
        start();
}
void DuiBasicLayoutAnimation::hideItem(int index) {
    Q_D(DuiBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    DuiItemState &state = d->states[index];
    Q_ASSERT(state.item());
    //Check if its being hidden or if it is hiding (i.e. not shown and not to-be-shown
    if (state.isSet(DuiItemState::STATE_FLAG_TO_BE_HIDDEN) ||
            (!state.isSet(DuiItemState::STATE_FLAG_TO_BE_SHOWN) &&
             !state.isSet(DuiItemState::STATE_FLAG_SHOWING))) {
        return; //It's already hiding/hidden.  Nothing to do
    }
    state.hide();
    if(!state.item()->isVisibleTo(NULL)) {
        //It is not visible, so do not bother animating
        state.item()->setOpacity(1); //Restore the opacity to 1, since we are hiding it now anyway
        hideItemNow(state.item());
        state.animationDone();
    }

    d->doItemHiddenAnimation(&state);
    state.hide(); //setTargetGeometry clears TO_BE_HIDDEN, so re-add
    start(); //start the timer to animate, if not already started
}
void DuiBasicLayoutAnimation::animatedDeleteItem(int index) {
    Q_D(DuiBasicLayoutAnimation);
    Q_ASSERT(index >= 0 && index < d->states.count());
    DuiItemState &state = d->states[index];
    Q_ASSERT(state.item());
    if(state.isSet(DuiItemState::STATE_FLAG_TO_BE_DELETED))
        return;
    if(!state.isSet(DuiItemState::STATE_FLAG_SHOWING)) {
        //If it's not currently being shown, just delete straight away
        layout()->removeAt(index);
        return;
    }

    state.addFlags(DuiItemState::STATE_FLAG_TO_BE_DELETED);

    d->doItemHiddenAnimation(&state);
    start(); //start the timer to animate, if not already started
}
void DuiBasicLayoutAnimation::updateState(QAbstractAnimation::State newState, QAbstractAnimation::State oldState)
{
    Q_D(DuiBasicLayoutAnimation);
    Q_UNUSED(oldState);
    if (newState == Running)
        d->timeSinceLastUpdate.restart();
}
void DuiBasicLayoutAnimation::updateCurrentTime(int msecs)
{
    Q_D(DuiBasicLayoutAnimation);
    Q_UNUSED(msecs);
    d->tick();
}

