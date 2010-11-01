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

#ifndef MFLOWLAYOUTPOLICY_H
#define MFLOWLAYOUTPOLICY_H

#include "mabstractlayoutpolicy.h"
#include "mlayout.h"

class MFlowLayoutPolicyPrivate;

/*!
 *  \class MFlowLayoutPolicy
 *
 *  \brief This policy implements a simple flow layout.
 *  \ingroup layouts
 *
 *  The items in the policy are organised into rows, starting from the top left.
 *  The size of each item is initially determined by its
 *  \link QGraphicsLayoutItem::preferredSize() preferredSize()\endlink.
 *  Items with an expanding \link QGraphicsLayoutItem::sizePolicy() sizePolicy()\endlink
 *  are then expanded horizontally and vertically to fill any left over available space.
 *
 *  The following example adds 10 labels to the flow layout policy:
 *
 *  \dontinclude mflowlayoutpolicy/mflowlayoutpolicy.cpp
 *  \skip Create a MLayout
 *  \until }
 *
 *  The result, with appropriate CSS styling, looks like:
 *  \image html mflowlayoutpolicy.jpg
 *
 *  \sa \ref layout-mflowlayoutpolicy
 *
 *  \section mbuttonsInFlowLayout Using MButton in a MFlowLayoutPolicy
 *
 *  In the default CSS theme, a MButton has a fixed \link QGraphicsLayoutItem::preferredWidth() preferredWidth()\endlink,
 *  which does not depend on the text inside of it.  This makes sense normally for buttons,
 *  but does not usually produce the desired behaviour in a flow layout.
 *  Every MButton in a MFlowLayoutPolicy will have the same width.
 *
 */
class M_CORE_EXPORT MFlowLayoutPolicy : public MAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a flow layout policy.
     */
    explicit MFlowLayoutPolicy(MLayout *);

    /*!
     * \brief Destroys a flow layout policy.
     *
     * \sa ~MAbstractLayoutPolicy()
     */
    virtual ~MFlowLayoutPolicy();

    /*!
     * \brief Return the number of rows that the items in the flow layout occupy in its current geometry.
     *
     * The number of rows depends on the layout's current geometry, and the layout's geometry is not usually
     * set until show() has been called.  This will not be greater than rowLimit(), if set.
     *
     * If the policy is not currently active, and has since been \link invalidate() invalidated\endlink, this will return -1.
     * If the policy is currently active and has been invalidated, this will trigger a relayout.
     *
     * @return The number of rows in the current geometry, or -1 if \link invalidate() invalidated\endlink and not active
     */
    int rowCount();

    /*! \brief Set the maximum number of rows to show.
     *
     *  No more than @p rowLimit rows will be shown.  The rest of the items will be hidden.
     *
     *  The default is -1, indicating no row limit */
    void setRowLimit(int rowLimit);

    /*! \brief Return the maximum number of rows to show.
     *
     *  No more than @p rowLimit rows will be shown.  The rest of the items will be hidden.
     *
     *  The default is -1, indicating no row limit */
    int rowLimit() const;

    /*! \brief Returns the alignment for @p item.
     *
     *  The default alignment is Qt::AlignCenter.
     *
     *  The alignment decides how the item is positioned within its assigned space in the
     *  case where there's more space available in the layout than the widgets can occupy.
     *
     *  \see setAlignment()
     */
    Qt::Alignment alignment( QGraphicsLayoutItem * item ) const;

    /*! \brief Sets the alignment of item to \p alignment.
     *
     * If item's alignment changes, the layout is automatically invalidated.
     * \see alignment()
     */
    void setAlignment( QGraphicsLayoutItem * item, Qt::Alignment alignment );

    /*! \reimp */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void addItem(QGraphicsLayoutItem *item);
    virtual void insertItem(int index, QGraphicsLayoutItem *item);
    virtual void removeAt(int index);
    virtual void invalidate();
protected:
    virtual void relayout();
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(MFlowLayoutPolicy)
    Q_DECLARE_PRIVATE(MFlowLayoutPolicy)
};

#endif // Header Guard
