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

#ifndef DUIPOPUPLISTVIEW_H
#define DUIPOPUPLISTVIEW_H

#include "duidialogview.h"
#include "duipopuplistmodel.h"
#include "duipopupliststyle.h"

#include <QModelIndex>
#include <QItemSelectionModel>

class DuiPopupListViewPrivate;
class DuiPopupList;
class QAbstractItemModel;

/*!
    \class DuiPopupListView
    \brief View class for standard DuiPopupList.

    \ingroup views

    DuiPopupListView is used to visualize popupList.
 */

class DUI_EXPORT DuiPopupListView : public DuiDialogView
{
    Q_OBJECT
    DUI_VIEW(DuiPopupListModel, DuiPopupListStyle)

public:
    /*!
       \brief Constructor
       \param controller Pointer to the button's controller
     */
    DuiPopupListView(DuiPopupList *controller);

    /*!
       \brief Destructor
     */
    virtual ~DuiPopupListView();

protected Q_SLOTS:

    /*!
       \brief Set the itemModel
     */
    virtual void setItemModel(QAbstractItemModel *itemModel);

    /*!
       \brief Set the selectionModel
     */
    virtual void setSelectionModel(QItemSelectionModel *selectionModel);

    /*!
       \brief Called when position of pannable viewport changes
       \deprecated Since 0.19
     */
    void sizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);

    /*!
        This slot is called when items are changed in the model. The
        changed items are those from \a topLeft to \a bottomRight
        inclusive. If just one item is changed \a topLeft == \a
        bottomRight.
       \deprecated Since 0.19
    */
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

    /*!
        This slot is called when rows are inserted.
       \deprecated Since 0.19

     */
    void rowsInserted(const QModelIndex &parent, int start, int end);

    /*!
        This slot is called when rows are removed.
       \deprecated Since 0.19
     */
    void rowsRemoved(const QModelIndex &parent, int start, int end);

    /*!
        This slot is called when the selection is changed. The previous
        selection (which may be empty), is specified by \a deselected, and the
        new selection by \a selected.
        \deprecated Since 0.19
    */
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

    /*!
        Scrolls the view if necessary to ensure that the item at \a index is visible.
    */
    virtual void scrollTo(const QModelIndex &index) const;

protected:

    //! \internal
    DuiPopupListView(DuiPopupListViewPrivate &dd, DuiPopupList *controller);
    //! \internal_end

private:
    Q_DISABLE_COPY(DuiPopupListView)
    Q_DECLARE_PRIVATE(DuiPopupListView)
};

#endif
