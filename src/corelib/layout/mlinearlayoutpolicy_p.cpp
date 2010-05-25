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
    notifyWidgetsOfLayoutPositionEnabled(false)
{
    rowCount = 0;
    engineWidget->setContentsMargins(0, 0, 0, 0);
    engineWidget->setMinimumSize(0, 0);
    engineWidget->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

MLinearLayoutPolicyPrivate::~MLinearLayoutPolicyPrivate()
{
    delete engineWidget; //This deletes the engine, which in turn deletes all of its children
}

void MLinearLayoutPolicyPrivate::fixIndex(int *index) const
{
    if (uint(*index) >= uint(rowCount))
        *index = rowCount;
}

void MLinearLayoutPolicyPrivate::refreshEngine()
{
    bool directionChanged = engineWidget->layoutDirection() != layout->layoutDirection();
    engineWidget->setLayoutDirection(layout->layoutDirection());   //Make sure that we have our RTL/LTR correct

    engine->invalidate();
    for (int i = engine->count() - 1; i >= 0; --i) {
        ProxyItem *item = static_cast<ProxyItem *>(engine->itemAt(i));
        item->refresh();
    }
    qreal left, top, right, bottom;
    layout->getContentsMargins(&left, &top, &right, &bottom);
    engine->setContentsMargins(left, top, right, bottom);

    engine->updateGeometry();

    //We need to make engine->geometry() equal the layout->geometry() so that the items are in the right place
    qreal topMargin = layout->geometry().top();
    qreal leftMargin = layout->geometry().left();
    qreal width = layout->geometry().right();
    qreal height = layout->geometry().bottom();
    engineWidget->setContentsMargins(leftMargin, topMargin, 0, 0);
    engineWidget->resize(width, height);
    
    //update layout positions here only if layout direction has changed
    if( directionChanged && notifyWidgetsOfLayoutPositionEnabled )
        notifyAllWidgetsOfLayoutPosition();
}

void MLinearLayoutPolicyPrivate::notifyWidgetOfLayoutPosition(int index, M::Position position)
{
    MWidgetController* widget = dynamic_cast<MWidgetController*>(static_cast<ProxyItem *>(engine->itemAt(index))->proxiedItem());
    if( widget ) {
        widget->setLayoutPosition(position);
    }
}

void MLinearLayoutPolicyPrivate::notifyAffectedWidgetsOfLayoutPosition(int index, bool add ) 
{
    if( !notifyWidgetsOfLayoutPositionEnabled )
        return;
    
    //setup first, last and center positions based on layout direction (LTR or RTL) and orientation (VERT or HORIZ)    
    M::Position first, last;
    if( layout->layoutDirection() == Qt::LeftToRight ) {
        first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalTopPosition;
        last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalBottomPosition;
    }
    else {
        first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalTopPosition;
        last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalBottomPosition;
    }
    M::Position center = (engine->orientation() == Qt::Horizontal) ? M::HorizontalCenterPosition : M::VerticalCenterPosition;

    //added to index
    if( add ) {
        //no more than three items in the layout, reposition them all
        if( engine->count() <= 3 )
            notifyAllWidgetsOfLayoutPosition();
        else {
            //added as first
            if( index == 0 ) {
                notifyWidgetOfLayoutPosition(0, first);
                notifyWidgetOfLayoutPosition(1, center);
            }
            //added as last
            else if( index == engine->count() - 1 ) {
                notifyWidgetOfLayoutPosition(engine->count()-2, center);
                notifyWidgetOfLayoutPosition(engine->count()-1, last);
            }
            //added to middle
            else {
                notifyWidgetOfLayoutPosition(index, center);
            }
        }
    }
    //removing item from index
    else {
        //remove positioning from the removed item
        notifyWidgetOfLayoutPosition(index, M::DefaultPosition);

        //change positioning of an affected item if item is removed from first or last position
        if( engine->count()-1 > 0 ) {
            //only one item remains after removal
            if( engine->count()-1 == 1 ) {
                notifyWidgetOfLayoutPosition((index == 0) ? 1 : 0, M::DefaultPosition);
            }
            else {
                //item is removed from first position, mark second item as first
                if( index == 0 ) {
                    notifyWidgetOfLayoutPosition(1, first);
                } 
                //item is removed from last position, mark second last item as last
                else if( index == engine->count()-1 ) {
                    notifyWidgetOfLayoutPosition(engine->count()-2, last);
                }
            }
        }
    }
    

}

void MLinearLayoutPolicyPrivate::notifyAllWidgetsOfLayoutPosition()
{
    if( notifyWidgetsOfLayoutPositionEnabled ) {
        //setup first, last and center positions based on layout direction (LTR or RTL) and orientation (VERT or HORIZ)    
        M::Position first, last;
        if( layout->layoutDirection() == Qt::LeftToRight ) {
            first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalTopPosition;
            last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalBottomPosition;
        }
        else {
            first = (engine->orientation() == Qt::Horizontal) ? M::HorizontalRightPosition : M::VerticalTopPosition;
            last = (engine->orientation() == Qt::Horizontal) ? M::HorizontalLeftPosition : M::VerticalBottomPosition;
        }
        M::Position center = (engine->orientation() == Qt::Horizontal) ? M::HorizontalCenterPosition : M::VerticalCenterPosition;;

    
        //only one item in the layout
        if( engine->count() == 1 ) {
            notifyWidgetOfLayoutPosition(0, M::DefaultPosition);
        //more than one item in the layout
        } else if( engine->count() > 1 ){
            notifyWidgetOfLayoutPosition(0, first);
            notifyWidgetOfLayoutPosition(engine->count()-1, last);
            for( int i = 1; i < engine->count()-1; ++i ) {
                notifyWidgetOfLayoutPosition(i, center);                
            }    
        }
    //remove positioning from all the items
    } else {
        for( int i = 0; i < engine->count(); ++i ) {
            notifyWidgetOfLayoutPosition(i, M::DefaultPosition);
        }                
    }
}


