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

#ifndef MLISTCELLCREATOR_H__
#define MLISTCELLCREATOR_H__

#include <QModelIndex>
#include <QSizeF>
#include <MWidgetRecycler>
#include <MWidget>

/*!
    \class MCellCreator
    \brief Interface for creating and updating items (cells) for MList.

    \section MCellCreator Overview
        MCellCreator is a purely abstract class and should be implemented by every class whose instances
        are intended to create items for MList.

        Main purpose of this interface is to create and update data of widgets which has to be inserted
        into MList. Also MCellCreator returns size of a list item widget.
  */
class M_CORE_EXPORT MCellCreator
{
public:
    /*!
      Default virtual destructor.
      */
    virtual ~MCellCreator() {}

    /*!
      \brief When MList needs a cell, it will call this function to get a pointer to a widget.
      MList keeps ownership of a pointer and will delete object when it's not needed.
      It's not required but highly recommended to use recycler. This function will be called very
      often during panning and to make panning smooth creator shouldn't allocate memory.

      Function should return fully constructed and initialized object. All data has to be set.

      \param index is a model index of the row for which widget should be created
      \param recycler is a MWidgetRecycler which allows widget reusage. MList will put widgets to
        this recycler and its possible to get recycled widget from it
      \param pointer to fully created widget

      \sa MWidgetRecycler
      */
    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const = 0;

    /*!
      \brief This function should be implemented in order to update content of existing list item.

      \param index model index which data should be set
      \param cell widget where data should be updated
      */
    virtual void updateCell(const QModelIndex &index, MWidget *cell) const = 0;

    /*!
      \brief This function should return cell size. Minimum and maximum sizes of cell widget will be updated
      if returned size bigger then maximum size or smaller then minimum size. Only height will be taken into account,
      width of item will be calculate by MList.

      \return cell size
      */
    virtual QSizeF cellSize() const = 0;
};

//! \internal
template <class T>
class MListCellCreatorHelper
{
public:
    static T *createCell(MWidgetRecycler &recycler, const QString &viewType, const QString &objectName) {
        T *cell = static_cast<T *>(recycler.take(T::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new T;

            if (!viewType.isEmpty())
                cell->setViewType(viewType);

            if (!objectName.isEmpty())
                cell->setObjectName(objectName);
        }

        return cell;
    }
};
//! \internal_end


/*!
    \class MAbstractCellCreator
    \brief MAbstractCellCreator is a helper class for implementing creators.

    \section MAbstractCellCreator Overview
        MAbstractCellCreator implements cellSize() function from MCellCreator interface by creating
        one instance of a widget and asking its preferred size. Also it automatically creates widgets
        of specified type specified by the template parameter. It asks recycler first and allocates memory
        only if there is no object in a recycler. With a template parameter you control which objects
        should be created, with setCellViewType(const QString&viewType) you may specify what view type should
        be assigned and with setObjectName(const QString& objectName) you may specify object name of constructed
        cell.

        If you wish to intitialize the cell widget differntly, or if you wish to create
        different widget types for some rows, you should do so in an override of the
        createCell() function. You may call the base class's createCell() when you
        still want the default implementation for a particular row.

        Only updateCell() function should be reimplemented to provide data to widget(cell).

    \section MAbstractCellCreator Examples
        Constructing a creator for MContentItem widget:
        \code
        class MContentItemCreator : public MAbstractCellCreator<MContentItem>
        {
        public:
            MContentItemCreator()
            {
                // in css corresponding selector would be:
                // MContentItem[myCustomViewType]#myObjectName { ....
                setCellViewType("myCustomViewType");
                setCellObjectName("myObjectName");
            }

            void updateCell(const QModelIndex& index, MWidget * cell) const
            {
                // Casting to MContentItem is safe because createCell will create only MContentItem(s).
                MContentItem * contentItem = qobject_cast<MContentItem *>(cell);
                QVariant data = index.data(Qt::DisplayRole);
                // We are assuming here that model returns data in a QStringList: [<title>, <subtitle>, <pictureid>]
                QStringList rowData = data.value<QStringList>();
                contentItem->setTitle(rowData[0]);
                if(rowData.count() > 1)
                    contentItem->setSubtitle(rowData[1]);
                if(rowData.count() > 2){
                    // Pictures is a storage for pictures which we add as a thumbnails
                    contentItem->setPixmap(Pictures::picture([rowData[2]);
                }
            }
        };
        \endcode
  */
template <class T>
class MAbstractCellCreator : public MCellCreator
{
public:
    MAbstractCellCreator() : size(QSizeF(-1, -1)), cellViewType(QString()),
        cellObjectName(QString()) {
        MWidget *cell = new T;
        size = cell->effectiveSizeHint(Qt::PreferredSize);
        delete cell;
    }

    virtual ~MAbstractCellCreator() {}

    virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const {
        MWidget *cell = MListCellCreatorHelper<T>::createCell(recycler, cellViewType, cellObjectName);
        updateCell(index, cell);
        return cell;
    }

    virtual QSizeF cellSize() const {
        return size;
    }

    void setCellViewType(const QString &viewType) {
        cellViewType = viewType;
    }

    void setCellObjectName(const QString &objectName) {
        cellObjectName = objectName;
    }

private:
    QSizeF size;
    QString cellViewType;
    QString cellObjectName;
};

#endif
