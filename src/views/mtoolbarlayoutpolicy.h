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

#ifndef MTOOLBARLAYOUTPOLICY_H
#define MTOOLBARLAYOUTPOLICY_H

#include "mnamespace.h"
#include "mlinearlayoutpolicy.h"

class QGraphicsWidget;
class QGraphicsLayoutItem;
class MLayout;


//! \internal
/*! Make a layout policy for aligning widgets nicely by adding spacers.
 * This is a very crude policy, and doesn't support most
 * of the functions of MLinearLayoutPolicy
 */
class MToolBarLayoutPolicy : public MLinearLayoutPolicy
{
public:
    static const int unlimitedCapacity = 0xffff;

    explicit MToolBarLayoutPolicy(MLayout *layout);
    ~MToolBarLayoutPolicy();

    /*!
     * \brief Insert widget into tool bar and remove that will overflow (won't fit at the end).
     */
    void insertWidgetAndRemoveOverflow(int widgetIndex, QGraphicsLayoutItem *item);

    /*!
     * \brief Remove the widget indicated by the index.
     */
    void removeWidgetAt(int widgetIndex);

    /*!
     * \brief Remove the widget.
     */
    void removeWidget(const QGraphicsLayoutItem *item);

    /*!
     * \brief Check if there is room for new widget in indicated index.
     */
    bool roomForWidget(int widgetIndex, bool textEdit) const;

    /*!
     * \brief Number of widgets.
     */
    int widgetCount() const;

    /*!
     * \brief Number of occupied widget slots.
     */
    int takenSlots() const;

    /*!
     * \brief Gets index of indicated widget.
     */
    int widgetIndexOf(const QGraphicsLayoutItem *item) const;

    /*!
     * \brief Gets widgets at indicated index.
     */
    QGraphicsLayoutItem *widgetAt(int widgetIndex) const;

    /*!
     * \brief Maximum number of slots.
     */
    int maxWidgetSlots() const;

    /*!
     * \brief Maximum number of slots that takes into account if the tool bar doesn't have its maximum space is use.
     */
    int effectiveMaxWidgetSlots() const;

    /*!
     * \brief Sets maximum number of slots.
     */
    void setMaxWidgetSlots(int maxSlotCount);

    /*!
     * \brief Number of free slots.
     */
    int freeWidgetSlots() const;

    /*!
     * \brief Enable/disable empty spaces between buttons.
     */
    void setSpacesBetween(bool enabled);

    /*!
     * \brief Sets prereserved slots.
     *  Qt::Alignment is used like this:
     *  AlignLeft = no prereserved slots on left
     *  AlignRight = no prereserved slots on right
     *  AlignJustify = no prereserved slots on left and right
     *  AlignHCenter = preseserved slots on left and right
     */
    void setWidgetAlignment(Qt::Alignment alignment, bool affectCapacity);

protected:
    /** Hide the normal functions since these include the spacers
     *  and we want to encourage the user to only use our functions */
    //! \reimp
    virtual int count() const;
    virtual QGraphicsLayoutItem *itemAt(int policyIndex) const;
    virtual void insertItem(int policyIndex, QGraphicsLayoutItem *item);
    void indexOf(const QGraphicsLayoutItem *item) const;
    virtual void removeAt(int policyIndex);

    virtual void activated();
    virtual void relayout();
    //! \reimp_end

private:
    int textEditIndex;
    int maxSlots;
    bool spacesBetween;
    Qt::LayoutDirection layoutDirection;
    QGraphicsWidget *leftSpacer;
    QGraphicsWidget *rightSpacer;
    Qt::Alignment widgetAlignment;
    bool widgetAlignmentAffectsCapacity;

    /*!
     * \brief Converts index of widget to matching index in policy.
     */
    int widgetIndexToPolicyIndex(int widgetIndex) const;

    /*!
     * \brief Converts policy index to matching index in widget.
     */
    int policyIndexToWidgetIndex(int policyIndex) const;

    /*!
     * \brief Inserts spacer to indicated policy index.
     */
    QGraphicsWidget *insertSpacer(int policyIndex);

    /*!
     * \brief Removes widgets that don't to tool bar anymore.
     */
    void removeOverflownWidgets();

      /*!
     * \brief Returns true if widgets should be aligned to the left side or justified.
     */
    bool isAlignedLeftOrJustified() const;

    /*!
     * \brief Returns true if widgets should be aligned to the right side or justified.
     */
    bool isAlignedRightOrJustified() const;

    /*!
     * \brief Returns true if widgets should be justified.
     */
    bool isJustifiedAlignment() const;

    /*!
     * \brief Updates spacers on the both sides.
     */
    void updateSideSpacers();

    /*!
     * \brief Updates left side spacer.
     */
    void updateLeftSpacer();

    /*!
     * \brief Updates right side spacer.
     */
    void updateRightSpacer();

    /*!
     * \brief Updates layout position of widgets at indicated index and those that its addition is affecting.
     */
    void updateLayoutPositionDueToInsert(int widgetIndex);

    /*!
     * \brief Updates layout position of widgets at indicated index and those that its removing is affecting.
     */
    void updateLayoutPositionDueToRemove(int widgetIndex);

    /*!
     * \brief Updates layout positions of all widgets in the tool bar.
     */
    void updateAllLayoutPositions();

    /*!
     * \brief Sets layout position of widget at index.
     */
    void setLayoutPosition(int widgetIndex, M::Position position);
};
//! \internal_end

#endif
