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

#include "mflowlayoutpolicy.h"
#include "mflowlayoutpolicy_p.h"
#include "mlayout.h"

#include <QtGui/QWidget>

MFlowLayoutPolicy::MFlowLayoutPolicy(MLayout *layout) :
    MAbstractLayoutPolicy(*(new MFlowLayoutPolicyPrivate(layout)))
{
    setHeightForWidth(true);
}

MFlowLayoutPolicy::~MFlowLayoutPolicy()
{ }

void MFlowLayoutPolicy::addItem(QGraphicsLayoutItem *item)
{
    Q_D(MFlowLayoutPolicy);
    MAbstractLayoutPolicy::addItem(item);
    int index = indexOf(item);
    if(index >= 0)
        d->alignments.insert(index, Qt::AlignCenter);

    Q_ASSERT(count() == d->alignments.count());
}
void MFlowLayoutPolicy::insertItem(int index, QGraphicsLayoutItem *item)
{
    Q_D(MFlowLayoutPolicy);
    MAbstractLayoutPolicy::insertItem(index, item);
    index = indexOf(item);
    if(index >= 0)
        d->alignments.insert(index, Qt::AlignCenter);
    Q_ASSERT(count() == d->alignments.count());
}
void MFlowLayoutPolicy::removeAt(int index)
{
    Q_D(MFlowLayoutPolicy);
    MAbstractLayoutPolicy::removeAt(index);
    d->alignments.removeAt(index);
    Q_ASSERT(count() == d->alignments.count());
}
//Convience function to get the width of the item, obeying heightForWidth if needed
static QSizeF getItemPreferredSize(const QGraphicsLayoutItem *item, qreal constraintWidth) {
    QSizeF size = item->effectiveSizeHint(Qt::PreferredSize);
    if (size.width() > constraintWidth) {
        if (item->sizePolicy().hasHeightForWidth()) {
            //If the object's preferred size is too big to fit, try constraining it
            size = item->effectiveSizeHint(Qt::PreferredSize, QSizeF(constraintWidth, -1));
        } else {
            size.setWidth(constraintWidth);
        }
    }
    return size;
}
QSizeF MFlowLayoutPolicy::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    Q_D(const MFlowLayoutPolicy);
    QSizeF new_size = constraint;
    if (which == Qt::MaximumSize) {
        if (new_size.width() < 0)
            new_size.setWidth(QWIDGETSIZE_MAX);
        if (new_size.height() < 0)
            new_size.setHeight(QWIDGETSIZE_MAX);
        return new_size;
    }
    //TODO - return (0,0) for PreferredSize?  Since this will be rounded up to MinimumSize automatically
    qreal left, top, right, bottom;
    d->layout->getContentsMargins(&left, &top, &right, &bottom);

    // There is a Qt bug 257455  which means that we aren't always passed the width constraint
    // To try to work around this, we use the current width as the constraint for finding the height,
    // and return that as the preferred width.
    //
    // http://www.qtsoftware.com/developer/task-tracker/index_html?method=entry&id=257455
    //
    // FIXME we should be able to remove this when/if Qt bug 257455 is fixed
    qreal constraintWidth;  //ideally this would just be same as new_size.width(), but it needs to be different to work around this bug..
    if (new_size.width() < 0) {
        qreal width = 0;
        const int theCount = count();
        for (int i = 0; i < theCount; ++i) {
            QGraphicsLayoutItem *item = itemAt(i);
            //The flow layout would become insanely more difficult if we want to find out the
            //minimum size based on the minimum size of the widgets.  Instead we use the
            //preferred size as the minimum size
            width = qMax(width, item->effectiveSizeHint(Qt::PreferredSize).width());
        }
        if (which == Qt::MinimumSize)
            new_size.setWidth(width);
        // Set to the current width, expanded to the largest preferred width of the widgets
        constraintWidth = qMax(d->layout->geometry().width() - (left + right), width);
        if (which == Qt::PreferredSize)
            new_size.setWidth(constraintWidth);
    } else {
        //subtract the margins from the given constraint
        constraintWidth = qMax(new_size.width() - (left + right), (qreal)0.0);
        new_size.setWidth(constraintWidth);
    }


    //We are now Qt::PreferredSize  or Qt::MinimumSize, and we have a width constraint
    QPointF current_position;
    qreal current_rowheight = 0;
    int rowCount = 0;
    const int theCount = count();
    for (int i = 0; i < theCount; ++i) {
        const QGraphicsLayoutItem *item = itemAt(i);
        //We use the preferredsize for both the minimum and preferred sizeHints since
        //doing otherwise would make this code a lot more complicated
        QSizeF size = getItemPreferredSize(item, constraintWidth);
        bool sameRow = d->roomForItemOnCurrentRow(size, constraintWidth, &current_position, &current_rowheight);
        if (sameRow) {
            //It fits on the same row
            current_position.rx() += size.width();
        } else if ((uint)++rowCount >= (uint)d->rowLimit) {
            break; //we have reached the maximum number of rows
        } else {
            //Place item on the next row
            current_position.rx() = size.width();
            current_position.ry() += current_rowheight + d->verticalSpacing;
            current_rowheight = size.height();
        }
    }

    new_size.setHeight(current_position.y() + current_rowheight);
    new_size += QSizeF(left + right, top + bottom);
    return new_size;
}
int MFlowLayoutPolicy::rowCount()
{
    Q_D(MFlowLayoutPolicy);
    if (d->numberOfRows == -1 && isActive()) //the layout is invalidated - we need to relayout first
        layout()->activate();
    if (d->numberOfRows == -1)
        return -1;

    return qMin((uint)d->numberOfRows, (uint)d->rowLimit);
}
void MFlowLayoutPolicy::invalidate()
{
    Q_D(MFlowLayoutPolicy);
    d->numberOfRows = -1;
}

void MFlowLayoutPolicy::relayout()
{
    Q_D(MFlowLayoutPolicy);

    /* The items are laid out in one or two passes.
     *
     * If the items are to be horizontally justified, then we want to expand all the items so that
     * the right hand edge aligns with the side of the layout.
     * This is done by making two passes over each row.  The first pass determines what items
     * can fit into the current row and how much space is left over.
     * The second pass divides up the left over space among the items and actually sets their
     * target positions
     */
    QPointF origin = contentsArea().topLeft();
    qreal constraintWidth = contentsArea().width();
    QPointF current_position;
    d->numberOfRows = 0;
    int i = 0;
    while (i < count() && (uint)d->numberOfRows < (uint)d->rowLimit) {
        //for each row
        //find the end of this row
        qreal current_rowheight = 0;
        int num_expandable = 0;
        int row_start = i; //The item index at the start of a row
        while (i < count()) {
            QGraphicsLayoutItem *item = itemAt(i);
            QSizeF size = getItemPreferredSize(item, constraintWidth);
            bool sameRow = d->roomForItemOnCurrentRow(size, constraintWidth, &current_position, &current_rowheight);
            //check if we have finished with this row
            if (sameRow) {
                if (item->sizePolicy().expandingDirections().testFlag(Qt::Horizontal))
                    num_expandable++;
                current_position.rx() += size.width();
                i++;
            } else
                break; //we have finished with this row
        }
        qreal leftover_space = constraintWidth - current_position.x();

        //Originally the code asserted if you try to add a big (minimum size)
        //object into a small (maximum size) layout, but sometimes users do silly
        //things and we need to handle that gracefully.
//        Q_ASSERT(leftover_space >= -0.00001);  //slight rounding errors can results in the number being just slightly less than 0
        //set the current position back to the start of the row
        current_position.setX(0);
        //we now know how wide this row would be, if we used preferred size.  Allow stretching
        //The row goes from "row_start" to "i-1", inclusive.
        for (int j = row_start; j < i; ++j) {
            QGraphicsLayoutItem *item = itemAt(j);
            //get the size again
            QSizeF size = getItemPreferredSize(item, constraintWidth);
            d->roomForItemOnCurrentRow(size, constraintWidth, &current_position);
//            Q_ASSERT(sameRow);

            Qt::Orientations expandable = item->sizePolicy().expandingDirections();
            QSizeF maxSize;
            if (expandable)
                maxSize = item->effectiveSizeHint(Qt::MaximumSize);
            //let the items expand vertically if they want to
            if (size.height() != current_rowheight) {
                if (expandable.testFlag(Qt::Vertical)) {
                    //expand item to rowheight, making sure we don't exceed max size
                    size.setHeight(qMin(maxSize.height(), current_rowheight));
                }
            }

            if (leftover_space > 0 && expandable.testFlag(Qt::Horizontal)) {
                //distribute remaining leftover space to items left to process
                //making sure that we don't exceed the maximum size
                //Note that this isn't a great algorithm.  It won't look very good if some items
                //do have a maximum width
                qreal expand_by = qMin(leftover_space / num_expandable, maxSize.width() - size.width());
                size.setWidth(size.width() + expand_by);
                leftover_space -= expand_by;
                Q_ASSERT(leftover_space >= -0.00001f); //slight rounding errors can results in the number being just slightly less than 0
                --num_expandable;
            }

            QRectF geometry = QRectF(origin, size);

            //Set vertical alignment as per flags
            if(d->alignments[j] & Qt::AlignVCenter)
                geometry.translate(0, (current_rowheight - size.height())/2);
            else if(d->alignments[j] & Qt::AlignBottom)
                geometry.translate(0, (current_rowheight - size.height()));

            if (layout()->layoutDirection() == Qt::LeftToRight)
                geometry.translate(current_position);
            else
                geometry.translate(constraintWidth - current_position.x() - size.width() , current_position.y());

            current_position.rx() += size.width();

            //now position this item
            setItemGeometry(j, geometry);
        }
        current_position.rx() = 0;
        current_position.ry() += current_rowheight + d->verticalSpacing;
        d->numberOfRows++;
    }
    //Hide any unplaced items - this can happen if we reached d->rowLimit
    while (i < count()) {
        hideItem(i++);
    }
}

void MFlowLayoutPolicy::setRowLimit(int rowLimit)
{
    Q_D(MFlowLayoutPolicy);
    d->rowLimit = rowLimit;
    invalidatePolicyAndLayout();
}
int MFlowLayoutPolicy::rowLimit() const
{
    Q_D(const MFlowLayoutPolicy);
    return d->rowLimit;
}
Qt::Alignment MFlowLayoutPolicy::alignment( QGraphicsLayoutItem * item ) const
{
    Q_D(const MFlowLayoutPolicy);
    int index = indexOf(item);
    if(index < 0)
        return Qt::AlignCenter;
    return d->alignments[index];
}

void MFlowLayoutPolicy::setAlignment( QGraphicsLayoutItem * item, Qt::Alignment alignment )
{
    Q_D(MFlowLayoutPolicy);
    int index = indexOf(item);
    if(index < 0 || d->alignments[index] == alignment)
        return;

    d->alignments[index] = alignment;
    invalidatePolicyAndLayout();
}

