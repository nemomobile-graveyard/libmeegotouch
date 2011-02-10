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

#ifndef MLIST_H
#define MLIST_H

#include <QRectF>

#include "mwidgetcontroller.h"
#include <mlistmodel.h>

class QGraphicsSceneResizeEvent;
class MListPrivate;
class QItemSelectionModel;
class QAbstractItemModel;
class QModelIndex;
class MCellCreator;
class MListFilter;

/*!
  \class MList
  \brief MList implements a list view.

  MList provides support for data that inherits QAbstractItemModel and can be set
  by setItemModel().

  Unlike, Qt model/view widgets, MList only shows one "view" column. To show
  multiple model columns in a MList, you should create a custom widget in your
  MCellCreator::createCell() implementation and use your
  MCellCreator::updateCell() implementation to show the various items of data
  from each column in one widget.

  See http://doc.trolltech.com/4.5/model-view-creating-models.html#a-read-only-example-model
  for how to build a custom item model that can be used with MList.

  A very minimal basic item model example is shown below. The example model has 1
  items and that item is MContentItem.

  Important thing to remember is model defines protocol between model and list. Model defines
  the way data will be passed to MCellCreator which will create widgets.

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

  Model doesn't tell MList how to create actual widgets, we need MCellCreator for that.
  MAbstractCellCreator is the best candidate for simple case (keep in mind that you can change style of MContentItem only
  in its constructor, so if other style is needed, MAbstractCellCreator will not work, MCellCreator should be used instead):

  \code
  class MContentItemCreator : public MAbstractCellCreator<MContentItem>
  {
  public:
      void updateCell(const QModelIndex& index, MWidget * cell) const
      {
          MContentItem * contentItem = qobject_cast<MContentItem *>(cell);
          QVariant data = index.data(Qt::DisplayRole);
          QStringList rowData = data.value<QStringList>();
          contentItem->setTitle(rowData[0]);
          contentItem->setSubtitle(rowData[1]);
      }
  };
  \endcode

  And finally it can be combined in a list:

  \code
    MList * list = new MList(panel);
    MContentItemCreator * cellCreator = new MContentItemCreator;
    list->setCellCreator(cellCreator);
    TestModel * model = new TestModel;
    list->setItemModel(model);
  \endcode

  See also MListView, MWidgetFactory.
 */

class M_CORE_EXPORT MList : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MList)

    /*!
        \property MList::showGroups set to true to make group headers visible
    */
    Q_PROPERTY(bool showGroups READ showGroups WRITE setShowGroups)

    /*!
        \property MList::columns specifies how many columns the list uses for presenting items
    */
    Q_PROPERTY(int columns READ columns WRITE setColumns)

    /*!
        \property MList::selectionMode specifies how selection should work in MList
    */
    Q_PROPERTY(SelectionMode selectionMode READ selectionMode WRITE setSelectionMode)

    friend class Ut_MList;
    friend class Pt_MList;

public:
    /*!
      This enumerated type is used by MList to indicate how it reacts to selection by the user.
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

    enum DisplayMode {
        /*!
          \brief The list index is always hidden.
        */
        Hide = 0,
        /*!
          \brief The list index is always visible.
        */
        Show,
        /*!
          \brief The list index appears on panning,
            and disappears when panning is stopped.
        */
        Auto,
        /*!
          \brief The list index appears on tapping
            right area of the list.
        */
        Floating
    };

    enum AnimationMode {
        /*!
          \brief The method shall be not animated.
        */
        NonAnimated = 0,
        /*!
          \brief The method shall be animated.
        */
        Animated = 1
    };

    enum ListOptimizationFlag {
        /*!
          MAbstractCellCreator::createCell() will be called only on new items. If items receives update
          only MAbstractCellCreate::updateCell() will be called. Enabled by default.
          */
        DontCallCreateCellDuringUpdate = 0x1
    };
    Q_DECLARE_FLAGS(ListOptimizationFlags, ListOptimizationFlag)

    /*!
     * \brief Constructor for creating an empty object.
     * \param parent Parent object.
     */
    MList(QGraphicsItem *parent = 0);

    /*!
    * \brief Destructor.
    */
    virtual ~MList();

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

        MList does not take ownership of the model unless it is the model's parent object because the view may be shared between many different MList(s).
    */
    void setItemModel(QAbstractItemModel *itemModel);

    /*!
        Returns model, associated with the MList.
      */
    QAbstractItemModel *itemModel() const;

    /*!
      Set's cell creator which will map data from model to widgets which will be displayed by MList. Ownership is transferred to MList.

      \sa MCellCreator
      */
    void setCellCreator(MCellCreator *cellCreator);

    /*!
      Returns cell creator associated with MList
      */
    const MCellCreator *cellCreator() const;

    /*!
      Set's header creator which will map data from model to widgets which will be displayed by MList in the header. Ownership is transferred to MList.

      \sa MCellCreator
      */
    void setHeaderCreator(MCellCreator *cellCreator);

    /*!
      Returns header creator associated with MList
      */
    const MCellCreator *headerCreator() const;

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
    void setSelectionMode(MList::SelectionMode mode);

    /*!
        \return selection mode of a list
      */
    MList::SelectionMode selectionMode() const;

    /*!
        \return index of first visible item
      */
    const QModelIndex firstVisibleItem() const;

    /*!
        \return index of last visible item
      */
    const QModelIndex lastVisibleItem() const;

    /*!
      \return <code>true</code> if MList shows groups, otherwise false
      */
    bool showGroups() const;

    /*!
      \brief Specifies whether list should show groups or not.

      MList will handle item model as a 2 level tree. First level will be headers and second level items for that header.

      Customization of the group header's appearance in the list is done through <code>group-header-object-name</code>. Group header is a label,
      so all styles of label applies to group header.
      Example:
      Application.css
      \code
      MList#customList
      {
        group-header-object-name : "redLabelOnWhiteBackground";
      }

      #redLabelOnWhiteBackground
      {
        color : #FF0000;
        background-color : #FFFFFF;
      }
      \endcode

      Application.cpp
      \code
      MList * list = ....;
      ...
      list->setObjectName("customList");
      ...
      \endcode
      */
    void setShowGroups(bool showGroups);

    /*!
      \return Returns the status of the list index availability.
      \deprecated Please use indexDisplayMode()
      */
    bool indexVisible();

    /*!
        \return filter which implements live filtering of list contents.
     */
    MListFilter *filtering() const;

    /*!
      \return list's optimization flags.
      */
    ListOptimizationFlags optimizationFlags() const;

    /*!
      \brief Sets one optimization flag to enabled if \a enabled is true, otherwise to disabled.
      */
    void setOptimizationFlag(ListOptimizationFlag optimizationFlag, bool enabled = true);

    /*!
      \brief Sets the list optimization flags to \a flags. All flags in \a flags are enabled
      and the others are disabled.
      */
    void setOptimizationFlags(ListOptimizationFlags optimizationFlags);

public Q_SLOTS:
    /*!
        \brief Convenience function - Select the given item.
        If index is not valid, the current selection is not changed.
     */
    void selectItem(const QModelIndex &index);

    /*!
        \brief Convenience function - Emits a long tap event for an item.
        \deprecated Use MList::longTapItem(QModelIndex, QPointF) instead.
     */
    void longTapItem(const QModelIndex &index);

    /*!
        \brief Convenience function - Emits a long tap event for an item. Also provides tap location.
     */
    void longTapItem(const QModelIndex &index, const QPointF &position);

    /*!
        \brief Scrolls list to a specific index. Call to function will ensure
        that item with specified index becomes visible.

        \deprecated Please use the MList::scrollTo(QModelIndex, AnimationMode).
      */
    void scrollTo(const QModelIndex &index);

    /*!
        \brief Scrolls list to a specific index. Call to function will ensure
        that item with specified index becomes visible. The scroll to might be
        executed animated or non-animated.
    */

    void scrollTo(const QModelIndex &index, AnimationMode mode);

    /*!
        \brief Scrolls list to a specific index with specified hint.

        \deprecated Please use the MList::scrollTo(QModelIndex, ScrollHint, AnimationMode).
      */
    void scrollTo(const QModelIndex &index, ScrollHint hint);

    /*!
        \brief Scrolls list to a specific index with specified hint. The method is executed
        either animated or non-animated.
    */
    void scrollTo(const QModelIndex &index, ScrollHint hint, AnimationMode mode);

    /*!
      \brief Specifies whether the list index for a grouped model should be visible or not.
      \deprecated Please use setIndexDisplayMode(MList::DisplayMode);
      */
    void setIndexVisible(bool visible);

    /*!
      \brief Specifies fi the list index bar for a grouped model should be visible or not, or
        automatically to appear on list panning.
      \sa MList::DisplayMode
    */
    void setIndexDisplayMode(DisplayMode displayMode);

    /*!
      \brief Returns visibility mode of list index.
      \sa MList::DisplayMode
      */
    DisplayMode indexDisplayMode() const;

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
        \brief Emitted when an item is long tapped.
        \deprecated The MList::itemLongTapped(QModelIndex, QPointF) extends the signal.
    */
    void itemLongTapped(const QModelIndex &index);

    /*!
        \brief Emitted when an item is long tapped. Also provides tap location.
    */
    void itemLongTapped(const QModelIndex &index, const QPointF &position);

    /*!
        \brief Emitted when list is moving, e.g. pannable by user
      */
    void panningStarted();

    /*!
        \brief Emitted when list stopped moving
      */
    void panningStopped();

protected:
    MList(MListPrivate *dd, MListModel *model, QGraphicsItem *parent);

    /*!
       Notification of model data modifications.
    */
    virtual void updateData(const QList<const char *>& modifications);

    /*!
       Notification of context menu event.
    */
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    /*!
        Handling of key events
     */
    virtual void keyPressEvent(QKeyEvent *event);

    /*!
       Handler of notifications of new receivers connected to MList signals.
    */
    virtual void connectNotify(const char *signal);

    /*!
       Handler of notifications of receivers disconnecting from MList signals.
    */
    virtual void disconnectNotify(const char *signal);

private:
    Q_DECLARE_PRIVATE(MList)
    Q_DISABLE_COPY(MList)
    friend class MListView;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(MList::ListOptimizationFlags)

#endif
