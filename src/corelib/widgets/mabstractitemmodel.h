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
 
#ifndef MABSTRACTITEMMODEL_H
#define MABSTRACTITEMMODEL_H

#include <MExport>
#include <QAbstractItemModel>

class MAbstractItemModelPrivate;

/*!
    \class MAbstractItemModel
    \brief MAbstractItemModel implementation of a simple list data model.
*/
class M_CORE_EXPORT MAbstractItemModel : public QAbstractItemModel
{
    Q_OBJECT
    /*!
        \property MAbstractItemModel::grouped
        \brief True if the model is a grouped (tree) model, false if the model is
               a plain list model.
    */
    Q_PROPERTY(bool grouped READ isGrouped WRITE setGrouped)

public:
    /*!
      \brief Constructor.
      \param parent Item model owner.
    */
    MAbstractItemModel(QObject *parent = NULL);
    /*!
      \brief Destructor.
    */
    virtual ~MAbstractItemModel();

    /*!
      \return Number of groups in the model. (O - for the plain list model).
    */
    virtual int groupCount() const = 0;
    
    /*!
      \return Number of rows in the group.
    */
    virtual int rowCountInGroup(int group) const = 0;

    /*!
      \param group The group index.
      \return Title of the group.
    */
    virtual QString groupTitle(int group) const = 0;

    /*!
      \param row The row index in the group.
      \param group The group index in the model, (-1 - for the plain list model).
      \param role The item data role request.
      \return The item data for the specified role.
    */
    virtual QVariant itemData(int row, int group, int role = Qt::DisplayRole) const = 0;

    /*!
      \brief Set's the grouping mode for the data model.
      \param mode The grouping mode of the model to set.
    */
    void setGrouped(bool mode);
    
    /*!
      \return The model grouping mode.
    */
    bool isGrouped() const;

    //! \reimp
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;    
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    //! \reimp_end
    
protected:
    //! \reimp
    int columnCount(const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    //! \reimp_end

    /*!
      \param parent The parent index where the row insertion begins.
      \param first The first row index.
      \param last The last row index.
      \param animated Flag to insert rows with or without animations.
    */
    void beginInsertRows(const QModelIndex &parent, int first, int last, bool animated);    
    
    /*!
      \param parent The parent index from which the rows are going to be removed.
      \param first The first row index.
      \param last The last row index.
      \param animated Flag to remove rows with or without animations.
    */  
    void beginRemoveRows(const QModelIndex &parent, int first, int last, bool animated);
    
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
    Q_DISABLE_COPY(MAbstractItemModel)
    Q_DECLARE_PRIVATE(MAbstractItemModel)

    //! \internal
    MAbstractItemModelPrivate *const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void _q_rowsInsertAnimated(QModelIndex,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_rowsRemoveAnimated(QModelIndex,int,int))
    Q_PRIVATE_SLOT(d_func(), void _q_layoutChanged())
    //! \internal_end

#ifdef UNIT_TEST
    // Test unit is defined as a friend of production code to access private members
    friend class Ut_MList;
#endif
};

#endif // MABSTRACTITEMMODEL_H
