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

#include "mpopuplist.h"
#include "mpopuplist_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MPopupList)

MPopupListPrivate::MPopupListPrivate()
{
}

MPopupList::MPopupList()
    : MDialog(new MPopupListPrivate(), new MPopupListModel(), MSceneWindow::PopupList)
{
    setCentralWidget(0);
}

MPopupList::MPopupList(MPopupListPrivate *dd, MPopupListModel *model, MSceneWindow::WindowType windowType)
    : MDialog(dd, model, windowType)
{
    setCentralWidget(0);
}

MPopupList::~MPopupList()
{
}

void MPopupList::setItemModel(QAbstractItemModel *itemModel)
{
    model()->setItemModel(itemModel);

    if (itemModel)
        model()->setSelectionModel(new QItemSelectionModel(itemModel, this));
    else
        model()->setSelectionModel(0);
}

QAbstractItemModel *MPopupList::itemModel() const
{
    return model()->itemModel();
}

void MPopupList::setSelectionModel(QItemSelectionModel *selectionModel)
{
    if (selectionModel && selectionModel->model() != itemModel()) {
        qWarning("MPopupList::setSelectionModel() failed: "
                 "Trying to set a selection model, which works on "
                 "a different model than the view.");
        return;
    }

    model()->setSelectionModel(selectionModel);
}

QItemSelectionModel *MPopupList::selectionModel() const
{
    return model()->selectionModel();
}

QModelIndex MPopupList::currentIndex() const
{
    if (!selectionModel() || !selectionModel()->hasSelection())
        return QModelIndex();

    return selectionModel()->currentIndex();
}

void MPopupList::scrollTo(const QModelIndex &index)
{
    if (index.model() != itemModel()) return;
    emit scrollToIndex(index);
    model()->setScrollToIndex(index);
}

void MPopupList::click(const QModelIndex &index)
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

void MPopupList::setCurrentIndex(const QModelIndex &index)
{
    if (itemModel() == NULL)
        return;

    if (index.isValid() && selectionModel()->model() != index.model()) {
        qWarning("MPopupList::selectItem() failed: "
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
