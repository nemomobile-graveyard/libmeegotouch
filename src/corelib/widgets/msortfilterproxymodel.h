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

#ifndef MSORTFILTERPROXYMODEL_H
#define MSORTFILTERPROXYMODEL_H

#include <MExport>
#include <QSortFilterProxyModel>

class MSortFilterProxyModelPrivate;

/*!
    \class MSortFilterProxyModel
    \brief MSortFilterProxyModel reimplementation of a custom sort/filter proxy data model.
           In case of filtering does not filter out the group headers (oposite to default
           QSortFilterProxyModel behavior).
*/
class M_CORE_EXPORT MSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    /*!
      \brief Constructor.
      \param parent Model owner.
    */
    MSortFilterProxyModel(QObject *parent = NULL);

    /*!
      \brief Destructor.
    */
    virtual ~MSortFilterProxyModel();

    /*!
      \brief Enable or disable locale-aware sorting.

      Sorting string data that are visible to the user should be done locale-aware since there are
      different sorting rules in different languages. For example, in Czech, words starting with
      "ch" are sorted after words starting with "h". By default, MSortFilterProxyModel does
      this. This function can turn this off (or on again).

      Disabling locale-aware sorting could be useful if there are only non-string data to be sorted,
      for example numbers, dates or times. This can improve performance slightly.
    */
    void enableLocaleAwareSorting(bool enable=true);

    /*!
      \brief Check if locale-aware sorting is enabled.
     */
    bool isLocaleAwareSortingEnabled() const;

    //! \reimp
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual void setSourceModel(QAbstractItemModel *sourceModel);
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    //! \reimp_end

Q_SIGNALS:
    /*!
      \param parent Parent index in which the rows were inserted.
      \param first First inserted row index.
      \param last Last inserted row index.
      \param animated Flag that shows if the insert operation was with animations or not.
    */
    void rowsInserted(const QModelIndex &parent, int first, int last, bool animated);

    /*!
      \param parent Parent index in which the rows were removed.
      \param first First removed row index.
      \param last Last removed row index.
      \param animated Flag that shows if the remove operation was with animations or not.
    */
    void rowsRemoved(const QModelIndex &parent, int first, int last, bool animated);

    /*!
      \param The signal is emitted if the layout change shall be animated.
    */
    void layoutAboutToBeAnimated();

private:
    bool filterAcceptsGroup(const QModelIndex &source_index) const;

    Q_PRIVATE_SLOT(d_func(), void _q_layoutAboutToBeAnimated())
    Q_PRIVATE_SLOT(d_func(), void _q_rowsInserted(QModelIndex,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoved(QModelIndex,int,int))

    Q_PRIVATE_SLOT(d_func(), void _q_resetAnimatedChange())

private:
    MSortFilterProxyModelPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MSortFilterProxyModel)
};

#endif // MSORTFILTERPROXYMODEL_H
