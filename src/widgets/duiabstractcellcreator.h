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

#ifndef DUILISTCELLCREATOR_H__
#define DUILISTCELLCREATOR_H__

#include <QModelIndex>
#include <QSizeF>
#include <DuiWidgetRecycler>
#include <DuiWidget>

/*!
    \class DuiCellCreator
    \brief Interface for creating and updating items (cells) for DuiList.

    \section DuiCellCreator Overview
        DuiCellCreator is a purely abstract class and should be implemented by every class whose instances
        are intended to create items for DuiList.

        Main purpose of this interface is to create and update data of widgets which has to be inserted
        into DuiList. Also DuiCellCreator returns size of a list item widget.
  */
class DUI_EXPORT DuiCellCreator
{
public:
    /*!
      Default virtual destructor.
      */
    virtual ~DuiCellCreator() {};

    /*!
      \brief When DuiList needs a cell, it will call this function to get a pointer to a widget.
      DuiList keeps ownership of a pointer and will delete object when it's not needed.
      It's not required but highly recommended to use recycler. This function will be called very
      often during panning and to make panning smooth creator shouldn't allocate memory.

      \param index is a model index of the row for which widget should be created
      \param recycler is a DuiWidgetRecycler which allows widget reusage. DuiList will put widgets to
        this recycler and its possible to get recycled widget from it
      \param pointer to a created widget

      \sa DuiWidgetRecycler
      */
    virtual DuiWidget *createCell(const QModelIndex &index, DuiWidgetRecycler &recycler) const = 0;

    /*!
      \brief This function should be implemented in order to update content of existing list item.

      \param index model index which data should be set
      \param cell widget where data should be updated
      */
    virtual void updateCell(const QModelIndex &index, DuiWidget *cell) const = 0;

    /*!
      \brief This function should return cell size. At the moment cells can't be of different size.

      \return cell size
      */
    virtual QSizeF cellSize() const = 0;
};

//! \internal
template <class T>
class DuiListCellCreatorHelper
{
public:
    static T *createCell(DuiWidgetRecycler &recycler, const QString &viewType, const QString &objectName) {
        T *cell = dynamic_cast<T *>(recycler.take(T::staticMetaObject.className()));
        if (cell == NULL) {
            cell = new T;

            if (!viewType.isEmpty())
                cell->setViewType(viewType);

            if (!viewType.isEmpty())
                cell->setObjectName(objectName);
        }

        return cell;
    }
};
//! \internal_end


/*!
    \class DuiAbstractCellCreator
    \brief DuiAbstractCellCreator is a helper class for implementing creators.

    \section DuiAbstractCellCreator Overview
        DuiAbstractCellCreator implements cellSize() function from DuiCellCreator interface by creating
        one instance of a widget and asking it's preffered size. Also it automatically creates widgets
        of specified type specified by the template parameter. It asks recycler first and allocates memory
        only if there is no object in a recycler. With a template parameter you control which objects
        should be created, with setCellViewType(const QString&viewType) you may specify what view type should
        be assigned and with setObjectName(const QString& objectName) you may specify object name of constructed
        cell.

        Only updateCell() function should be reimplemented to provide data to widget(cell).

    \section DuiAbstractCellCreator Examples
        Constructing a creator for DuiContentItem widget:
        \code
        class DuiContentItemCreator : public DuiAbstractCellCreator<DuiContentItem>
        {
        public:
            DuiContentItemCreator()
            {
                // in css corresponding selector would be:
                // DuiContentItem[myCustomViewType]#myObjectName { ....
                setCellViewType("myCustomViewType");
                setCellObjectName("myObjectName");
            }

            void updateCell(const QModelIndex& index, DuiWidget * cell) const
            {
                // Casting to DuiContentItem is safe because createCell will create only DuiContentItem(s).
                DuiContentItem * contentItem = qobject_cast<DuiContentItem *>(cell);
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
class DuiAbstractCellCreator : public DuiCellCreator
{
public:
    DuiAbstractCellCreator() : size(QSizeF(-1, -1)), cellViewType(QString()),
        cellObjectName(QString()) {
        DuiWidget *cell = new T;
        size = cell->effectiveSizeHint(Qt::PreferredSize);
        delete cell;
    }

    virtual ~DuiAbstractCellCreator() {}

    virtual DuiWidget *createCell(const QModelIndex &index, DuiWidgetRecycler &recycler) const {
        DuiWidget *cell = DuiListCellCreatorHelper<T>::createCell(recycler, cellViewType, cellObjectName);
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
