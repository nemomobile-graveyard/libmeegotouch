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

#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H

#include "mwidgetcontroller.h"
#include "mcomboboxmodel.h"
#include <QModelIndex>

class MComboBoxPrivate;

/**
    \class MComboBox
    \brief MComboBox implementation of a comboBox widget.

    \ingroup widgets

    \section MComboBox Overview

        The MComboBox widget is a combined button and popup list.
        It is very similar to QComboBox, but does not allow editing the text.

        Currently, we expect the model to have only one level of depth,  e.g. a table or list and not a tree model.
        Qt's QComboBox can handle trees  by setting a root index but because writing tree models is more complex and
        showing a sublist is a rather rare use case, API is kept simple and expects simple models.

        MComboBox uses the Qt's model/view framework for its popup list and to store its items.
        By default a QStandardItemModel stores the items and a MPopupList displays the popuplist.
        You can access the model by model(), but MComboBox also provides functions
        to set and get item data (e.g., setItemText() and itemText()).
        You can also set a new model.

        A combobox can be populated using the insert functions,
        insertItem(),  insertItems(), addItem() and addItems() for example. Items can be
        changed with setItemText(). An item can be removed with
        removeItem() and all items can be removed with clear(). The text
        of the current item is returned by currentText(), and the text of
        a numbered item is returned with itemText(). The current item can be
        set with setCurrentIndex(). The number of items in the combobox is
        returned by count();

        For example, if you just want the user to pick from a list of strings:

        \code
        MComboBox *combobox = new MComboBox();
        combobox->setTitle("Select an item");
        combobox->setIconID("icon-l-gallery");

        QStringList stringList;
        stringList << "Item 1" << "Item 2";
        combobox->addItems(stringList);
        \endcode
 */

class M_CORE_EXPORT MComboBox : public MWidgetController
{

    Q_OBJECT
    M_CONTROLLER(MComboBox)

    /**
        \property MComboBox::iconID
        \brief See MComboBoxModel::iconID
    */
    Q_PROPERTY(QString iconID READ iconID WRITE setIconID)

    /**
        \property MComboBox::iconVisible
        \brief See MComboBoxModel::iconVisible
    */
    Q_PROPERTY(bool iconVisible READ isIconVisible WRITE setIconVisible)

    /**
        \property MComboBox::title
        \brief See MComboBoxModel::title
    */
    Q_PROPERTY(QString title READ title WRITE setTitle)

    /*!
        \property MComboBox::progressIndicatorVisible
        \brief See MComboBoxModel::progressIndicatorVisible
    */
    Q_PROPERTY(bool progressIndicatorVisible READ isProgressIndicatorVisible WRITE setProgressIndicatorVisible )

    /*!
        \property MComboBox::itemModel
        \brief The QAbstractItemModel which is used by the MComboBox for the list of items.
    */
    Q_PROPERTY(QAbstractItemModel *itemModel READ itemModel WRITE setItemModel)

    /*!
        \property MComboBox::selectionModel
        \brief Keeps track of itemModel selected items.
    */
    Q_PROPERTY(QItemSelectionModel *selectionModel READ selectionModel WRITE setSelectionModel)

    /*!
        \property MComboBox::currentIndex
        \brief The index of the current item in the combobox.

        The current index can change when inserting or removing items.

        By default, for an empty combo box or a combo box in which no current
        item is set, this property has the value of -1.
    */
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex USER true)

    /*!
        \property MComboBox::currentText
        \brief The text of the current item.

        By default, for an empty combo box or a combo box in which no current
        item is set, this property contains the empty string.
    */
    Q_PROPERTY(QString currentText READ currentText STORED false)

    /*!
        \property MComboBox::count
        \brief The number of items in the combobox.

        By default, for an empty combo box, this property has the value of 0.
    */
    Q_PROPERTY(int count READ count STORED false)

public:

    /**
        Constructs a combobox with the given parent
     */
    MComboBox(QGraphicsItem *parent = 0);

    /**
        Destroys the combobox.
    */
    virtual ~MComboBox();

    /**
       \brief Sets the item model \a itemModel as the new datasource for the comboBox.
       itemModel must not be 0.

       MComboBox does not take ownership of the model.
       It is up the caller to ensure that it will be
       deleted.  If the model is replaced, it is up to the
       caller to ensure the previous model is not leaked.

       @arg itemModel The model to use as a new datasource
     */
    virtual void setItemModel(QAbstractItemModel *itemModel);

    /**
        \brief Returns the model that this comboBox is presenting.
    */
    QAbstractItemModel *itemModel() const;

    /**
        Sets the current selection model to the given \a selectionModel.

        Note that, if you call setItemModel() after this function, the given \a selectionModel will be replaced.

        \note It is up to the application to delete the old selection model if it is no
        longer needed; i.e., if it is not being used by other views. This will happen
        automatically when its parent object is deleted. However, if it does not have a
        parent, or if the parent is a long-lived object, it may be preferable to call its
        deleteLater() function to explicitly delete it.

        \sa selectionModel(), setItemModel()
    */
    virtual void setSelectionModel(QItemSelectionModel *selectionModel);

    /**
        \brief Returns the current selection model.
    */
    QItemSelectionModel *selectionModel() const;

    /*!
       Returns the \ref logicalid "logical ID" associated with this comboBox's icon.
     */
    QString iconID() const;

    /**
        \brief Returns true if the icon of the comboBox is visible.
    */
    bool isIconVisible() const;

    /**
        \brief Returns the title of comboBox
    */
    QString title() const;

    /*!
        \brief Returns true if progress indicator is visible
    */
    bool isProgressIndicatorVisible() const;

    /**
        \brief Adds an item to the combobox with the given \a text.
        The item is appended to the list of existing items.
    */
    void addItem(const QString &text);

    /**
        \brief Adds an item to the combobox with the given \a iconID and \a text.
        The item is appended to the list of existing items.
    */
    void addItem(const QString &iconID, const QString &text);

    /**
        \brief Adds each of the strings in the given \a texts to the combobox.
        Each item is appended to the list of existing items in turn.
    */
    void addItems(const QStringList &texts);

    /**
        \brief the number of items in the combobox
        By default, for an empty combo box, this property has a value of 0.
    */
    int count() const;

    /**
        \brief the index of the current item in the combobox.

        The current index can change when inserting or removing items.

        By default, for an empty combo box or a combo box in which no current
        item is set, this property has a value of -1.
    */
    int currentIndex() const;

    /**
        \brief the text of the current item

        By default, for an empty combo box or a combo box in which no current
        item is set, this property contains an empty string.
    */
    QString currentText() const;

    /**
        \brief Returns the text for the given \a index in the combobox.
    */
    QString itemText(int index) const;

    /**
        \brief Returns the iconID for the given \a index in the combobox.
    */
    QString itemIconID(int index) const;

    /**
        \brief Inserts the \a text into the combobox at the given \a index.

        If the index is equal to or higher than the total number of items,
        the new item is appended to the list of existing items. If the
        index is zero or negative, the new item is prepended to the list
        of existing items.

        \sa insertItems()
     */
    void insertItem(int index, const QString &text);

    /**
        \brief Inserts the \a icon, \a text into the combobox at the given \a index.

        If the index is equal to or higher than the total number of items,
        the new item is appended to the list of existing items. If the
        index is zero or negative, the new item is prepended to the list
        of existing items.

        \sa insertItems()
     */
    void insertItem(int index, const QString &icon, const QString &text);

    /**
        \brief Inserts the strings from the \a list into the combobox as separate items,
        starting at the \a index specified.

        If the index is equal to or higher than the total number of items, the new items
        are appended to the list of existing items. If the index is zero or negative, the
        new items are prepended to the list of existing items.

        \sa insertItem()
     */
    void insertItems(int index, const QStringList &list);

    /**
        \brief Removes the item at the given \a index from the combobox.

        This will update the current index if the index is removed.
    */
    void removeItem(int index);

    /**
        \brief Sets the \a text for the item on the given \a index in the combobox.
    */
    void setItemText(int index, const QString &text);

    /**
        \brief Sets the \a iconID for the item on the given \a index in the combobox.
    */
    void setItemIconID(int index, const QString &iconID);


public Q_SLOTS:

    /**
        \brief Clears the combobox, removing all items.

        Note: If you have set an external model on the combobox this model
        will still be cleared when calling this function.
    */
    void clear();

    /*!
       Sets the \ref logicalid "logical ID" associated with this comboBox's icon to \a id.
     */
    void setIconID(const QString &id);

    /**
        \brief Set the visibility of the icon of the comboBox.
    */
    void setIconVisible(bool);

    /**
        \brief Set comboBox title
    */
    void setTitle(const QString &title);

    /*!
      \brief shows\hides progress indicator
    */
    void setProgressIndicatorVisible(bool visible);

    /*!
      \brief shows progress indicator
    */
    inline void showProgressIndicator() {
        setProgressIndicatorVisible(true);
    }
    /*!
      \brief hides progress indicator
    */
    inline void hideProgressIndicator() {
        setProgressIndicatorVisible(false);
    }

    /**
        \brief Set current index
    */
    void setCurrentIndex(int index);

    /*!
      \brief Performs a click.
    */
    void click();

    /*!
      \brief Dismisses popup list programatically.
    */
    void dismiss();

Q_SIGNALS:

    /**
        \brief This signal is emitted when comboBox is clicked.
     */
    void clicked();

    /**
        \brief This signal is emitted when popup list is dismissed.
     */
    void dismissed();

    /**
        \brief This signal is emitted whenever the currentIndex in the combobox changes
        either through user interaction or programmatically.
     */
    void currentIndexChanged(int index);

    /**
        \brief This is an overloaded member function, provided for convenience.

        This signal is sent whenever the currentIndex in the combobox changes either through
        user interaction or programmatically.
        The item's text is passed.
     */
    void currentIndexChanged(const QString &text);

    /**
        \brief This signal is sent when the user chooses an item in the combobox. The item's index is passed.

        Note that this signal is sent even when the choice is not changed.
        If you need to know when the choice actually changes, use signal currentIndexChanged().
      */
    void activated(int index);

    /**
        \brief This signal is sent when the user chooses an item in the combobox. The item's text is passed.

        Note that this signal is sent even when the choice is not changed.
        If you need to know when the choice actually changes, use signal currentIndexChanged().
     */
    void activated(const QString &text);

protected:

    //! \internal
    MComboBox(MComboBoxPrivate *dd, MComboBoxModel *model, QGraphicsItem *parent);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MComboBox)
    Q_DISABLE_COPY(MComboBox)

    Q_PRIVATE_SLOT(d_func(), void _q_modelDestroyed())
    Q_PRIVATE_SLOT(d_func(), void _q_itemModelDataChanged(QModelIndex, QModelIndex))
    Q_PRIVATE_SLOT(d_func(), void _q_itemModelReset())
    Q_PRIVATE_SLOT(d_func(), void _q_selectionModelSelectionChanged(QItemSelection, QItemSelection))
    Q_PRIVATE_SLOT(d_func(), void _q_itemClicked(QModelIndex))

#ifdef UNIT_TEST
    friend class Ut_MComboBox;
#endif
};

#endif

