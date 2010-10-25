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

#ifndef MFREESTYLELAYOUTPOLICY_H
#define MFREESTYLELAYOUTPOLICY_H

#include "mabstractlayoutpolicy.h"
#include "mlayout.h"

class MFreestyleLayoutPolicyPrivate;

/*!
 * \class MFreestyleLayoutPolicy
 * \brief Freestyle layout policy.
 *
 * This policy arranges the items as much as possible according to the user's
 * wishes but avoids any overlap. The height is computed as needed.
 *
 * The following example adds items to the freestyle layout policy in a circle shape:
 *
 * \dontinclude mfreestylelayoutpolicy/mfreestylelayoutpolicy.cpp
 * \skip Create a MLayout
 * \until }
 *
 * The result, with appropriate CSS styling, looks like:
 * \image html mfreestylelayoutpolicy.jpg
 *
 * \sa \link layout-mfreestylelayoutpolicy MFreestyleLayoutPolicy Example \endlink
 */
class M_CORE_EXPORT MFreestyleLayoutPolicy : public MAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a freestyle layout policy
     */
    explicit MFreestyleLayoutPolicy(MLayout *layout);

    /*!
     * \brief Destroys the freestyle layout policy.
     */
    virtual ~MFreestyleLayoutPolicy();

    /*!
     * \brief Add an item to the top left hand corner in the freestyle layout.
     *
     * If the new item overlaps any existing items, the existing items will be
     * moved out of the way.
     *
     * The item will be given its preferredSize.
     * @param item The item to add.
     */
    virtual void addItem(QGraphicsLayoutItem *item);

    /*!
     * \brief Add an item at a specific geometry in the freestyle layout.
     *
     * If the new item overlaps any existing items, the existing items will be
     * moved out of the way.
     *
     * @param item The item to add.
     * @param geom The geometry where to add the item.
     */
    void addItemAtGeometry(QGraphicsLayoutItem *item, const QRectF &geom);

    /*! \brief Move an item to the given geometry.
     *
     * The given @a item will be animated to the given geometry, if possible,
     * moving any overlapping items out of the way, if possible.
     */
    void moveItemTo(QGraphicsLayoutItem *item, const QRectF &geom);

    /*! \reimp */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void removeAt(int index);
    /*! \reimp_end */
protected:

    virtual void relayout();

private:
    Q_DECLARE_PRIVATE(MFreestyleLayoutPolicy)
    Q_DISABLE_COPY(MFreestyleLayoutPolicy)
};

#endif // Header Guard
