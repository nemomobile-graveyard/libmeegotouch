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

#ifndef DUIFREESTYLELAYOUTPOLICY_H
#define DUIFREESTYLELAYOUTPOLICY_H

#include "duiabstractlayoutpolicy.h"
#include "duilayout.h"

class DuiFreestyleLayoutPolicyPrivate;

/*!
 * \class DuiFreestyleLayoutPolicy
 * \brief Freestyle layout policy.
 *
 * This policy arranges the items as much as possible according to the user's
 * wishes but avoids any overlap. The height is computed as needed.
 *
 * The following example adds items to the freestyle layout policy in a circle shape:
 *
 * \dontinclude duifreestylelayoutpolicy/duifreestylelayoutpolicy.cpp
 * \skip Create a DuiLayout
 * \until }
 *
 * The result, with appropriate CSS styling, looks like:
 * \image html duifreestylelayoutpolicy.png
 *
 * \sa \link layout-duifreestylelayoutpolicy DuiFreestyleLayoutPolicy Example \endlink
 */
class DUI_EXPORT DuiFreestyleLayoutPolicy : public DuiAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a freestyle layout policy
     */
    explicit DuiFreestyleLayoutPolicy(DuiLayout *layout);

    /*!
     * \brief Destroys the freestyle layout policy.
     */
    virtual ~DuiFreestyleLayoutPolicy();

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
    Q_DECLARE_PRIVATE(DuiFreestyleLayoutPolicy)
    Q_DISABLE_COPY(DuiFreestyleLayoutPolicy)
};

#endif // Header Guard
