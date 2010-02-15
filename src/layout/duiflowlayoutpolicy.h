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

#ifndef DUIFLOWLAYOUTPOLICY_H
#define DUIFLOWLAYOUTPOLICY_H

#include "duiabstractlayoutpolicy.h"
#include "duilayout.h"

class DuiFlowLayoutPolicyPrivate;

/*!
 *  \class DuiFlowLayoutPolicy
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
 *  \dontinclude duiflowlayoutpolicy/duiflowlayoutpolicy.cpp
 *  \skip Create a DuiLayout
 *  \until }
 *
 *  The result, with appropriate CSS styling, looks like:
 *  \image html duiflowlayoutpolicy.png
 *
 *  \sa \ref layout-duiflowlayoutpolicy
 *
 *  \section duibuttonsInFlowLayout Using DuiButton in a DuiFlowLayoutPolicy
 *
 *  In the default CSS theme, a DuiButton has a fixed \link QGraphicsLayoutItem::preferredWidth() preferredWidth()\endlink,
 *  which does not depend on the text inside of it.  This makes sense normally for buttons,
 *  but does not usually produce the desired behaviour in a flow layout.
 *  Every DuiButton in a DuiFlowLayoutPolicy will have the same width.
 *
 */
class DUI_EXPORT DuiFlowLayoutPolicy : public DuiAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a flow layout policy.
     */
    explicit DuiFlowLayoutPolicy(DuiLayout *);

    /*!
     * \brief Destroys a flow layout policy.
     *
     * \sa ~DuiAbstractLayoutPolicy()
     */
    virtual ~DuiFlowLayoutPolicy();

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

    /*! \reimp */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void addItem(QGraphicsLayoutItem *item);
    virtual void insertItem(int index, QGraphicsLayoutItem *item);

    virtual void invalidate();
protected:
    virtual void relayout();
    virtual bool hasHeightForWidth() const {
        return true;
    }
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(DuiFlowLayoutPolicy)
    Q_DECLARE_PRIVATE(DuiFlowLayoutPolicy)
};

#endif // Header Guard
