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

#include "mlinearlayoutpolicy_p.h"

#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsWidget>
#include <QtGui/QWidget>

#include "mwidgetcontroller.h"

#include "mlayout.h"
#include "mlayouthelper_p.h"
MLinearLayoutPolicyPrivate::MLinearLayoutPolicyPrivate(MLayout *layout, Qt::Orientation orientation) :
    MAbstractLayoutPolicyPrivate(layout),
    engineWidget(new QGraphicsWidget),
    engine(new QGraphicsLinearLayout(orientation, engineWidget)),
    rowCount(0),
    notifyWidgetsOfLayoutPositionEnabled(false)
{
}

MLinearLayoutPolicyPrivate::~MLinearLayoutPolicyPrivate()
{
    delete engineWidget; //This deletes the engine, which in turn deletes all of its proxy children (they are setOwnedByLayout(true))
}

void MLinearLayoutPolicyPrivate::fixIndex(int *index) const
{
    if (uint(*index) >= uint(rowCount))
        *index = rowCount;
}

void MLinearLayoutPolicyPrivate::refreshEngine()
{
    //Make sure that we have our RTL/LTR correct
    bool directionChanged = engineWidget->layoutDirection() != layout->layoutDirection();
    if (directionChanged)
        engineWidget->setLayoutDirection(layout->layoutDirection());

    for (int i = engine->count() - 1; i >= 0; --i) {
        ProxyItem *item = static_cast<ProxyItem *>(engine->itemAt(i));
        item->refresh();
    }
    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    engine->setContentsMargins(left, top, right, bottom);

    engine->updateGeometry();

    if (directionChanged && notifyWidgetsOfLayoutPositionEnabled)
        notifyAllWidgetsOfLayoutPosition();
}
void MLinearLayoutPolicyPrivate::refreshWidget()
{
    //We need to make engine->geometry() equal the layout->geometry() so that the items are in the right place
    qreal topMargin = layout->geometry().top();
    qreal leftMargin = layout->geometry().left();
    engineWidget->setContentsMargins(leftMargin, topMargin, 0, 0);

    qreal width = layout->geometry().right();
    qreal height = layout->geometry().bottom();
    engineWidget->resize(width, height);
    engine->setGeometry(engine->geometry()); //We need to call this to make sure the layout actually updates contents margins and RTL
}

void MLinearLayoutPolicyPrivate::notifyWidgetOfLayoutPosition(int index, M::Position position)
{
    MWidgetController* widget = dynamic_cast<MWidgetController*>(static_cast<ProxyItem *>(engine->itemAt(index))->proxiedItem());
    if (widget)
        widget->setLayoutPosition(position);
}

void MLinearLayoutPolicyPrivate::notifyAffectedWidgetsOfLayoutPosition(int index, bool add)
{
    if (!notifyWidgetsOfLayoutPositionEnabled)
        return;

    //setup first, last and center positions based on layout direction (LTR or RTL) and orientation (VERT or HORIZ)
    M::Position first, last;
    if (layout->layoutDirection() == Qt::LeftToRight) {
        first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalTopPosition;
        last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalBottomPosition;
    } else {
        first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalTopPosition;
        last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalBottomPosition;
    }
    M::Position center = (engine->orientation() == Qt::Horizontal) ? M::HorizontalCenterPosition : M::VerticalCenterPosition;

    //added to index
    if (add) {
        //no more than three items in the layout, reposition them all
        if (engine->count() <= 3) {
            notifyAllWidgetsOfLayoutPosition();
        } else {
            if (index == 0) {
                //added as first
                notifyWidgetOfLayoutPosition(0, first);
                notifyWidgetOfLayoutPosition(1, center);
            } else if (index >= engine->count() - 1) {
                //added as last
                notifyWidgetOfLayoutPosition(engine->count()-2, center);
                notifyWidgetOfLayoutPosition(engine->count()-1, last);
            } else {
                //added to middle
                notifyWidgetOfLayoutPosition(index, center);
            }
        }
    } else {
        //removing item from index
        //remove positioning from the removed item
        notifyWidgetOfLayoutPosition(index, M::DefaultPosition);

        //change positioning of an affected item if item is removed from first or last position
        if (engine->count()-1 > 0) {
            if (engine->count() == 2) {
                //only one item remains after removal
                notifyWidgetOfLayoutPosition((index == 0) ? 1 : 0, M::DefaultPosition);
            } else if (index == 0) {
                notifyWidgetOfLayoutPosition(1, first);
            } else if (index == engine->count()-1) {
                notifyWidgetOfLayoutPosition(engine->count()-2, last);
            }
        }
    }
}

void MLinearLayoutPolicyPrivate::notifyAllWidgetsOfLayoutPosition()
{
    if (notifyWidgetsOfLayoutPositionEnabled) {
        //setup first, last and center positions based on layout direction (LTR or RTL) and orientation (VERT or HORIZ)
        M::Position first, last;
        if (layout->layoutDirection() == Qt::LeftToRight) {
            first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalTopPosition;
            last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalBottomPosition;
        } else {
            first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalTopPosition;
            last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalBottomPosition;
        }
        M::Position center = (engine->orientation() == Qt::Horizontal) ? M::HorizontalCenterPosition : M::VerticalCenterPosition;;

        if (engine->count() == 1) {
            //only one item in the layout
            notifyWidgetOfLayoutPosition(0, M::DefaultPosition);
        } else if (engine->count() > 1) {
            //more than one item in the layout
            notifyWidgetOfLayoutPosition(0, first);
            notifyWidgetOfLayoutPosition(engine->count()-1, last);
            for (int i = 1; i < engine->count()-1; ++i) {
                notifyWidgetOfLayoutPosition(i, center);
            }
        }
    //remove positioning from all the items
    } else {
        for (int i = 0; i < engine->count(); ++i) {
            notifyWidgetOfLayoutPosition(i, M::DefaultPosition);
        }
    }
}


