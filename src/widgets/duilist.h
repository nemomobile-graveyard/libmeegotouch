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

#ifndef DUILIST_H
#define DUILIST_H

#include <QRectF>

#include "duiwidgetcontroller.h"
#include <duilistmodel.h>

class QGraphicsSceneResizeEvent;
class DuiListPrivate;
class QItemSelectionModel;
class QAbstractItemModel;
class QModelIndex;
class DuiCellCreator;

/*!
  \class DuiList
  \brief DuiList implements a list view.

  DuiList provides support for data that inherits QAbstractItemModel and can be set
  by setItemModel().

  Unlike, Qt model/view widgets, DuiList only shows one "view" column. To show
  multiple model columns in a DuiList, you should create a custom widget in your
  DuiCellCreator::createCell() implementation and use your
  DuiCellCreator::updateCell() implementation to show the various items of data
  from each column in one widget.

  See http://doc.trolltech.com/4.5/model-view-creating-models.html#a-read-only-example-model
  for how to build a custom item model that can be used with DuiList.

  A very minimal basic item model example is shown below. The example model has 1
  items and that item is DuiContentItem.

  Important thing to remember is model defines protocol between model and list. Model defines
  the way data will be passed to DuiCellCreator which will create widgets.

  Model will look like this:
  \code

  class TestModel : public QAbstractListModel
  {
     Q_OBJECT

   public:
     TestModel(QObject *parent = 0)
        : QAbstractListModel(parent) {}

     int rowCount(const QModelIndex &parent = QModelIndex()) const;
     QVariant data(const QModelIndex &index, int role) const;

  };


  int TestModel::rowCount(const QModelIndex &parent) const
  {
     Q_UNUSED(parent);
     return 1;
  }

  QVariant TestModel::data(const QModelIndex &index, int role) const
  {
    if (role == Qt::DisplayRole) {
        QStringList rowData;
        rowData << "Angelina"; // first name
        rowData << "Joli"; // last name
        return QVariant(rowData);
    }

    return QVariant();
  }

  \endcode

  Model doesn't tell DuiList how to create actual widgets, we need DuiCellCreator for that.
  DuiAbstractCellCreator is the best candidate for simple case (keep in mind that you can change style of DuiContentItem only
  in it's constructor, so if other style is needed, DuiAbstractCellCreator will not work, DuiCellCreator should be used instead):

  \code
  class DuiContentItemCreator : public DuiAbstractCellCreator<DuiContentItem>
  {
  public:
      void updateCell(const QModelIndex& index, DuiWidget * cell) const
      {
          DuiContentItem * contentItem = qobject_cast<DuiContentItem *>(cell);
          QVariant data = index.data(Qt::DisplayRole);
          QStringList rowData = data.value<QStringList>();
          contentItem->setTitle(rowData[0]);
          contentItem->setSubtitle(rowData[1]);
      }
  };
  \endcode

  And finally it can be combined in a list:

  \code
    DuiList * list = new DuiList(panel);
    DuiContentItemCreator * cellCreator = new DuiContentItemCreator;
    list->setCellCreator(cellCreator);
    TestModel * model = new TestModel;
    list->setItemModel(model);
  \endcode

  See also DuiListView, DuiWidgetFactory.
 */

class DUI_EXPORT DuiList : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiList)

    /*!
        \property DuiList::showGroups set to true to make group headers visible
    */
    Q_PROPERTY(bool showGroups READ showGroups WRITE setShowGroups)

    /*!
        \property DuiList::columns specifies how many columns the list uses for presenting items
    */
    Q_PROPERTY(int columns READ columns WRITE setColumns)

    /*!
        \property DuiList::selectionMode specifies how selection should work in DuiList
    */
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)

    friend class Ut_DuiList;
    friend class Pt_DuiList;

public:
    /*!
      This enumerated type is used by DuiList to indicate how it reacts to selection by the user.
      */
    enum SelectionMode {
        /*!
          Items cannot be selected.
          */
        NoSelection,
        /*!
          When the user selects an item, any already-selected item becomes unselected.
          */
        SingleSelection,
        /*!
          When the user selects an item in the usual way, the selection status of that item is toggled and the other items are left alone.
          */
        MultiSelection
    };

    enum ScrollHint {
        /*!
          Scroll to ensure that the item is visible.
          */
        EnsureVisibleHint,
        /*!
          Scroll to position the item at the top of the viewport.
          */
        PositionAtTopHint,
        /*!
          Scroll to position the item at the bottom of the viewport.
          */
        PositionAtBottomHint,
        /*!
          Scroll to position the item at the center of the viewport.
          */
        PositionAtCenterHint
    };

    /*!
     * \brief Constructor for creating an empty object.
     * \param parent Parent object.
     */
    DuiList(QGraphicsItem *parent = 0);

    /*!
    * \brief Destructor.
    */
    virtual ~DuiList();

    /*!
        \brief Sets object to fetch displayed items from. Similar to 'setModel' method in QListView.
     */

    /*!
        \brief Sets the model for the view to present.

        This function will create and set a new selection model, replacing any model that was previously set with setSelectionModel().
        However, the old selection model will not be deleted as it may be shared between several views.
        We recommend that you delete the old selection model if it is no longer required. This is done with the following code:

        \code
        QItemSelectionModel *m = view->selectionModel();
        view->setModel(new model);
        delete m;
        \endcode

        If both the old model and the old selection model do not have parents, or if their parents are long-lived objects,
        it may be preferable to call their deleteLater() functions to explicitly delete them.

        DuiList does not take ownership of the model unless it is the model's parent object because the view may be shared between many different DuiList(s).
    */
    void setItemModel(QAbstractItemModel *itemModel);

    /*!
        Returns model, associated with the DuiList.
      */
    QAbstractItemModel *itemModel() const;

    /*!
      Set's cell creator which will map data from model to widgets which will be displayed by DuiList.

      \sa DuiCellCreator
      */
    void setCellCreator(DuiCellCreator *cellCreator);

    /*!
      Returns cell creator associated with DuiList
      */
    const DuiCellCreator *cellCreator() const;

    /*!
        \brief Sets the amount of columns to be used for presenting list items. Set to 1 by default.

        When columns is > 1, the rows will be split vertically, making the cell widget
        narrower, with rows flowing from top-to-bottom into the next column. For
        instance, this could be used to create a "grid" list.
     */
    void setColumns(int columns);
    int columns() const;

    /*!
        \brief Returns the current selection model
    */
    QItemSelectionModel *selectionModel() const;

    /*!
        \brief Sets selection model.
    */
    void setSelectionModel(QItemSelectionModel *selectionModel);

    /*!
        \brief Sets selection mode. By default NoSelection is set.
        Check SelectionMode enumeration for details.
      */
    void setSelectionMode(DuiList::SelectionMode mode);

    /*!
        \return selection mode of a list
      */
    DuiList::SelectionMode selectionMode() const;

    /*!
        \return index of first visible item
      */
    const QModelIndex firstVisibleItem() const;

    /*!
        \return index of last visible item
      */
    const QModelIndex lastVisibleItem() const;

    /*!
      \return <code>true</code> if DuiList shows groups, otherwise false
      */
    bool showGroups() const;

    /*!
      \brief Specifies whether list should show groups or not.
      */
    void setShowGroups(bool showGroups);

public Q_SLOTS:
    /*!
        \brief Convenience function - Select the given item.
        If index is not valid, the current selection is not changed.
     */
    void selectItem(const QModelIndex &index);

    /*!
        \brief Scrolls list to a specific index. Call to function will ensure
        that item with specified index becomes visible.
      */
    void scrollTo(const QModelIndex &index);

    /*!
        \brief Scrolls list to a specific index with specified hint.
      */
    void scrollTo(const QModelIndex &index, ScrollHint hint);

Q_SIGNALS:
    /*!
        \brief Emitted when scrollTo(index) is called to tell the view to scroll
        to the given item index
     */
    void scrollToIndex(const QModelIndex &index);

    /*!
        \brief Emitted when the selection model has changed
    */
    void selectionModelChanged(QItemSelectionModel *selectionModel);

    /*!
        \brief Emitted when an item is clicked
    */
    void itemClicked(const QModelIndex &index);

    /*!
        \brief Emitted when list is moving, e.g. pannable by user
      */
    void panningStarted();

    /*!
        \brief Emitted when list stopped moving
      */
    void panningStopped();

protected:
    DuiList(DuiListPrivate *dd, DuiListModel *model, QGraphicsItem *parent);

    /*!
       Notification of model data modifications.
    */
    virtual void updateData(const QList<const char *>& modifications);

private:
    Q_DECLARE_PRIVATE(DuiList)
    Q_DISABLE_COPY(DuiList)
    friend class DuiListView;
};

#endif
