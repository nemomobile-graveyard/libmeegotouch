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

#include <QGraphicsLinearLayout>
#include "mlayout.h"
#include "mlinearlayoutpolicy.h"
#include "mwidgetcontroller.h"
#include "mtextedit.h"
#include "mbutton.h"
#include "mtoolbarlayoutpolicy.h"

MToolBarLayoutPolicy::MToolBarLayoutPolicy(MLayout *layout)
    : MLinearLayoutPolicy(layout, Qt::Horizontal),
      textEditIndex(-1),
      maxSlots(unlimitedCapacity),
      spacesBetween(false),
      layoutDirection(Qt::LeftToRight),
      leftSpacer(0),
      rightSpacer(0),
      widgetAlignment(Qt::AlignHCenter),
      widgetAlignmentAffectsCapacity(false),
      centerAllWidgets(false),
      centerOffsetEnabledForAll(false),
      centerLabelOnlyButtons(true),
      centerOffsetEnabledForLabelOnly(false),
      centerOffset(0.0)
{
    setSpacing(0);
    updateSpacers();
}

MToolBarLayoutPolicy::~MToolBarLayoutPolicy()
{
    for (int i = widgetCount()-1; i >= 0; i--)
        removeWidgetAtPreservingSpacers(i);

    while (MLinearLayoutPolicy::count()) {
        // Remove left and right spacers
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(count()-1);
        MLinearLayoutPolicy::removeAt(count()-1);
        delete item;
    }
}

void MToolBarLayoutPolicy::insertWidgetAndRemoveOverflow(int widgetIndex, QGraphicsLayoutItem *item)
{
    widgetIndex = qMin((uint)widgetIndex, (uint)widgetCount());
    Q_ASSERT(item);

    //A ugly hack to try to cope with QTBUG-11134
    item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    if (dynamic_cast<MTextEdit *>(item->graphicsItem())) {
        if (textEditIndex != -1)  //Remove any previous text edit
            removeWidgetAtPreservingSpacers(textEditIndex);
        textEditIndex = widgetIndex;
    } else if (widgetIndex <= textEditIndex)
        textEditIndex++;

    int policyIndex = widgetIndexToPolicyIndex(widgetIndex);
    Q_ASSERT(MLinearLayoutPolicy::count() >= policyIndex);

    if (widgetCount() > 0) {
        bool lastItem = (widgetIndex == widgetCount());
        if (lastItem) {
            MLinearLayoutPolicy::insertItem(policyIndex-1, item, Qt::AlignVCenter);
            insertSpacer(policyIndex-1);
        } else {
            MLinearLayoutPolicy::insertItem(policyIndex, item, Qt::AlignVCenter);
            insertSpacer(policyIndex+1);
        }
    } else
        MLinearLayoutPolicy::insertItem(policyIndex, item, Qt::AlignVCenter);

    updateLayoutPositionDueToInsert(widgetIndex);
    removeOverflownWidgets();
    updateSpacers();
}

void MToolBarLayoutPolicy::removeOverflownWidgets()
{
    while (takenSlots() > effectiveMaxWidgetSlots())
        removeWidgetAtPreservingSpacers(widgetCount()-1);
}

void MToolBarLayoutPolicy::removeWidgetAt(int widgetIndex)
{
    removeWidgetAtPreservingSpacers(widgetIndex);
    updateSpacers();
}

void MToolBarLayoutPolicy::removeWidgetAtPreservingSpacers(int widgetIndex)
{
    if (widgetIndex < 0 || widgetIndex > widgetCount())
        return;

    if (widgetIndex == textEditIndex)
        textEditIndex = -1;
    else if (textEditIndex != -1 && widgetIndex < textEditIndex)
        textEditIndex--;

    updateLayoutPositionDueToRemove(widgetIndex);

    int policyIndex = widgetIndexToPolicyIndex(widgetIndex);
    Q_ASSERT(MLinearLayoutPolicy::count() >= policyIndex);

    bool lastItem = (widgetIndex == widgetCount()-1);
    bool onlyItem = (widgetCount() == 1);

    MLinearLayoutPolicy::removeAt(policyIndex);
    if (!onlyItem) {
        //remove and delete spacer
        int spacerIndex = lastItem ? policyIndex-1 : policyIndex;
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(spacerIndex);
        MLinearLayoutPolicy::removeAt(spacerIndex);
        delete item;
    }
}

bool MToolBarLayoutPolicy::roomForWidget(int widgetIndex, bool textEdit) const
{
    widgetIndex = qMin((uint)widgetIndex, (uint)widgetCount());

    if (textEdit) {
        if (textEditIndex != -1 && widgetIndex > textEditIndex) {
            //If we come first, we push the old textedit out of the way
            return false; //No room for us
        } else if (widgetIndex+1 >= effectiveMaxWidgetSlots())
            return false; //No room to add
    }
    else if (widgetIndex >= effectiveMaxWidgetSlots())
        return false; //No room to add
    else if (textEditIndex != -1 && widgetIndex >= effectiveMaxWidgetSlots() -1)
        return false; //No room to add because textedit takes up two spaces
    return true;
}

int MToolBarLayoutPolicy::widgetCount() const
{
    return (MLinearLayoutPolicy::count()-(leftSpacer ? 1 : 0))/2;
}

int MToolBarLayoutPolicy::takenSlots() const
{
    if (textEditIndex != -1)
        return widgetCount()+1;  //textedit counts for two slots
    else
        return widgetCount();
}

int MToolBarLayoutPolicy::widgetIndexOf(const QGraphicsLayoutItem *item) const
{
    int base = leftSpacer ? 1 : 0;
    for (int i = widgetCount()-1; i >= 0; i--) {
        if (MLinearLayoutPolicy::itemAt(base+i*2) == item)
            return i;
    }
    return -1;
}

void MToolBarLayoutPolicy::removeWidget(const QGraphicsLayoutItem *item)
{
    removeWidgetAt(widgetIndexOf(item));
}

void MToolBarLayoutPolicy::removeAllWidgets()
{
    for (int i = widgetCount()-1; i >= 0; i--) {
        removeWidgetAtPreservingSpacers(i);
    }
}

QGraphicsLayoutItem *MToolBarLayoutPolicy::widgetAt(int widgetIndex) const
{
    if (widgetIndex < 0 || widgetIndex >= widgetCount())
        return 0;

    return MLinearLayoutPolicy::itemAt(widgetIndexToPolicyIndex(widgetIndex));
}

int MToolBarLayoutPolicy::maxWidgetSlots() const
{
    return maxSlots;
}

int MToolBarLayoutPolicy::effectiveMaxWidgetSlots() const
{
    int actualMax = maxSlots;

    if (widgetAlignmentAffectsCapacity && maxSlots != unlimitedCapacity) {
        if (!isAlignedLeftOrJustified())
            actualMax--;
        if (!isAlignedRightOrJustified())
            actualMax--;
    }

    return actualMax;
}

void MToolBarLayoutPolicy::setMaxWidgetSlots(int maxSlotCount)
{
    maxSlots = maxSlotCount;

    removeOverflownWidgets();
    updateSpacers();
}

int MToolBarLayoutPolicy::freeWidgetSlots() const
{
    return (qMax(0, effectiveMaxWidgetSlots()-takenSlots()));
}

void MToolBarLayoutPolicy::setSpacesBetween(bool enabled)
{
    if (spacesBetween == enabled)
        return;

    spacesBetween = enabled;

    updateSpacers();
}

void MToolBarLayoutPolicy::setWidgetAlignment(Qt::Alignment alignment, bool affectCapacity)
{
    widgetAlignment = alignment;
    widgetAlignmentAffectsCapacity = affectCapacity;

    removeOverflownWidgets();
    updateSpacers();
}

void MToolBarLayoutPolicy::setLabelOnlyButtonCentering(bool center, bool enableOffset)
{
    if (centerLabelOnlyButtons == center && centerOffsetEnabledForLabelOnly == enableOffset)
        return;

    centerLabelOnlyButtons = center;
    centerOffsetEnabledForLabelOnly = enableOffset;

    updateSpacers();
}

void MToolBarLayoutPolicy::setCentering(bool allWidgets, bool enableOffset)
{
    if (centerAllWidgets == allWidgets && centerOffsetEnabledForAll == enableOffset)
        return;

    centerAllWidgets = allWidgets;
    centerOffsetEnabledForAll = enableOffset;

    updateSpacers();
}

void MToolBarLayoutPolicy::setCenterOffset(qreal offset)
{
    if (offset == centerOffset)
        return;

    centerOffset = offset;

    updateContentsMargins();
}

int MToolBarLayoutPolicy::count() const
{
    return MLinearLayoutPolicy::count();
}

QGraphicsLayoutItem *MToolBarLayoutPolicy::itemAt(int policyIndex) const
{
    return MLinearLayoutPolicy::itemAt(policyIndex);
}

void MToolBarLayoutPolicy::insertItem(int policyIndex, QGraphicsLayoutItem *item)
{
    return MLinearLayoutPolicy::insertItem(policyIndex, item);
}

void MToolBarLayoutPolicy::indexOf(const QGraphicsLayoutItem *item) const
{
    Q_UNUSED(item);
}

void MToolBarLayoutPolicy::removeAt(int policyIndex)
{
    //This can be called by the layout, and we need to make sure that we remove the spacer as well
    removeWidgetAt(policyIndexToWidgetIndex(policyIndex));
}

void MToolBarLayoutPolicy::activated()
{
    MAbstractLayoutPolicy::activated();
    updateAllLayoutPositions();
}

void MToolBarLayoutPolicy::relayout()
{
    bool directionChanged = layoutDirection != layout()->layoutDirection();
    layoutDirection = layout()->layoutDirection();

    MLinearLayoutPolicy::relayout();

    if (directionChanged)
        updateAllLayoutPositions();
}

int MToolBarLayoutPolicy::widgetIndexToPolicyIndex(int widgetIndex) const
{
    return ((leftSpacer ? 1 : 0)+widgetIndex*2);
}

int MToolBarLayoutPolicy::policyIndexToWidgetIndex(int policyIndex) const
{
    return (policyIndex-(leftSpacer ? 1 : 0))/2;
}

QGraphicsWidget *MToolBarLayoutPolicy::insertSpacer(int policyIndex)
{
    //Create an item that will expand if necessary
    QGraphicsWidget *item = new QGraphicsWidget;
    item->setMaximumWidth(spacesBetween ? -1 : 0);
    item->setPreferredSize(1,1); //Ugly hack around QTBUG-11134
    item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    MLinearLayoutPolicy::insertItem(policyIndex, item);
    return item;
}

bool MToolBarLayoutPolicy::isAlignedLeftOrJustified() const
{
    return (widgetAlignment.testFlag(Qt::AlignLeft) || widgetAlignment.testFlag(Qt::AlignJustify));
}

bool MToolBarLayoutPolicy::isAlignedRightOrJustified() const
{
    return (widgetAlignment.testFlag(Qt::AlignRight) || widgetAlignment.testFlag(Qt::AlignJustify));
}

bool MToolBarLayoutPolicy::isJustifiedAlignment() const
{
    return (widgetAlignment.testFlag(Qt::AlignJustify));
}

void MToolBarLayoutPolicy::updateSpacers()
{
    updateContentsMargins();

    if (centerAllWidgets || (centerLabelOnlyButtons && widgetCount() > 0 && labelOnlyButtonCount() == widgetCount())) {
        activateLeftSpacer(true);
        activateRightSpacer(true);
        activateMiddleSpacers(false);

        return;
    }

    // Activate side spacer if there is just single widget in justified alignment
    // or alignment is neither to that direction nor justified.
    bool singleWidgetInJustified = (widgetCount() == 1 && isJustifiedAlignment());
    activateLeftSpacer(singleWidgetInJustified || !isAlignedLeftOrJustified());
    activateRightSpacer(singleWidgetInJustified || !isAlignedRightOrJustified());

    activateMiddleSpacers(spacesBetween);
}

void MToolBarLayoutPolicy::activateLeftSpacer(bool activate)
{
    if (!leftSpacer) {
        leftSpacer = new QGraphicsWidget;
        leftSpacer->setPreferredSize(1,1); //QTBUG-11134
        MLinearLayoutPolicy::insertItem(0, leftSpacer);
    }

    if (activate) {
        leftSpacer->setMaximumWidth(-1);
        leftSpacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    } else {
        leftSpacer->setMaximumWidth(0);
        leftSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    }
}

void MToolBarLayoutPolicy::activateRightSpacer(bool activate)
{
    if (!rightSpacer) {
        rightSpacer = new QGraphicsWidget;
        rightSpacer->setPreferredSize(1,1); //QTBUG-11134
        MLinearLayoutPolicy::insertItem(count(), rightSpacer);
    }

    if (activate) {
        rightSpacer->setMaximumWidth(-1);
        rightSpacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    } else {
        rightSpacer->setMaximumWidth(0);
        rightSpacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    }
}

void MToolBarLayoutPolicy::activateMiddleSpacers(bool activate)
{
    int base = 1+(leftSpacer ? 1 : 0);
    for (int i=base; i<MLinearLayoutPolicy::count()-1; i += 2) {
        QGraphicsLayoutItem *item = MLinearLayoutPolicy::itemAt(i);
        if (item) {
            if (activate) {
                item->setMaximumWidth(-1);
                item->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            } else {
                item->setMaximumWidth(0);
                item->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
            }
        }
    }
}

qreal MToolBarLayoutPolicy::effectiveCenterOffset() const
{
    if (centerAllWidgets && centerOffsetEnabledForAll)
        return centerOffset; //center offset is active for all widgets
    else if (centerLabelOnlyButtons && centerOffsetEnabledForLabelOnly && widgetCount() > 0 && labelOnlyButtonCount() == widgetCount())
        return centerOffset; //center offset is active for label only buttons and all buttons are such;
    return 0.0f;
}

void MToolBarLayoutPolicy::updateContentsMargins()
{
    qreal offset = effectiveCenterOffset();

    if (offset != 0.0f) {
        qreal margin = qAbs(offset*2);
        setContentsMargins(offset < 0.0f ? margin : -1, -1,
                           offset > 0.0f ? margin : -1, -1);
    } else {
        qreal left(-1), right(-1);
        getContentsMargins(&left, 0, &right, 0);
        if (left != -1 || right != -1)
            setContentsMargins(-1, -1, -1, -1);
    }
}

void MToolBarLayoutPolicy::updateLayoutPositionDueToInsert(int widgetIndex)
{
    if (!isActive())
        return;

    if (widgetCount() <= 3)
        updateAllLayoutPositions();
    else {
        bool leftToRight = layout()->layoutDirection() == Qt::LeftToRight;

        if (widgetIndex == 0) { // Added to first position
            setLayoutPosition(0, leftToRight ? M::HorizontalLeftPosition : M::HorizontalRightPosition);
            setLayoutPosition(1, M::HorizontalCenterPosition);
        } else if (widgetIndex == widgetCount() - 1) { // Added to last position
            setLayoutPosition(widgetCount()-2, M::HorizontalCenterPosition);
            setLayoutPosition(widgetCount()-1, leftToRight ? M::HorizontalRightPosition : M::HorizontalLeftPosition);
        } else // Added to middle
            setLayoutPosition(widgetIndex, M::HorizontalCenterPosition);
    }
}

void MToolBarLayoutPolicy::updateLayoutPositionDueToRemove(int widgetIndex)
{
    if (!isActive())
        return;

    setLayoutPosition(widgetIndex, M::DefaultPosition);

    if (widgetCount()-1 > 0) {
        if (widgetCount()-1 == 1) {
            // Only one will be left
            setLayoutPosition((widgetIndex == 0) ? 1 : 0, M::DefaultPosition);
        } else {
            bool leftToRight = layout()->layoutDirection() == Qt::LeftToRight;

            if (widgetIndex == 0)
                // Second is the new number one
                setLayoutPosition(1, leftToRight ? M::HorizontalLeftPosition : M::HorizontalRightPosition);
            else if (widgetIndex == widgetCount()-1)
                // Second but last will be last
                setLayoutPosition(widgetCount()-2, leftToRight ? M::HorizontalRightPosition : M::HorizontalLeftPosition);
        }
    }
}

void MToolBarLayoutPolicy::updateAllLayoutPositions()
{
    if (!isActive())
        return;

    if (widgetCount() == 1) {
        setLayoutPosition(0, M::DefaultPosition);
    } else if (widgetCount() > 1) {
        bool leftToRight = layout()->layoutDirection() == Qt::LeftToRight;

        setLayoutPosition(0, leftToRight ? M::HorizontalLeftPosition : M::HorizontalRightPosition);
        setLayoutPosition(widgetCount()-1, leftToRight ? M::HorizontalRightPosition : M::HorizontalLeftPosition);

        for (int i = 1; i < widgetCount()-1; ++i) {
            setLayoutPosition(i, M::HorizontalCenterPosition);
        }
    }
}

void MToolBarLayoutPolicy::setLayoutPosition(int widgetIndex, M::Position position)
{
    MWidgetController* widget = dynamic_cast<MWidgetController*>(widgetAt(widgetIndex));
    if (widget) {
        widget->setLayoutPosition(position);
    }
}

int MToolBarLayoutPolicy::labelOnlyButtonCount() const
{
    int cnt(0);

    for (int i = 0; i < widgetCount(); i++) {
        MButton *button = dynamic_cast<MButton *>(widgetAt(i));
        if (button &&
            !button->text().isEmpty() && button->iconID().isEmpty() &&
            button->viewType() == MButton::defaultType) {
            cnt++;
        }
    }

    return cnt;
}

