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

#ifndef MGRIDLAYOUTPOLICY_H
#define MGRIDLAYOUTPOLICY_H

#include "mabstractlayoutpolicy.h"
class MGridLayoutPolicyPrivate;

class QWidget;

/*!
 * \class MGridLayoutPolicy
 * \brief Policy that uses the Qt grid layout engine to provide a grid layout.
 * \ingroup layouts
 *
 * This class provides a policy which is similar to QGraphicsGridLayout, with the advantage
 * of allowing multiple policies and animation.  You can use QGraphicsGridLayout instead
 * to slightly reduce memory overhead if these advantages are not required.

 * The following example adds items to the grid layout policy:
 *
 * \dontinclude mgridlayoutpolicy/mgridlayoutpolicy.cpp
 * \skip Create a MLayout
 * \until }
 *
 * The result, with appropriate CSS styling, looks like:
 * \image html mgridlayoutpolicy.jpg
 *
 * \sa \ref layout-mgridlayoutpolicy
 *
 * \section gridLimitations Limitations - equally-sized columns
 *
 * It is difficult to create a grid layout with equally-sized columns, with both
 * MGridLayoutPolicy and QGraphicsGridLayout.  The current workaround is to
 * manually set the preferred width of all the items inside the layout to the same width.
 *
 * As an example, to layout items in two columns with equal widths:
 *
 * \dontinclude two_columns/twocolumns.cpp
 * \skip Create a MLayout that we set the policy for
 * \until labelLayout->setPreferredWidth(1)
 *
 * We are working with Qt to try to find a better solution.
 *
 * \section qgraphicsgridlayout_instead Using QGraphicsGridLayout instead
 *
 * If you do not need animations or multiple policies, you can use QGraphicsGridLayout for same effect in less code.
 * For example:
 *
 * \dontinclude qgraphicsgridlayout/qgraphicsgridlayout.cpp
 * \skip Create a grid layout
 * \until }
 *
 * \sa \ref layout-qgraphicsgridlayout
 */
class M_CORE_EXPORT MGridLayoutPolicy : public MAbstractLayoutPolicy
{
public:
    /*!
     * \brief Constructs a grid layout policy.
     *
     * @param layout The layout to associate with.
     */
    explicit MGridLayoutPolicy(MLayout *layout);

    /*!
     * \brief Destructor to clean up engine.
     */
    virtual ~MGridLayoutPolicy();

    /*!
     * \brief Get the number of rows.
     *
     * @return The number of rows.
     */
    int rowCount() const;

    /*!
     * \brief Get the number of columns.
     *
     * @return The number of columns.
     */
    int columnCount() const;

    /*!
     * \brief Set spacing for a single row.
     *
     * @param row The row to set the spacing for.
     * @param spacing The spacing to use.
     */
    void setRowSpacing(int row, qreal spacing);

    /*!
     * \brief Get spacing for a single row.
     *
     * @param row The row to get the spacing for.
     *
     * @return The spacing.
     */
    qreal rowSpacing(int row) const;

    /*!
     * \brief Set spacing for a single column.
     *
     * @param column The column to set the spacing for.
     * @param spacing The spacing to use.
     */
    void setColumnSpacing(int column, qreal spacing);

    /*!
     * \brief Get spacing for a single column.
     *
     * @param column The column to get the spacing for.
     *
     * @return The spacing.
     */
    qreal columnSpacing(int column) const;

    /*!
     * \brief Set the default alignment for a whole row.
     *  This value can be overridden on an item for item basis.
     *
     * @param   row       the row to set the default alignment for
     * @param   alignment the default alignment to use
     */
    void setRowAlignment(int row, Qt::Alignment alignment);

    /*!
     * \brief Get the default alignment for a whole row.
     *
     * @param   row       the row to get the alignment for
     * @return  the default alignment in use
     */
    Qt::Alignment rowAlignment(int row) const;

    /*!
     * \brief Set the default alignment for a whole column.
     *  This value can be overridden on an item for item basis.
     *
     * @param   column     the column to set the default alignment for
     * @param   alignment  the default alignment to use
     */
    void setColumnAlignment(int column, Qt::Alignment alignment);

    /*!
     * \brief Get the default alignment for a whole column.
     *
     * @param   column       the row to get the alignment for
     * @return  the default alignment in use
     */
    Qt::Alignment columnAlignment(int column) const;

    /*!
     * \brief Get an item.
     *
     * Get the item found at the given position in the grid.
     *
     * @param row The row to query.
     * @param col The column to query.
     *
     * @return The item at the position or 0 if invalid/unset.
     */
    QGraphicsLayoutItem *itemAt(int row, int col) const;

    /*!
     * \brief Add an item at a position in the grid.
     *
     * @param item The item to add.
     * @param row The row to add the item to.
     * @param column The column to add the item to.
     * @param rowSpan The number of cells the item spans in the row
     * @param columnSpan The number of cells the item spans in the column
     * @param alignment The alignment to use.
     */
    void addItem(QGraphicsLayoutItem *item, int row, int column,
                 int rowSpan, int columnSpan, Qt::Alignment alignment = 0);

    /*!
     * \brief Add an item at a position in the grid.
     *
     * @param item The item to add.
     * @param row The row to add the item to.
     * @param column The column to add the item to.
     * @param alignment The alignment to use.
     */
    inline void addItem(QGraphicsLayoutItem *item, int row, int column, Qt::Alignment alignment = 0) {
        addItem(item, row, column, 1, 1, alignment);
    }

    /*! \reimp */
    virtual void removeAt(int index);
    /*! \reimp_end */

    /*! Sets the stretch factor for \a row. */
    void setRowStretchFactor(int row, int stretch);
    /*! Returns the stretch factor for \a row. */
    int rowStretchFactor(int row) const;
    /*! Sets the stretch factor for \a column. */
    void setColumnStretchFactor(int column, int stretch);
    /*! Returns the stretch factor for \a column. */
    int columnStretchFactor(int column) const;
    /*! Sets the minimum height for \a row to \a height. */
    void setRowMinimumHeight(int row, qreal height);
    /*! Returns the minimum height for \a row. */
    qreal rowMinimumHeight(int row) const;
    /*! Sets the preferred height for \a row. */
    void setRowPreferredHeight(int row, qreal height);
    /*! Returns the preferred height for \a row. */
    qreal rowPreferredHeight(int row) const;
    /*! Sets the maximum height for \a row to \a height. */
    void setRowMaximumHeight(int row, qreal height);
    /*! Returns the maximum height for \a row. */
    qreal rowMaximumHeight(int row) const;
    /*! Sets the fixed height for \a row to \a height. */
    void setRowFixedHeight(int row, qreal height);
    /*! Sets the minimum width for \a column to \a width. */
    void setColumnMinimumWidth(int column, qreal width);
    /*! Returns the minimum width for \a column. */
    qreal columnMinimumWidth(int column) const;
    /*! Sets the preferred width for \a column to \a width. */
    void setColumnPreferredWidth(int column, qreal width);
    /*! Returns the preferred width for \a column. */
    qreal columnPreferredWidth(int column) const;
    /*! Sets the maximum width of \a column to \a width. */
    void setColumnMaximumWidth(int column, qreal width);
    /*! Returns the maximum width for \a column. */
    qreal columnMaximumWidth(int column) const;
    /*! Sets the fixed width of \a column to \a width. */
    void setColumnFixedWidth(int column, qreal width);

    /*!
      Returns the alignment for \a item. The default alignment is
      Qt::AlignCenter.

      The alignment decides how the item is positioned within its assigned space
      in the case where there's more space available in the layout than the
      widgets can occupy.

      \sa setAlignment()
     */
    Qt::Alignment alignment(QGraphicsLayoutItem *item) const;
    /*!
      Sets the alignment of \a item to \a alignment. If \a item's alignment
      changes, the layout is automatically invalidated.

      \sa alignment(), invalidate()
     */
    void setAlignment(QGraphicsLayoutItem *item, Qt::Alignment alignment);


    /*! \reimp */
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setHorizontalSpacing(qreal spacing);
    virtual void setVerticalSpacing(qreal spacing);
    /*! \reimp_end */


protected:
    /*! \reimp */
    virtual void relayout();
    virtual void invalidate();
    /*! \reimp_end */

private:
    Q_DISABLE_COPY(MGridLayoutPolicy)
    Q_DECLARE_PRIVATE(MGridLayoutPolicy)
};

#endif // Header Guard
