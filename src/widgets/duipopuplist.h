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

#ifndef DUIPOPUPLIST_H
#define DUIPOPUPLIST_H

#include "duidialog.h"
#include "duipopuplistmodel.h"

class DuiPopupListPrivate;
class QAbstractItemModel;
class QItemSelectionModel;
class QModelIndex;

/**
    \class DuiPopupList
    \brief DuiPopupList implements a popup list for the Dui Framework.

    \ingroup widgets

    DuiPopupList is providing limited number and predefined/system-defined set of values to the user,
    among which the user can choose one option, the selected option is displayed to the user.
    The alternatives in Popup list are mutually exclusive.

    DuiPopupList derived from DuiDialog.

    It makes heavy use of Qt's Interview Framework to move most code out of the view into in a model
    to be implemented by the user.

    Use one of the various Qt convenience models if subclassing a model is not necessary.

    Currently, we expect the model to have only one level of depth, e.g. a table or list and not a tree model.
    Qt's QComboBox can handle trees to by setting a root index but because writing tree models is more
    complex and showing a sublist is a rather rare use case, API is kept simple and expects simple models.

    \section DuiPopupListUseGuidelines Usage guidelines
        - Guidelines
            - Popup list can be used if you have three or more items to display.
            - In case you have less than 3 items, consider using toggle button. Usage of toggle button will save one finger press (= will save users time).
            - In case you have more items, consider displaying your choices with other component than popup list. Possibly separate view.
            - In case that you need to display dynamically changing item(s), popup list not good way to display the content. It is better to select dialog or some
              other view that is capable of displaying a lot of content. By selecting one view for the user in all situations, you will make interactions to feel more
              predictable.
            - Popup list content items cannot be added or removed by the user.
        - Popup list size:
            - Popup list gets its size based on the longest popup list item. Very long labels inside the popup list will be truncated when device is rotated to portrait.
            - Therefore it is important not to place too long popup list items inside the popup list.
            - Suggested total width for the popup list button is 35-40% of landscape screen width. Label that can be placed inside is button area - popup icon -
              borders around each visual element.
        - Application defines popup list items.
            - You can place inside the popup list similar objects as in list.
            - Prefer labels that have approximately same length.
            - Choices need to be mutually exclusive.
            - Define default popup list value for the popup list.
                - If you know safe choice or most often selected choice from the popup list, display that as default popuplist value.
                - In case there is no default value, the prompt text is displayed as popup list value.
            - Define the order of the popup list items. Use some logical order such as from A to Z, or time-based order.
        - Define the location for the popup list.

    \section DuiPopupListExampleCodes Example codes
        If you just want the user to pick from a list of strings, use QStringListModel like:

        \code
          DuiPopupList *popuplist = new DuiPopupList();
          QStringListModel *model = new QStringListModel(this);
          QStringList stringList;
          stringList << "Item 1" << "Item 2";
          model->setStringList(stringList);
          popuplist->setItemModel(model);
          popuplist->appear();
        \endcode

    \sa DuiPopupListModel DuiPopupListStyle
 */

class DUI_EXPORT DuiPopupList : public DuiDialog
{

    Q_OBJECT
    DUI_CONTROLLER(DuiPopupList)

public:

    /**
        Constructs a new popuplist with no content
     */
    DuiPopupList();

    /**
        Destroys the popuplist.
     */
    virtual ~DuiPopupList();

    /**
        Sets the item model \a model as the new datasource
        for the PopupList. If you do not want to write
        your own model, use QStringList model as
        a convenience adapter

        DuiPopupList does not take ownership of the model.
        It is up the caller to ensure that it will be
        deleted.  If the model is replaced, it is up to the
        caller to ensure the previous model is not leaked

        @arg model The model to use as a new datasource
     */
    virtual void setItemModel(QAbstractItemModel *itemModel);

    /**
        Returns the model that this PopupList is presenting.
    */
    QAbstractItemModel *itemModel() const;

    /**
        Sets the current selection model to the given \a selectionModel.

        Note that, if you call setItemModel() after this function, the given \a selectionModel
        will be replaced by one created by the view.

        \note It is up to the application to delete the old selection model if it is no
        longer needed; i.e., if it is not being used by other views. This will happen
        automatically when its parent object is deleted. However, if it does not have a
        parent, or if the parent is a long-lived object, it may be preferable to call its
        deleteLater() function to explicitly delete it.

        \sa selectionModel(), setItemModel(), clearSelection()
    */
    virtual void setSelectionModel(QItemSelectionModel *selectionModel);

    /**
        Returns the current selection model.
        \sa setSelectionModel()
    */
    QItemSelectionModel *selectionModel() const;

    /**
        Scrolls the view if necessary to ensure that the item at \a index is visible.
    */
    virtual void scrollTo(const QModelIndex &index);

    /**
        Return the model index of the item that is currently being
        shown as the selected item
     *
        @return the current item model index or invalid QModelIndex
    */
    QModelIndex currentIndex() const;

public Q_SLOTS:

    /**
      \brief Performs a click
      All the usual signals associated with a click are emitted as appropriate.
    */
    void click(const QModelIndex &index);

    /**
         Select the given item, deselecting all others.
         This is equivalent to:
         \code
           selectionModel()->select(index, QItemSelectionModel::ClearAndSelect)
         \endcode
         If index is not valid, the current selection is cleared
     */
    void setCurrentIndex(const QModelIndex &index);

Q_SIGNALS:

    /**
        This signal is emitted when the item is clicked.
     */
    void clicked(const QModelIndex &index);

    /**
        This signal is emitted when the current selected item is changed.
     */
    void currentIndexChanged(const QModelIndex &index);

    /**
        This signal is emitted when scrollTo(index) is called to tell the view to scroll to the given item index.
     */
    void scrollToIndex(const QModelIndex &index);

protected:

    //! \internal
    DuiPopupList(DuiPopupListPrivate *dd, DuiPopupListModel *model, DuiSceneWindow::WindowType windowType);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(DuiPopupList)
    Q_DISABLE_COPY(DuiPopupList)

    friend class DuiPopupListView;
    friend class DuiPopupListViewPrivate;
};

#endif

