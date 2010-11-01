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

#ifndef MABSTRACTLAYOUTPOLICY_H
#define MABSTRACTLAYOUTPOLICY_H

#include "mexport.h"

#include <QSizeF>
#include <QRectF>

class MAbstractLayoutPolicyStyle;
class MAbstractLayoutPolicyPrivate;
class MLayout;

class QGraphicsLayoutItem;
/*!
 * \class MAbstractLayoutPolicy
 * \brief The MAbstractLayoutPolicy class provides the base class for all MLayout policies.
 * \ingroup layouts
 *
 * MAbstractLayoutPolicy is an abstract class that defines an API for
 * arranging QGraphicsWidget, MWidget and other QGraphicsLayoutItem objects
 * in the associated MLayout.
 *
 * A MLayout can have many different policies, with one of the policies being
 * active at any moment.  Changing a MLayout's active policy causes the items
 * to move into a new position.
 *
 * \section visibility Visibility
 *
 * Items added to a layout will be automatically hidden and shown, depending on whether
 * they are in the currently active policy.  If your widget inherits MWidget then you can still
 * manually hide an item by calling MWidget::hide().

 * \section multiple_policies Multiple Policies
 *
 * A layout can have multiple policies associated with it.  See the MLayout documentation for more information and examples.
 *
 * \sa MLayout, \ref layout-multiplepolicies, \ref layout-inside-layout
 *
 * \section styling_policy CSS Styling of Policies
 *
 * Policies can be styled in the CSS stylesheet files, however the class name is always
 * MAbstractLayoutPolicy.  Their object name can be set with setStyleName() and they
 * can be styled different depending on the current device orientation.
 *
 * For example:
 *
 * \code
 *  MLinearLayoutPolicy mypolicy(mylayout);
 *  mypolicy.setStyleName("address");
 * \endcode
 *  could then be styled in the .css file with
 * \code
 *  MAbstractLayoutPolicyStyle#address {
 *    vertical-spacing: 5;
 *    horizontal-spacing: 5;
 *    margin-left: 10.0;
 *    margin-right: 10.0;
 *    margin-top: 10.0;
 *    margin-bottom: 10.0;
 *  }
 *  MAbstractLayoutPolicyStyle#address.Landscape {
 *    vertical-spacing: 0;
 *  }
 * \endcode
 * This sets the vertical spacing to be 0 pixels in landscape mode, and 5 pixels in portrait mode.
 *
 * \section custom_policy Writing a Custom Policy
 *
 * Implementing new layout policies is a straight forward process. This
 * abstract base class handles registration and deregistration with
 * the layout it is associated with, and by default handles the adding and
 * removing of items from the policy.
 *
 * <table class="qt-style">
 * <tr><th>Function</th><th>Description</th></tr>
 * <tr>
 *    <td>MAbstractLayoutPolicy::addItem()</td>
 *    <td>Add an item to the layout.  The default implementation is usually sufficient (adds the item to the internal list of items, and invalidates the policy and layout).  You may want to implement custom addItem functions that additionally take position, alignment etc parameters.  You must call MAbstractLayoutPolicy::addItem() from any addItem implementation to maintain the internal state.</td>
 * </tr><tr>
 *    <td>MAbstractLayoutPolicy::relayout()</td>
 *    <td>Triggers a relayout of items.  This does the main task of laying out the items.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::sizeHint()</td>
 *     <td>Returns the policy's size hints.  This must enclose all of the items inside</td>
 * </tr>
 * </table>
 * <p>
 *
 * Additionally it may be useful to implement the following functions if the default behaviour is not sufficient:
 * <table class="qt-style">
 * <tr><th>Function</th><th>Description</th></tr>
 * <tr>
 *    <td>MAbstractLayoutPolicy::setHorizontalSpacing()</td>
 *    <td>Sets the horizontal spacing between items.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::setVerticalSpacing()</td>
 *     <td>Sets the vertical spacing between items.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::sizeHint()</td>
 *     <td>Returns the policy's size hints.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::count()</td>
 *     <td> Returns the number of items in your layout.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::itemAt()</td>
 *     <td> Returns a pointer to an item in your layout.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::removeAt()</td>
 *     <td> Removes an item from your layout without destroying it.</td>
 * </tr><tr>
 *     <td>MAbstractLayoutPolicy::invalidate()</td>
 *     <td>Notifies that an item has been added/removed or some other property has changed, requiring items to be laid out.  You may want to override this to invalidate an internal model.</td>
 * </tr>
 * </table>
 *
 * For more details on how to implement each function, refer to the individual
 * function documentation.
 *
 * Each layout defines its own API for arranging widgets and layout items. For
 * example, with a grid layout, you require a row and a column index with optional
 * row and column spans, alignment, spacing, and more. A linear layout, however,
 * requires a single row or column index to position its items. For a grid layout,
 * the order of insertion does not affect the layout in any way, but for a linear
 * layout, the order is essential. When writing your own layout subclass, you are
 * free to choose the API that best suits your layout.
 *
 * \section laying_out_item_in_custom_policy Laying out items in a custom policy
 *
 * For each item in a policy, the policy should at least call the
 * QGraphicsLayoutItem::preferredSize() function to get the item's preferred size.
 * A well-behaved policy will also respect the QGraphicsLayoutItem::sizePolicy() so
 * that items can expand to take up all available space.
 *
 */

class M_CORE_EXPORT MAbstractLayoutPolicy
{
public:
    /** \brief Create a policy and associate it with the given layout
     *
     *  The layout must not be NULL.
     *  The layout takes ownership of the policy, deleting the policy when the layout is deleted.  If the policy
     *  is explicitly deleted, it will automatically be removed from the layout.
     */
    explicit MAbstractLayoutPolicy(MLayout *layout);

    /*!
     * \brief Destroys an abstract layout policy.
     *
     * If the policy was currently active, the next policy in the layout
     * is automatically \link activate() activated\endlink, if available. Items in this
     * policy continue to be owned by the associated MLayout and are
     * \link QGraphicsItem::hide() hidden\endlink if not in the new policy.
     *
     * \sa activate()
     */
    virtual ~MAbstractLayoutPolicy();

    /*!
     * \brief Set the content margins for this policy.
     *
     * This method sets the contents margins for the policy, overriding the values set in the CSS.
     *
     * By default, these are all -1, meaning that the MLayout::getContentsMargins()
     * will be used.  Any that are changed to be a non-negative number will override the
     * value returned by MLayout::getContentsMargins()
     *
     * @param left Left margin.  Set to -1 indicate to use MLayout's left margin
     * @param top Top margin.  Set to -1 indicate to use MLayout's top margin
     * @param right Right margin.  Set to -1 indicate to use MLayout's right margin
     * @param bottom Bottom margin.  Set to -1 indicate to use MLayout's bottom margin
     *
     * To use the values set in the CSS again, call unsetContentsMargins();
     *
     * \sa MLayout::getContentsMargins()
     * \sa MLayout::setContentsMargins()
     * \sa unsetContentsMargins()
     * \sa getContentsMargins()
     */
    void setContentsMargins(qreal left, qreal top, qreal right, qreal bottom);

    /*! \brief Unset the contents margins for this policy, reading the values set in the CSS instead.
     */
    void unsetContentsMargins();

    /*!
     * \brief Get the content margins override for this policy.
     *
     * This method gets the contents margins override for the policy.
     *
     * By default, these are all -1, meaning that the MLayout's getContentsMargins
     * will be used.  Any that are changed to be a non-negative number will override the
     * value returned by MLayout's getContentsMargins
     *
     * @param left Left margin.  -1 indicates to use MLayout's left margin
     * @param top Top margin.  -1 indicates to use MLayout's top margin
     * @param right Right margin.  -1 indicates to use MLayout's right margin
     * @param bottom Bottom margin.  -1 indicates to use MLayout's bottom margin
     */
    void getContentsMargins(qreal *left, qreal *top, qreal *right, qreal *bottom) const;

    /*!
     * \brief Get the layout this policy is associated with.
     */
    MLayout *layout() const;

    /*!
     * \brief Make this policy the active one.
     *
     * This method sets the policy as the one used by the layout to do
     * all the work.
     */
    void activate();

    /*!
     * \brief Whether this is the active policy.
     *
     * Checks whether this policy is the one currently used by the
     * associated layout.
     */
    bool isActive() const;

    /*!
     * \brief Return the sizeHint for this policy.
     *
     * Implementations must ensure that the sizeHint returned encompasses all of
     * the items inside, including taking into account contents margins returned from
     * getContentsMargins()
     *
     * External classes, except for MLayout, should not call this directly,
     * but should use MLayout::effectiveSizeHint()
     */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const = 0;

    /*!
     * \brief Called when the style has been changed.
     *
     * \sa applyStyle()
     */
    void updateStyle();

    /*! \brief Sets the name of this policy, for CSS styling.
     *
     * This is similar to MWidgetController::setStyleName() and is used as
     * the object name for the MStyle object name.
     * For example:
     * \code
     *  MLinearLayoutPolicy mypolicy(mylayout);
     *  mypolicy.setStyleName("address");
     * \endcode
     *  could then be styled in the .css file with
     * \code
     *  MAbstractLayoutPolicy#address {
     *    vertical-spacing: 5;
     *  }
     * \endcode
     *
     * By default the object's name is the empty string.
     *
     * \param name object name
     */
    void setStyleName(const QString &name);

    /*! \brief Return the name of this policy, for CSS styling.
     *
     * This is similar in functionality and usage to MWidgetController::styleName()
     *
     * \sa setStyleName(const QString &)
     */
    QString styleName() const;

    /*!
     * \brief Sets the style name
     * \deprecated Use setStyleName() instead.
     * It simply calls setStyleName().
     */
    void setObjectName(const QString &name);

    /*!
     * \brief returns the style name.
     * \deprecated Use styleName() instead.
     * It simply calls styleName().
     */
    QString objectName() const;

    /*!
     * \brief Set the horizontal spacing.
     *
     * The base implementation sets the horizontal spacing and invalidates the
     * policy, triggering a relayout().  This overrides the value set in the CSS.
     *
     * @param spacing The spacing to use.
     */
    virtual void setHorizontalSpacing(qreal spacing);

    /*!
     * \brief Get the horizontal spacing.
     *
     * @return The spacing to use.
     */
    virtual qreal horizontalSpacing() const;

    /*!
     * \brief Set the vertical spacing.
     *
     * The base implementation sets the horizontal spacing and invalidates the
     * policy, triggering a relayout().  This overrides the value set in the CSS.
     *
     * @param spacing The spacing to use.
     */
    virtual void setVerticalSpacing(qreal spacing);

    /*!
     * \brief Get the vertical spacing.
     *
     * @return The spacing to use.
     */
    virtual qreal verticalSpacing() const;

    /*!
     * \brief Convenience function to set both the horizontal and vertical spacing.
     */
    virtual void setSpacing(qreal spacing);

    /*!
     * \brief Returns the number of items in the policy.
     */
    virtual int count() const;

    /*!
     * \brief Get the item stored in the policy.
     *
     * Get the item found at the given index.
     *
     * @param index The index to check.
     * @return The item at the given index or NULL if the index is invalid.
     */
    virtual QGraphicsLayoutItem *itemAt(int index) const;

    /*!
     * \brief Index of the given item.
     *
     * This function uses the virtual functions count() and itemAt() to find
     * the index of the given item.
     *
     * Note that this takes linear, O(N), time.
     *
     * @param item The item to return the index of
     * @return Index of the item
     */
    int indexOf(const QGraphicsLayoutItem *item) const;

    /*!
     * \brief Remove an item from the policy only.
     *
     * Remove an item from the policy, leaving it in layout.  If the policy is active
     * and the item is a QGraphicsWidget (or MWidget) then the item will be hidden
     * via QGraphicsWidget::hide().
     *
     * Note that layouts themselves are not a QGraphicsWidget and so will not be hidden
     * when removed from another layout.  So if you remove another layout from a policy,
     * it will stay on the screen.  You can solve this by placing the layout inside of a
     * widget, and put that widget in the policy.
     *
     * @param index The policy index of the item to remove.
     */
    virtual void removeAt(int index);

    /*!
     * \brief Remove an item from the policy only.
     *
     * Convienence function
     *
     * @param item The item to remove.
     */
    void removeItem(const QGraphicsLayoutItem *item);

    /*!
     * \brief Returns whether this policy's height depends on its width even if it does not contain any height-for-width items.
     */
    virtual bool hasHeightForWidth() const;

protected:
    /*!
     * \brief Shared d_ptr setup constructor.
     */
    explicit MAbstractLayoutPolicy(MAbstractLayoutPolicyPrivate &);

    /*! \brief Set whether this policy has a height which depends on its width */
    void setHeightForWidth(bool hasHeightForWidth);

    /*!
     * \brief Insert an item in the policy at the given index.
     *
     * Inserts @p item into the layout at @p index, or before any item that is currently at @p index.
     *
     * The base class function is protected since this does not make sense for all policies.  Policies can
     * make this function public where it is suitable, or create their own API.
     *
     * @param item The item to insert.
     * @param index The index to place the item
     */
    virtual void insertItem(int index, QGraphicsLayoutItem *item);

    /*!
     * \brief Add an item to the policy.
     *
     * If the item is a QGraphicsWidget, it will be shown or hidden depending
     * on whether it is in the active policy.
     *
     * Note that the order of the items in the policy is independent of the order of the items
     * in the MLayout.
     *
     * @param item The item to add.
     *
     * \sa \ref layout-inside-layout
     */
    virtual void addItem(QGraphicsLayoutItem *item);

    /*! \brief Set the target geometry of the given item to the given geometry
     *
     *  To be used by inherited policies to set the geometry of the item.  Typically called from
     *  relayout() and addItem().  This does nothing if the policy is not the currently active
     *  policy in the layout.
     *
     *  Note that this causes the item to be shown, if it's being hidden.
     */
    void setItemGeometry(int index, const QRectF &geometry);

    /*! \brief Return the target geometry of the item at the given index
     */
    QRectF itemGeometry(int index) const;

    /*! \brief Hide the given item.
     *
     *  When a policy is made active, any items not in the policy are automatically
     *  hidden.  The rest of the items in the policy are not changed.  To hide
     *  any items that are in the policy, call this function.
     *
     *  To show the item again call the setItemGeometry() function.
     *
     *  To hide items as a user of MLayout, see \ref visibility
     */
    void hideItem(int index);

    /*! \brief Called when the style has changed (e.g the CSS file has been loaded).
     *
     * Inheriting policies reimplementing this should call this base class function.
     * This function can be reimplemented to be read specific attributes in the CSS file.
     * The pointer to the style, containing the updated values, can be accessed via the style() function
     * \sa style()
     */
    virtual void applyStyle();

    /** Return the style for this policy */
    const MAbstractLayoutPolicyStyle *style() const;

    /*!
     * \brief Callback triggered when a relayout is required.
     *
     * This method is triggered whenever complete relayout is required
     * (e.g. when the layout itself is resized).
     *
     * All affected items are computed and the layout is triggered to update
     * the information.
     *
     * You must override this method in your custom policies.
     */
    virtual void relayout() = 0;

    /*! \deprecated Use removeAt() instead.  There shouldn't be any need for this function. Since 0.19
     */
    virtual void Q_DECL_DEPRECATED aboutToBeRemovedFromLayout(const QGraphicsLayoutItem *item);

    /*!
     * \brief Invalidate any cached size and geometry information here and invalidate layout
     * If this policy is the current policy (aka activated) then it invalidates the layout.
     * It then calls invalidate() which can be overridden by base classes to invalidate internal models
     */
    void invalidatePolicyAndLayout();

    /*!
     * \brief Invalidate any cached size and geometry information here.
     * This is called whenever the layout itself is invalidated by MLayout, and also called by
     * invalidatePolicyAndLayout()
     */
    virtual void invalidate();

    /*! This virtual function discards any cached size hint information.
     *  You should always call this function if you change the return value of the
     *  sizeHint() function. Subclasses must always call the base implementation
     *  when reimplementing this function.
     */
    virtual void updateGeometry();

    /*!
     * \brief Get the current content rectangle of the layout.
     *
     * Be aware that these coordinates are not local, this is different to
     * contentsRect()
     */
    QRectF contentsArea() const;

    /*!
     * \brief Called by the layout when the policy is made active.
     *
     * The base class implementation hides all the items not in this policy,
     * and shows all items that are in this policy.
     */
    virtual void activated();

    // shared d_ptr:
    MAbstractLayoutPolicyPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MAbstractLayoutPolicy)
    Q_DECLARE_PRIVATE(MAbstractLayoutPolicy)

    friend class MLayout;
    friend class MLayoutPrivate;
};

#endif // Header Guard
