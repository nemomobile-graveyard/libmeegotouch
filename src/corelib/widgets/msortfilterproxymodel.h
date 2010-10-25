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

    //! \reimp
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    //! \reimp_end

private:
    bool filterAcceptsGroup(const QModelIndex &source_index) const;
};

#endif // MSORTFILTERPROXYMODEL_H
