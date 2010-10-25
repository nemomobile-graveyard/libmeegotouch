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

#ifndef MLAYOUT_H
#define MLAYOUT_H

#include "mexport.h"

#include <QtGui/QGraphicsLayout>
#include <QtGui/QGraphicsWidget>

class QGraphicsItem;
class MAbstractLayoutPolicy;
class MLayoutPrivate;
class MLayoutAnimation;
/*!
 * \class MLayout
 * \brief The magic layout class, extending QGraphicsLayout to add support for easily switching between
 * multiple \link MAbstractLayoutPolicy layout policies \endlink .
 * \ingroup layouts
 *
 * This layout class uses a policy to lay out its child \link QGraphicsLayoutItem QGraphicsLayoutItem's\endlink.
 * A nice feature is that transitions may be animated by use of a
 * MLayoutAnimation.
 *
 * The MLayout owns all its associated policies, deleting them when the layout is deleted.
 *
 * \section relation_to_qgraphicslayout Relation to QGraphicsLayout
 * Use a MLayout if you want the ability to:
 *
 * \li Switch between multiple policies
 * \li Animate the position and opacity of items
 * \li Use one of the policies with no equivalent in QGraphicsLayout (e.g. MFlowLayoutPolicy, MFreestyleLayoutPolicy)
 *
 * Use a QGraphicsLayout if you want:
 *
 * \li A more lightweight layout
 * \li A simple layout with no animation
 *
 * \sa \ref layout-qgraphicslayout
 *
 * \section policies Policies
 * A layout can have multiple policies, with one active at any given time.  With a normal Qt layout,
 * items are added directly to the layout, however with MLayout items are added to the layout's
 * associated policies.  The policies all inherit from QAbstractLayoutPolicy and take care of
 * geometry management for a set of widgets.  To create more complex layouts you can nest MLayouts
 * inside each other or a mix of MLayouts and other QGraphicsLayouts.
 *
 * The following code creates a MLayout with a linear and a flow policy, adding items to both policies.
 * The first policy created will be the default policy.  The other policies can be switched to by calling
 * MAbstractLayoutPolicy::activate().
 *
 *  \dontinclude multiplepolicies/multiplepolicies.cpp
 *  \skip Create a MLayout
 *  \until layout->setPortraitPolicy
 *
 * \sa \ref layout-multiplepolicies
 *
 * \section layoutsInLayouts Layouts inside Layouts
 *
 * Another layout (MLayout or QGraphicsLayout) can be added to a policy to nest layouts to create
 * more complex layouts.  Items which are also layouts are not animated themselves, so prevent
 * any problem of trying to animate a layout which is itself trying to animate the items inside of it.
 *
 * \section visibility Item Visibility
 *
 * Items added to a layout will be automatically hidden and shown, depending on whether
 * they are in the currently active policy.  If your widget inherits MWidget then you can still
 * manually hide an item by calling MWidget::hide().  Usually it is sufficient to just call
 * <code>mywidget->hide()</code>, but sometimes you have a QGraphicsItem or QGraphicsLayoutItem pointer
 * that you must first cast to a MWidget.
 *
 * For \ref layout-hidden_widgets "example":
 *
 * \dontinclude hidden_widgets/hidden_widgets.cpp
 * \skip Create a MLayout
 * \until window.show
 *
 * \sa \ref layout-inside-layout
 *
 * \section main_policies Main Policies
 * \li MLinearLayoutPolicy - For placing items horizontally or vertically
 *
 *     \image html mlinearlayoutpolicy_small.png
 *
 * \li MGridLayoutPolicy - For placing items in a grid
 *
 *     \image html mgridlayoutpolicy_small.png
 *
 * \li MFlowLayoutPolicy - For placing items in a horizontal line, overflowing onto the next line
 *
 *     \image html mflowlayoutpolicy_small.png
 *
 * \li MFreestyleLayoutPolicy - For placing items freely, but still preventing items from overlapping
 *
 *     \image html mfreestylelayoutpolicy_small.png
 *
 * \section styling_layouts Styling
 *
 * CSS styling is done on the policies themselves - see MAbstractLayoutPolicy for more information.
 *
 * \sa \ref styling_policy
 *
 * \section custom_widgets Writing custom widgets
 *
 * For an item to be placed inside of a layout, the QGraphicsLayoutItem::sizeHint() and
 * QGraphicsLayoutItem::sizePolicy() need to return sensible values.
 * The minimum sizeHint should be the minimum size at which the item is usable.  The
 * preferred sizeHint should be the size at which it is usable, and the maximum sizeHint
 * is typically <tt>QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX)</tt>.
 *
 * If you are implementing a custom MWidgetController and MWidgetView, then implement
 * MWidgetView::sizeHint(), leaving the default MWidgetController::sizeHint() function
 * which just calls the view's \link MWidgetView::sizeHint() sizeHint \endlink function.
 *
 * For an item in the layout, the QGraphicsLayoutItem::sizePolicy() function determines
 * how the item should expand when there is space available.  For example, a MButton cannot
 * sensibly make use of more or less vertical space than its preferred height, so its vertical
 * \link QGraphicsLayoutItem::sizePolicy() sizePolicy \endlink would be \link QSizePolicy QSizePolicy::Fixed. \endlink
 *
 * On the other hand, a text editing widget can use all the space that it can get, so its
 * \link QGraphicsLayoutItem::sizePolicy() sizePolicy \endlink would be \link QSizePolicy QSizePolicy::Expanding \endlink
 * in both directions.
 *
 * \sa \ref laying_out_item_in_custom_policy
 *
 * \section examples Examples
 * There are many \ref layouts-example "provided examples" of using layouts to help you use layouts easily and effectively.
 *
 * \sa \ref layouts, \ref layout-inside-layout, \ref example-calculator
 */
class M_CORE_EXPORT MLayout : public QGraphicsLayout
{
public:

    /*!
     * \brief Constructs the layout.
     */
    explicit MLayout(QGraphicsLayoutItem *parent = 0);

    /*!
     * \brief Destructs the layout.
     *
     * This destructor deletes all policies associated with the layout.
     */
    virtual ~MLayout();

    // QGraphicsLayout:
    /*!
     * \brief The number of items handled by this layout.
     *
     * The count includes items currently being animatedly deleted.
     */
    virtual int count() const;

    /*! \brief Returns if there are any items in this layout.
     *
     * This includes items currently being animatedly deleted.
     */
    bool isEmpty() const;

    /**
     * \brief Get the item at the given position.
     *
     * Note that the item may be marked for deletion if animatedDeleteAt()
     * was called.
     */
    virtual QGraphicsLayoutItem *itemAt(int index) const;

    /*!
     * \brief Invalidate the current layout information.
     *
     * This is used to trigger invalidation in the policies.
     */
    virtual void invalidate();

    /*! \internal */
    void Q_DECL_DEPRECATED invalidateLayoutOnly();
    /*! \internal_end */

    // QGraphicsLayoutItem:
    /*!
     * \brief Get the size hints for this layout.
     *
     * This just calls the \link MAbstractLayoutPolicy::sizeHint() sizeHint function\endlink for the current policy.
     */
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    /*!
     * \brief Remove an item handled by this layout.
     *
     * Removes the item immediately from the layout and all of its
     * policies without destroying it, unless animatedDeleteAt() has
     * been called previously for this item.
     * Ownership of the item is transferred to the caller.
     *
     * Some policies allow items to be removed for only that policy.
     * See the appropriate policy API documentation.
     *
     * Note that the item will remain on the scene, remaining visible.
     *
     * To remove the item from the layout immediately and delete it, simply do:
     *
     * \code
     *   delete item;
     * \endcode
     *
     * and the item will be removed automatically from the layout.
     *
     * To remove the item from the layout and the scene, or to hide the item,
     * see takeAt().
     *
     * @param item  The item to remove.
     */
    void removeItem(const QGraphicsLayoutItem *const item);

    /*!
     * \brief Remove the item at the given position.
     *
     * Removes the item immediately from the layout and all of its
     * policies without destroying it, unless animatedDeleteAt() has
     * been called previously for this item.
     * Ownership of the item is transferred to the caller.
     *
     * Some policies allow items to be removed for only that policy.
     * See the appropriate policy API documentation.
     *
     * Note that the item will remain on the scene, remaining visible.
     * See the removeItem() and
     * takeAt() functions for more information.
     *
     * \sa takeAt(), removeItem()
     */
    virtual void removeAt(int index);

    /*! \brief Return and remove the item at the given position.
     *
     * Removes the item immediately from the layout and all of its
     * policies without destroying it, unless animatedDeleteAt() has
     * been called previously for this item.
     * Ownership of the item is transferred to the caller.
     *
     * Note:
     * If animatedDeleteAt() has been called previously for this item
     * this will return NULL.
     *
     * Some policies allow items to be removed for only that policy.
     * See the appropriate policy API documentation.
     *
     * This can be used to delete all items from the layout like so:
     *
     * \code
     *   while(!layout->isEmpty())
     *       delete takeAt(0);
     * \endcode
     *
     * And it can be used to remove an item from the layout and hide it:
     *
     * \code
     *   if( (QGraphicsItem *item = layout->takeAt(0)->graphicsItem()) )
     *       item->hide;
     * \endcode
     *
     * or to remove and hide all the items:
     *
     * \code
     *   while(!layout->isEmpty())
     *       if( (QGraphicsItem *item = layout->takeAt(0)->graphicsItem()) )
     *           item->hide;
     * \endcode
     *
     *
     * If you want to just remove and delete a single item, simply delete the item
     * and it will be removed automatically from the layout.
     *
     * \sa removeItem(), removeAt()
     */
    virtual QGraphicsLayoutItem *takeAt(int index);

    /*!
     * \brief Remove and delete the item at the given position.
     *
     * The item is removed by the animation and then deleted.
     * This allows for an animation when removing the item.
     * The item is removed from the layout and all of its policies.
     *
     * Note that count() will still include this item until it is
     * deleted, but this behavior may change in future revisions.
     * Likewise the item can still be referenced with \link itemAt() itemAt(index) \endlink
     * until the animation has finished.
     *
     * It is currently not possible to animatedly remove an item
     * without deleting it.
     *
     * If the item is itself a layout, the layout will be deleted but
     * the children inside will not be, and their ownership is transferred
     * to the caller.
     */
    virtual void animatedDeleteAt(int index);

    /*!
     * \brief Remove and delete the item at the given position.
     *
     * Calls animatedDeleteAt()
     */
    virtual void animatedDeleteItem(const QGraphicsLayoutItem *item);

    /*!
     * \brief Find the index of the given graphics item.
     *
     * @param item The graphics item to find.
     *
     * @return The index the item is stored at or -1 if it was not found.
     */
    int indexOf(const QGraphicsItem *item) const;

    /*!
     * \brief Find the index of the given graphics item.
     *
     * @param item The graphics item to find.
     *
     * @return The index the item is stored at or -1 if it was not found.
     */
    inline int indexOf(const QGraphicsWidget *item) const {
        return indexOf(static_cast<const QGraphicsLayoutItem *>(item));
    }

    /*!
     * \brief Find the index of the given graphics layout item.
     *
     * @param item The graphics layout item to find.
     *
     * @return The index the item is stored at or -1 if it was not found.
     */
    int indexOf(const QGraphicsLayoutItem *item) const;

    /*!
     * \brief Set the active policy applied to this layout.
     *
     * This activates the given, already associated, policy in a layout with multiple
     * policies.
     *
     * The policy encapsulates the actual layout logic. Switching a policy
     * triggers a relayout() of all objects handled by the layout.  The first
     * policy created for a layout is automatically made active - there's no
     * requirement to call this function if you only have one policy for a layout.
     *
     * Setting a policy that is not associated with this layout triggers an
     * assertion.
     *
     * This does nothing if @p policy is NULL.
     *
     * This is equivalent to calling MAbstractLayoutPolicy::activate().
     *
     * @param policy  The policy to use for all layout operations.
     */
    void setPolicy(MAbstractLayoutPolicy *policy);

    /*! \brief Convenience function to set the policy used in landscape mode.
     *
     *  When the orientation of the device changes to landscape, the layout will
     *  set the given policy as active, if given.
     *
     *  The default is NULL - the policy is not changed when the device is rotated.
     *
     *  If the device is currently in landscape orientation and @p policy is not NULL, then the @p policy is immediately activated.
     */
    void setLandscapePolicy(MAbstractLayoutPolicy *policy);

    /*! \brief The policy to be used in landscape mode.
     *
     * The default is NULL - the policy is not changed when the device is rotated.
     */
    MAbstractLayoutPolicy *landscapePolicy() const;

    /*! \brief Convenience function to set the policy used in portrait mode.
     *
     *  When the orientation of the device changes to portrait, the layout will
     *  set the given policy as active, if given.
     *
     *  The default is NULL - the policy is not changed when the device is rotated.
     *
     *  If the device is currently in portrait orientation and @p policy is not NULL, then the @p policy is immediately activated.
     */
    void setPortraitPolicy(MAbstractLayoutPolicy *policy);

    /*! \brief The policy to be used in portrait mode.
     *
     * The default is NULL - the policy is not changed when the device is rotated.
     */
    MAbstractLayoutPolicy *portraitPolicy() const;

    /*!
     * \brief Get the currently active policy.
     *
     * @return The currently active policy, or NULL only if there are no associated policies.
     */
    MAbstractLayoutPolicy *policy() const;

    /*!
     * \brief Get a list of all policy objects associated with this layout.
     *
     * @return A list of all policies associated with this layout.  None of the pointers will be NULL.
     */
    QList<MAbstractLayoutPolicy *> registeredPolicies() const;

    /*!
     * \brief Set the animation to be used for state transitions.
     *
     * If @p animation is NULL then no animations will occur - all items will jump into the new
     * states without any transitions.
     *
     * Any previous animation is automatically deleted.
     *
     * @param animation  The animation to be used for all state transitions.
     *
     * \sa animation()
     */
    void setAnimation(MLayoutAnimation *animation);

    /*!
     * \brief Get the current animation.
     *
     * If this is NULL then no animations will occur - all items will jump into the new
     * states without any transitions.
     *
     * @return The currently set up animation.
     * \sa setAnimation()
     */
    MLayoutAnimation *animation() const;

    /*!
     * \brief Set the default contents margins for the layout.
     *
     * This method sets the contents margins for the layout and can be overridden by the
     * policies.
     *
     * If the current \link MAbstractLayoutPolicy::getContentsMargins() policy's getContentsMargins() \endlink
     * returns -1 for any of the four parameters, the layout's content margins, set by this
     * setContentsMargins() function, are used for those parameters instead.
     *
     * For example:
     * \code
     * MLayout *layout = new MLayout;
     * MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
     * policy->setContentsMargins(-1, 0, -1, 0);
     * layout->setContentsMargins(5, 5, 5, 5);
     * qreal left, top, right, bottom;
     * layout->getContentsMargins(left, top, right, bottom);
     * // Now left = 5, top = 0, right = 5, bottom = 0
     * \endcode
     *
     * @param left Left margin.
     * @param top Top margin.
     * @param right Right margin.
     * @param bottom Bottom margin.
     *
     * \sa getContentsMargins(), MAbstractLayoutPolicy::setContentsMargins(), MAbstractLayoutPolicy::getContentsMargins()
     */
    virtual void setContentsMargins(qreal left, qreal top, qreal right, qreal bottom);

    /*!
     * \brief Get the contents margins for the current layout and policy.
     *
     * For each of the parameters, this returns the value returned by the
     * current \link MAbstractLayoutPolicy::getContentsMargins() policy's getContentsMargins()\endlink, unless that
     * returns -1, in which case the value set by setContentsMargins() is used for that parameter.  If
     * neither have been set, the margins are set by the current QStyle.
     *
     * For example:
     * \code
     * MLayout *layout = new MLayout;
     * MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
     * policy->setContentsMargins(-1, 0, -1, 0);
     * layout->setContentsMargins(5, 5, 5, 5);
     * qreal left, top, right, bottom;
     * layout->getContentsMargins(left, top, right, bottom);
     * // Now left = 5, top = 0, right = 5, bottom = 0
     * \endcode
     *
     * @param left Left margin.
     * @param top Top margin.
     * @param right Right margin.
     * @param bottom Bottom margin.
     *
     * \sa setContentsMargins(), MAbstractLayoutPolicy::setContentsMargins(), MAbstractLayoutPolicy::getContentsMargins()
     */
    virtual void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const;

    /*!
     * \brief Returns the direction the items will be laid out in.
     *
     * For example, layouts in a right-to-left language,
     * such as Arabic or Hebrew, are usually laid out from right to left.
     *
     * If the layout is in a QGraphicsWidget, the layout direction is
     * the QGraphicsWidget::layoutDirection().  Otherwise the QApplication::layoutDirection()
     * is used.
     *
     * \sa QGraphicsWidget::layoutDirection(), QApplication::layoutDirection(), Qt::LayoutDirection
     */
    Qt::LayoutDirection layoutDirection() const;

    /*! \reimp */
    virtual void widgetEvent ( QEvent * e );
    /*! \reimp_end */

protected:
    /*!
     * \brief Internal function to add a new item to this layout
     *
     * This is called by MAbstractLayoutPolicy when an item is added to the policy.
     *
     * For a user of MLayout, this function is protected.  To add items to the layout,
     * add them to the policy instead.
     *
     * The layout takes ownership of the item.  If \link QGraphicsLayoutItem::ownedByLayout item->ownedByLayout()\endlink is true,
     * the item will be deleted when the layout is deleted.
     *
     * The item will never be deleted by a removeAt() call and will always be deleted
     * by an animatedDeleteAt() call.
     *
     * If the item was already added, this returns the index of its current position.
     *
     * @param item  The item to add.
     * @return Index of item whether it was added or not, or -1 if @p item is NULL
     */
    virtual int addItem(QGraphicsLayoutItem *item);

    /** \internal
     * This is a protected function since setGeometry() should not be called directly.  The layout
     * should be placed inside of another widget, and that widget's \link QGraphicsItem::setGeometry() geometry \endlink set.
     * The idea is just to prevent some bugs that can be difficult to track down.
     *
     * This function can be called anyway, if really necessary, by:
     *
     * \code
     *   QGraphicsLayout *layout = someMLayout;
     *   layout->setGeometry(rect);
     * \endcode
     */
    virtual void setGeometry(const QRectF &rect);
    /** \internal_end */

    /** \internal
     * Private pointer */
    MLayoutPrivate *const d_ptr;
    /** \internal_end */

private:
    Q_DISABLE_COPY(MLayout)
    Q_DECLARE_PRIVATE(MLayout)


    friend class MAbstractLayoutPolicy;
    friend class MLayoutAnimation;
    friend class MBasicLayoutAnimationPrivate;
    friend class Ut_MLayout;
};

#endif // Header Guard
