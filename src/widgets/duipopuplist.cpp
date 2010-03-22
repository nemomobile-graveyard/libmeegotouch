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

#include "duipopuplist.h"
#include "duipopuplist_p.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiPopupList)

DuiPopupListPrivate::DuiPopupListPrivate()
{
}

DuiPopupList::DuiPopupList()
    : DuiDialog(new DuiPopupListPrivate(), new DuiPopupListModel(), DuiSceneWindow::PopupList)
{
    setCentralWidget(0);
}

DuiPopupList::DuiPopupList(DuiPopupListPrivate *dd, DuiPopupListModel *model, DuiSceneWindow::WindowType windowType)
    : DuiDialog(dd, model, windowType)
{
    setCentralWidget(0);
}

DuiPopupList::~DuiPopupList()
{
}

void DuiPopupList::setItemModel(QAbstractItemModel *itemModel)
{
    model()->setItemModel(itemModel);

    if (itemModel)
        model()->setSelectionModel(new QItemSelectionModel(itemModel, this));
    else
        model()->setSelectionModel(0);
}

QAbstractItemModel *DuiPopupList::itemModel() const
{
    return model()->itemModel();
}

void DuiPopupList::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel && selectionModel->model() != itemModel()) {
        qWarning("DuiPopupList::setSelectionModel() failed: "
                 "Trying to set a selection model, which works on "
                 "a different model than the view.");
        return;
    }

    model()->setSelectionModel(selectionModel);
}

QItemSelectionModel *DuiPopupList::selectionModel() const
{
    return model()->selectionModel();
}

QModelIndex DuiPopupList::currentIndex() const
{
    if (!selectionModel() || !selectionModel()->hasSelection())
        return QModelIndex();

    return selectionModel()->currentIndex();
}

void DuiPopupList::scrollTo(const QModelIndex &index)
{
    if (index.model() != itemModel()) return;
    emit scrollToIndex(index);
}

void DuiPopupList::click(const QModelIndex &index)
{
    if (index.model() != itemModel()) return;

    if (index == currentIndex()) {
        emit clicked(index);
    } else {
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);

        emit clicked(index);
        emit currentIndexChanged(index);
    }

    accept();
}

void DuiPopupList::setCurrentIndex(const QModelIndex &index)
{
    if (itemModel() == NULL)
        return;

    if (index.isValid() && selectionModel()->model() != index.model()) {
        qWarning("DuiPopupList::selectItem() failed: "
                 "Trying to select an item that is for"
                 " a different model than the view ");
        return;
    }

    // Since we didn't change the current index, no new signals should not send
    if (index == currentIndex()) return;

    if (index.isValid())
        selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    else
        selectionModel()->clear();

    emit currentIndexChanged(index);
}
