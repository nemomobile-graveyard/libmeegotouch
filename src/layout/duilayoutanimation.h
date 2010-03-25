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

#ifndef DUILAYOUTANIMATION_H
#define DUILAYOUTANIMATION_H

#include "duianimation.h"
#include "duiexport.h"
#include "duiitemstate.h"
#include "duilayout.h"
class DuiLayoutAnimationPrivate;

/*!
 * \class DuiLayoutAnimation
 * \brief Animation interface for animating adding and removing of items and animating
 * switching between policies in a DuiLayout.
 *
 * Note that the item to animate will only be inherited from QGraphicsWidget. Non-widgets are not animated.
 *
 * \section custom_animation How to write a custom animation
 */
class DUI_EXPORT DuiLayoutAnimation : public DuiAnimation
{
public:
    /*!
     * \brief Construct the layout animator.
     */
    explicit DuiLayoutAnimation(DuiLayout *layout);
    /*!
     * \brief Destroys the layout animator.
     */
    virtual ~DuiLayoutAnimation();

    /*!
     * \brief Return a pointer to the associated layout
     */
    DuiLayout *layout() const;

    virtual void itemRemovedFromLayout(int index) = 0;
    virtual void itemAddedToLayout(int index) = 0;
    virtual void animatedDeleteItem(int index) = 0;
    virtual void setItemGeometry(int index, const QRectF &geometry) = 0;
    virtual void hideItem(int index) = 0;

    /*!
     * \brief Return whether the animator is currently animating any items.
     */
    bool isAnimating() const;


protected:

    /*! \brief Hide the given item.
     *
     *  Dervived classes should call this function to hide the given item, rather than
     *  calling QGraphicsItem::hide() on the item directly. */
    void hideItemNow(QGraphicsWidget *item);

    /*! \brief Show the given item if it was hidden by the layout.
     *
     *  Dervived classes should call this function to show the given item, rather than
     *  calling QGraphicsItem::show() on the item directly.  */
    void showItemNow(QGraphicsWidget *item);

    /** \internal
     * Shared d_ptr setup constructor. */
    explicit DuiLayoutAnimation(DuiLayoutAnimationPrivate &, DuiLayout *layout);
    /** \internal_end */

    DuiLayoutAnimationPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiLayoutAnimation)
    Q_DECLARE_PRIVATE(DuiLayoutAnimation)
};

#endif // Header Guard
